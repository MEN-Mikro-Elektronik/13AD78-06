/****************************************************************************
 ************                                                    ************
 ************                   AD78_SIMP                        ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file ad78_simp.c
 *       \author dieter.pfeuffer@men.de
 *        $Date: 2009/09/01 09:45:32 $
 *    $Revision: 1.3 $
 *
 *        \brief Simple example program for the AD78 driver.
 *
 *               Control the AD78 PSU.
 *
 *     Required: libraries: mdis_api, usr_oss
 *     \switches (none)
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: ad78_simp.c,v $
 * Revision 1.3  2009/09/01 09:45:32  MRoth
 * R: MDVE warnings
 * M: removed unused variables
 *
 * Revision 1.2  2009/08/31 14:35:42  MRoth
 * R: Porting to MDIS5
 * M: added support for 64bit (MDIS_PATH)
 *
 * Revision 1.1  2006/05/26 15:06:32  DPfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/ad78_drv.h>
#include <MEN/ad78c.h>
#include <MEN/wdog.h>

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void PrintError(char *info);

/********************************* main ************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return	          success (0) or error (1)
 */
int main(int argc, char *argv[])
{
	MDIS_PATH path;
	char	  *device;
	int32	  powerDown, counter;
	u_int8	  mode;

	if (argc < 2 || strcmp(argv[1],"-?")==0) {
		printf("Syntax: ad78_simp <device>\n");
		printf("Function: AD78 example to control the AD78 PSU\n");
		printf("Options:\n");
		printf("    device       device name\n");
		printf("\n");
		return(1);
	}

	device = argv[1];

	/*--------------------+
    |  open path          |
    +--------------------*/
	if ((path = M_open(device)) < 0) {
		PrintError("open");
		return(1);
	}

	/*------------------------+
    |  power up confirmation  |
    +------------------------*/
	printf("- set ON Acknowledge Timeout: none\n");
	if ((M_setstat(path, AD78_ONACK, 0)) < 0) {
		PrintError("setstat AD78_ONACK");
		goto abort;
	}

	/*---------------------------+
    |  power down configuration  |
    +---------------------------*/
	/* no delay between power down initiation and power down signal */
	printf("- set Shutdown Delay: none\n");
	AD78C_DOWN_DELAY_MIN2MODE(0,mode);
	if ((M_setstat(path, AD78_DOWN_DELAY, mode)) < 0) {
		PrintError("setstat AD78_DOWN_DELAY");
		goto abort;
	}

	/* wait 2min from power down signal to powering off the PSU output voltage */
	printf("- set OFF Delay: 2min\n");
	AD78C_OFF_DELAY_MIN2MODE(2,mode);
	if ((M_setstat(path, AD78_OFF_DELAY, mode)) < 0) {
		PrintError("setstat AD78_OFF_DELAY");
		goto abort;
	}

	/*------------------------+
    |  watchdog config+start  |
    +------------------------*/
	/* set watchdog trigger time to 2sec */
	printf("- set Watchdog Trigger Time: 2sec\n");
	if ((M_setstat(path, WDOG_TIME, 2000)) < 0) {
		PrintError("setstat WDOG_TIME");
		goto abort;
	}

	/* start watchdog */
	printf("- start watchdog\n");
	if ((M_setstat(path, WDOG_START, 0)) < 0) {
		PrintError("setstat WDOG_START");
		goto abort;
	}

	/* repeat until keypress */
	printf("- loop until keypress:\n");
	counter = 1;
	do {
		printf("  trigger watchdog and check power down request: %5d times\r", counter++);
		/*------------------------+
		|  watchdog trigger       |
		+------------------------*/
		if ((M_setstat(path, WDOG_TRIG, 0)) < 0) {
			PrintError("setstat WDOG_TRIG");
			goto abort;
		}

		/*------------------------+
		|  power down request?    |
		+------------------------*/
		if ((M_getstat(path, AD78_DOWN_EVT, &powerDown)) < 0) {
			PrintError("getstat AD78_DOWN_EVT");
			goto abort;
		}

		if( powerDown ){

			printf(	"\n"
					"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
					"!!!                                             !!!\n"
					"!!! THE PSU OUTPUT VOLTAGE WILL BE SWITCHED OFF !!!\n"
					"!!! IN 2 MINUTES.                               !!!\n"
					"!!!                                  +-----+    !!!\n"
					"!!! YOU HAVE TO SHUTDOWN YOUR SYSTEM | NOW |    !!!\n"
					"!!!                                  +-----+    !!!\n"
					"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			/*
			 * Note:
			 * The Watchdog was automatically stopped from the PSU controller.
			 * So we just close the path here.
			 */
			M_close(path);

			return 0;
		}

		UOS_Delay(1000);

	} while( UOS_KeyPressed() == -1 );

	printf(	"\n" );

	/*--------------------+
    |  cleanup            |
    +--------------------*/
	abort:

	/* stop watchdog */
	printf("- stop watchdog\n");
	if ((M_setstat(path, WDOG_STOP, 0)) < 0) {
		PrintError("setstat WDOG_STOP");
		goto abort;
	}

	if (M_close(path) < 0)
		PrintError("close");

	return(0);
}

/********************************* PrintError ******************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
*/
static void PrintError(char *info)
{
	printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}
