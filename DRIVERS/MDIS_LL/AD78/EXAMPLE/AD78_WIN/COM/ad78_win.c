/****************************************************************************
 ************                                                    ************
 ************                   AD78_WIN                        ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file ad78_win.c
 *       \author dieter.pfeuffer@men.de
 *        $Date: 2009/08/31 14:35:44 $
 *    $Revision: 1.2 $
 *
 *        \brief Windows service for the AD78 driver.
 *
 *               The service initiates a PSU power off (AD78_SWOFF) during
 *               Windows shutdown and performs a Windows shutdown if
 *               requested from the PSU (polls AD78_DOWN_EVT).
 *
 *     Required: libraries: mdis_api, usr_oss
 *     \switches DBG
 */
 /*
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \page _AD78_WIN AD78_WIN Windows service

	The AD78_WIN Windows service is an example that demonstrates the usage of
	the AD78 driver from a Windows service.

    \n \section _AD78_WIN_FuncDesc Functional Description

	The AD78_WIN Windows service performs the following tasks:
	- ServiceJob(): sends an ON Acknowledge (#AD78_ONACK) when the service starts
	- configures the AD78 PSU:
	  - no shutdown delay (#AD78_DOWN_DELAY)
	  - 1min OFF delay (#AD78_OFF_DELAY)
	- polls every second the shutdown event flag (#AD78_DOWN_EVT), and if the flag is set:
	  - forces a Windows shutdown
    - sends an SW power off (#AD78_SWOFF) to the PSU if a Windows shutdown is in progress\n

	<b>Please, refer to the example section to view implementation details in the source code.</b>

    \n \section _AD78_WIN_Usage AD78_WIN Usage
	The AD78_WIN service must be copied to the target system and then be registered with the
	Service Control utility (SC.exe) that is included in Windows XP and the Microsoft Platform SDK:
	\code

	C:\>sc create ad78_win binPath= C:\SERVICES\ad78_win.exe start= auto
	\endcode\n

	After this, the service will be automatically started during the Windows boot.
	However, the service can be manually started/stopped with the Windows services
	control panel utility or the SC.exe tool:
	\code

	C:\>sc start ad78_win
	\endcode
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>

#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/ad78_drv.h>
#include <MEN/ad78c.h>

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
#define SLEEP_TIME		1000
#define DBG_NAME		"ad78_win"
#define MAX_DEVNAME_NBR	99

#ifdef DBG
#   define DBGWRT(_x_)	DbgWrite _x_
#else
#   define DBGWRT(_x_)
#endif

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/
SERVICE_STATUS			G_serviceStatus;
SERVICE_STATUS_HANDLE	G_statusHdl;
static MDIS_PATH 		G_path;
BOOL					G_ShutdownFromPsu;

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
void  ServiceMain( int argc, char** argv );
void  ControlHandler( DWORD request );
static int ServiceJob( void );
static int SystemShutdown();
static void PrintError( char *info );
static void DbgWrite( char *fmt, ... );

/********************************* main ************************************/
/** Program main function
 */
void main()
{
	SERVICE_TABLE_ENTRY ServiceTable[2];

	DBGWRT(( "%s: main()\n", DBG_NAME ));

	ServiceTable[0].lpServiceName = "Ad78Service";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

	// start the control dispatcher thread for our service
    StartServiceCtrlDispatcher( ServiceTable );
}

/****************************** ServiceMain ********************************/
/** Service main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 */
void ServiceMain( int argc, char** argv )
{
    int error=-1;

	DBGWRT(( "%s: ServiceMain() entered\n", DBG_NAME ));

	G_ShutdownFromPsu = FALSE;

    G_serviceStatus.dwServiceType        = SERVICE_WIN32;
    G_serviceStatus.dwCurrentState       = SERVICE_START_PENDING;
    G_serviceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    G_serviceStatus.dwWin32ExitCode      = 0;
    G_serviceStatus.dwServiceSpecificExitCode = 0;
    G_serviceStatus.dwCheckPoint         = 0;
    G_serviceStatus.dwWaitHint           = 0;

	// registering control handler
    G_statusHdl = RegisterServiceCtrlHandler(
				"Ad78Service",
				(LPHANDLER_FUNCTION)ControlHandler);
    if (G_statusHdl == (SERVICE_STATUS_HANDLE)0){
        return;
    }

	// report running status to SCM
    G_serviceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus( G_statusHdl, &G_serviceStatus );

	// perform the service
	error = ServiceJob();

	G_serviceStatus.dwWin32ExitCode = error;
	G_serviceStatus.dwCurrentState  = SERVICE_STOPPED;
	SetServiceStatus( G_statusHdl, &G_serviceStatus );

	DBGWRT(( "%s: exiting ServiceMain()\n", DBG_NAME ));

    return;
}

/****************************** ServiceJob ********************************/
/** Service job
 *
 *  \return	          success (0) or error (-1)
 */
static int ServiceJob( void )
{
	int32	n, powerDown;
	char	devName[20];
    int		error=-1;
	u_int8	mode;

	DBGWRT(( "%s: ServiceJob() entered\n", DBG_NAME ));

	/*--------------------+
    |  open G_path        |
    +--------------------*/

	// We cannot specify the device name as an argument if the service
	// is started automatically. So we try to open the AD78 device by
	// enumerate the device name (ad78_1, ad78_2, ...).
	for( n=1; n<MAX_DEVNAME_NBR; n++ ){
		sprintf( devName, "ad78_%d", n );
		if( (G_path = M_open(devName)) >= 0 )
			break;
	}

	if( G_path < 0 ){
		PrintError("open");
		return( -1 );
	}

	DBGWRT(( "%s: device %s opened\n", DBG_NAME, devName ));

	/*------------------------+
    |  power up confirmation  |
    +------------------------*/
	DBGWRT(( "%s: -> M_setstat(AD78_ONACK)\n", DBG_NAME ));
	if ((M_setstat(G_path, AD78_ONACK, 0)) < 0) {
		PrintError("setstat AD78_ONACK");
		goto abort;
	}

	/*---------------------------+
    |  power down configuration  |
    +---------------------------*/
	/* no delay between power down initiation and power down signal */
	AD78C_DOWN_DELAY_MIN2MODE(0,mode);
	DBGWRT(( "%s: -> M_setstat(AD78_DOWN_DELAY, %d)\n", DBG_NAME, mode ));
	if ((M_setstat(G_path, AD78_DOWN_DELAY, mode)) < 0) {
		PrintError("setstat AD78_DOWN_DELAY");
		goto abort;
	}

	/* set time to wait from power down signal to powering off the PSU output voltage */
	AD78C_OFF_DELAY_MIN2MODE(1,mode);
	DBGWRT(( "%s: -> M_setstat(AD78_OFF_DELAY, %d)\n", DBG_NAME, mode ));
	if ((M_setstat(G_path, AD78_OFF_DELAY, mode)) < 0) {
		PrintError("setstat AD78_OFF_DELAY");
		goto abort;
	}

    // loop until shutdown or stop
    while (G_serviceStatus.dwCurrentState == SERVICE_RUNNING){

		UOS_Delay(SLEEP_TIME);

		// power down request already received?
		if( G_ShutdownFromPsu == TRUE ){
			// just wait until we will die
			continue;
		}

		/*------------------------+
		|  power down request?    |
		+------------------------*/
		DBGWRT(( "%s: <- M_getstat(AD78_DOWN_EVT)\n", DBG_NAME ));
		if ((M_getstat(G_path, AD78_DOWN_EVT, &powerDown)) < 0) {
			PrintError("getstat AD78_DOWN_EVT");
			goto abort;
		}

		if( powerDown ){

			DBGWRT(( "%s: >>> power down request\n", DBG_NAME ));
			G_ShutdownFromPsu = TRUE;

			/*----------------------------+
			|  initiate Windows shutdown  |
			+----------------------------*/
			if( SystemShutdown() )
				goto abort;

		}
	} // while

	error = 0;

abort:
	/*--------------------+
    |  cleanup            |
    +--------------------*/

	if (M_close(G_path) < 0)
		PrintError("close");

	DBGWRT(( "%s: exiting ServiceJob()\n", DBG_NAME ));

    return(error);
}

/********************************* ControlHandler *****************************/
/** Handler for windows control events
 *
 *  \param request       \IN  request type
 */
void ControlHandler( DWORD request )
{
    switch(request)
    {
        case SERVICE_CONTROL_STOP:
			DBGWRT(( "%s: >>> SERVICE_CONTROL_STOP received\n", DBG_NAME ));

            G_serviceStatus.dwWin32ExitCode = 0;
            G_serviceStatus.dwCurrentState  = SERVICE_STOPPED;
            break;

        case SERVICE_CONTROL_SHUTDOWN:
			DBGWRT(( "%s: >>> SERVICE_CONTROL_SHUTDOWN received\n", DBG_NAME ));

            G_serviceStatus.dwWin32ExitCode = 0;
            G_serviceStatus.dwCurrentState  = SERVICE_STOPPED;

			// shutdown request not from PSU?
			if( G_ShutdownFromPsu == FALSE ){
				/*--------------------------+
				|  perform SW power off     |
				+--------------------------*/
				DBGWRT(( "%s: -> M_setstat(AD78_SWOFF)\n", DBG_NAME ));
				if ((M_setstat(G_path, AD78_SWOFF, 0)) < 0) {
					PrintError("setstat AD78_SWOFF");
					G_serviceStatus.dwWin32ExitCode = -1;
				}
			}
			break;

        default:
            break;
    }

    // report current status
    SetServiceStatus( G_statusHdl,  &G_serviceStatus );

    return;
}

/********************************* SystemShutdown ******************************/
/** Shutdown the system
 *
 *  \return	          success (0) or error (-1)
 */
static int SystemShutdown()
{
	HANDLE				hToken;
	TOKEN_PRIVILEGES	tkp;

	// get a token for this process
	if( !OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) )
		return( -1 );

	// get the LUID for the shutdown privilege
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount				= 1;  // one privilege to set
	tkp.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED;

	// get the shutdown privilege for this process
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if( GetLastError() != ERROR_SUCCESS )
		return -1;

	// shutdown the system and force all applications to close
	if( !ExitWindowsEx(
			EWX_SHUTDOWN | EWX_FORCE,
			SHTDN_REASON_MAJOR_POWER |
			SHTDN_REASON_MINOR_ENVIRONMENT) )
      return -1;

   return 0;
}


/********************************* PrintError ******************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
 */
static void PrintError(char *info)
{
	static char strBuf[500];

	sprintf( strBuf, "*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
	DBGWRT(( "%s: %s\n", DBG_NAME, strBuf ));
}

/********************************* DbgWrite *********************************/
/** Print a debug message
 *
 *  \param *fmt       \IN format string
 *  \param ...        \IN argument list
 */
static void DbgWrite( char *fmt, ... )
{
	char		outBuf[500];
	va_list		argptr;

	va_start( argptr, fmt );

	vsprintf( outBuf, fmt, argptr );
	OutputDebugString( outBuf );

	va_end( argptr );
}