/****************************************************************************
 ************                                                    ************
 ************                    AD78_TEST                        ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file ad78_test.c
 *       \author dieter.pfeuffer@men.de
 *
 *        \brief Test tool for AD78 PIC
 *
 *     Required: libraries: mdis_api, usr_oss, usr_utl
 *     \switches (none)
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright (c) 2006-2019, MEN Mikro Elektronik GmbH
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

#include <stdio.h>
#include <stdlib.h>
#include <MEN/men_typs.h>
#include <MEN/usr_oss.h>
#include <MEN/usr_utl.h>
#include <MEN/mdis_api.h>
#include <MEN/ad78_drv.h>
#include <MEN/ad78c.h>
#include <MEN/wdog.h>

static const char IdentString[]=MENT_XSTR(MAK_REVISION);

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/
static MDIS_PATH  G_Path;
static u_int32	  G_TimeStart;

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
#define NONE		-1
#define	PWR_ON		1
#define	PWR_OFF		0

#define MSEC_2_MIN_SEC( msec, min, sec )	\
	min = (msec) / (60*1000);	\
	sec = (double)((msec) % (60*1000))/1000.0;

#define	TIMER_START \
	G_TimeStart = UOS_MsecTimerGet();

#define TIMER_GET_MIN_SEC( min, sec )				\
	MSEC_2_MIN_SEC( UOS_MsecTimerGet() - G_TimeStart, min, sec )

#define TIMER_GET_MSEC \
	(UOS_MsecTimerGet() - G_TimeStart )

#define TIMER_GET_SEC \
	((UOS_MsecTimerGet() - G_TimeStart) / 1000 )

#define FWARE_DELAY		10				/* ms */
#define POLL_TIME		100				/* ms */
#define DELAYTIME		500				/* ms */
#define USERTIME		5000			/* ms */
#define _TOUT_OFFSET(t)	(((t)*3)/100)	/* 3% */
#define TOUT_OFFSET(t)	(_TOUT_OFFSET(t) < DELAYTIME ? \
							DELAYTIME : _TOUT_OFFSET(t))	/* 3% or DELAYTIME */
#define TOUT_MIN(t)		((t) - TOUT_OFFSET(t))
#define TOUT_MAX(t)		((t) + TOUT_OFFSET(t))

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void usage(void);
static void PrintMdisError(char *info);

static int TestSwOffOn( u_int32 downDelay, u_int32 offDelay, u_int32 wot );
static int Wait4DownEvent( u_int32 timeout );
static int Wait4PwrOnOff( u_int32 state, u_int32 timeout );
static int WotDuringDowndelay( u_int32 timeout );
static int TestKeyOffOn( u_int32 downDelay, u_int32 offDelay );
static int TestOffAck( void );
static int TestOnAck( u_int32 testLevel );
static int TestWdog( u_int32 wdog );
static int CheckRstErroffErrcount( u_int32 *rst, u_int32 *errOff, u_int32 *ackErr, u_int32 *wdogErr );
static int Restart( void );
static int WdogOnOff( u_int32 enable );

/********************************* usage ***********************************/
/**  Print program usage
 */
static void usage(void)
{
	printf("Usage: ad78_test [<opts>] <device> [<opts>]\n");
	printf("Function: Test AD78 PIC\n");
	printf("Options:\n");
	printf("    device       device name                                \n");
	printf("    -s=<level>   test SWOFF, DOWN_DELAY, OFF_DELAY, WOT     \n");
	printf("                   level=1/2/3 : short/medium/complete test \n");
	printf("    -k           test key off/on                            \n");
	printf("                   Note: requires user interaction          \n");
	printf("    -a=<level>   test on/off acknowledge                    \n");
	printf("                   level=1/2 : short/complete test          \n");
	printf("    -w=<level>   test watchdog                              \n");
	printf("                   level=1/2 : short/complete test          \n");
	printf("    -f=<time>    show firmware internal state               \n");
	printf("                   poll all <time> in ms in a loop          \n");
	printf("\n");
	printf("Copyright (c) 2006-2019, MEN Mikro Elektronik GmbH\n%s\n", IdentString);
}

/********************************* main ************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return	          success (0) or error (1)
 */
int main( int argc, char *argv[])
{
	char	*device,*str,*errstr,buf[40];
	int32	n, val, ret=1;
	int32	swOff, keyOff, onOffAck, fState, wdog;

	/*--------------------+
    |  check arguments    |
    +--------------------*/
	if( (errstr = UTL_ILLIOPT("s=ka=f=w=?", buf)) ){	/* check args */
		printf("*** %s\n", errstr);
		return(1);
	}

	if( UTL_TSTOPT("?") ){						/* help requested ? */
		usage();
		return(1);
	}

	/*--------------------+
    |  get arguments      |
    +--------------------*/
	for (device=NULL, n=1; n<argc; n++)
		if( *argv[n] != '-' ){
			device = argv[n];
			break;
		}

	if( !device || argc<3 ) {
		usage();
		return(1);
	}

	swOff  = ((str = UTL_TSTOPT("s=")) ? atoi(str) : NONE);
	keyOff = (UTL_TSTOPT("k") ? 1 : NONE);
	onOffAck  = ((str = UTL_TSTOPT("a=")) ? atoi(str) : NONE);
	wdog = ((str = UTL_TSTOPT("w=")) ? atoi(str) : NONE);
	fState  = ((str = UTL_TSTOPT("f=")) ? atoi(str) : NONE);

	/*--------------------+
    |  open path          |
    +--------------------*/
	if( (G_Path = M_open(device)) < 0) {
		PrintMdisError("open");
		return(1);
	}

	/*--------------------+
    |  enable test mode   |
    +--------------------*/
	printf("=== enable test mode ===\n");
	if( (M_setstat(G_Path, AD78_TEST1, AD78C_TEST1_TMODE1)) < 0) {
			PrintMdisError("setstat AD78_TEST1");
		goto abort;
	}

	if( swOff != NONE ){
		/*------------------------------------+
		|  SWOFF, DOWN_DELAY, OFF_DELAY, WOT  |
		+------------------------------------*/

		struct _DELAY_TABLE
		{
			int32 downDelay		/* 0..64min */;
			int32 offDelay;		/* 0..16min */
			int32 wot;			/* 0..65535min (should be <> downDelay for test) */
			int32 testLevel;	/* 1,2,3 */
		} delay[] =
		{ /* down   off      wot      level */
			/* short test */
			{ 0,	 0,		  1,		1},
			{ 0,	 1,		  1,		1},
			{ 2,	 0,		  1,		1},	/* wot during downDelay */
			{ 1,	 1,		  2,		1},
			/* medium test */
			{ 1,	 2,		  2,		2},
			{ 2,	 1,		  3,		2},
			{ 2,	 2,		  3,		2},
			/* complete test */
			{ 0,	 4,		 60,		3},
			{ 0,	 8,		255,		3},
			{ 0,	16,		720,		3},
			{ 4,	 0,		  3,		3}, /* wot during downDelay */
			{ 8,	 0,		  9,		3},
			{16,	 0,		 17,		3},
			{32,	 0,		 33,		3},
			{64,	 0,		 65,		3},
		};

		printf("=== test SWOFF, DOWN_DELAY, OFF_DELAY, WOT ===\n");

		/* set ONACK timeout */
		printf("- set onAckTout=none\n");
		if( (M_setstat(G_Path, AD78_ONACK_TOUT, AD78C_ONACK_TOUT_NONE)) < 0) {
			PrintMdisError("setstat AD78_ONACK_TOUT");
			return 1;
		}

		for( n=0; n<sizeof(delay)/sizeof(struct _DELAY_TABLE); n++ ){

			if( delay[n].testLevel > swOff )
				continue;

			if( (TestSwOffOn( delay[n].downDelay, delay[n].offDelay, delay[n].wot )) )
				goto abort;
		}
	}

	if( keyOff != NONE ){
		/*-------------------------------+
		|  KEY OFF/ON                    |
		+-------------------------------*/
		printf("=== test key off/on ===\n");

		if( (TestKeyOffOn( 0, 1 )) )
			goto abort;
	}

	if( onOffAck != NONE ){

		/*-------------------------------+
		|  OFF ACKNOWLEDGE               |
		+-------------------------------*/
		if( (TestOffAck()) )
			goto abort;

		/*-------------------------------+
		|  ON ACKNOWLEDGE                |
		+-------------------------------*/
		if( (TestOnAck( onOffAck )) )
			goto abort;
	}

	if( wdog != NONE ){

		/*-------------------------------+
		|  WATCHDOG                      |
		+-------------------------------*/
		if( (TestWdog( wdog )) )
			goto abort;
	}

	if( fState != NONE ){
		/*-------------------------------+
		|  Firmware State                |
		+-------------------------------*/
		char *str;
		printf("=== poll firmware state all %dms (until keypress) ===\n", fState);
		/* repeat until keypress */
		do {

			if ((M_getstat(G_Path, AD78_TEST4, &val)) < 0) {
				PrintMdisError("getstat AD78_TEST4");
				goto abort;
			}

			switch( val ){
			case SM_STATE_OFF:
				str = "SM_STATE_OFF";
				break;
			case SM_STATE_WAIT_SMB_ON_ACK:
				str = "SM_STATE_WAIT_SMB_ON_ACK";
				break;
			case SM_STATE_ON:
				str = "SM_STATE_ON";
				break;
			case SM_STATE_WAIT_SHUTDOWN_DELAY:
				str = "SM_STATE_WAIT_SHUTDOWN_DELAY";
				break;
			case SM_STATE_WAIT_OFF_DELAY:
				str = "SM_STATE_WAIT_OFF_DELAY";
				break;
			case SM_STATE_WAIT_OFF:
				str = "SM_STATE_WAIT_OFF";
				break;
			case SM_STATE_ERROR_OFF:
				str = "SM_STATE_ERROR_OFF";
				break;
			default:
				str = "UNKNOWN";
			}

			printf("- firmware state: %s (%d)                          \r", str, val);
			UOS_Delay(fState);

		} while( (UOS_KeyPressed() == -1) );

		printf("\n");
	}

	ret = 0;

	/*--------------------+
    |  cleanup            |
    +--------------------*/
	abort:

	if( ret )
		printf("*** SOME TESTS FAILED ***\n");
	else
		printf("=== ALL TESTS SUCCEEDED ===\n");

	/*--------------------+
    |  disable test mode  |
    +--------------------*/
	printf("=> press any key to disable the test mode\n");
	UOS_KeyWait();

	printf("=== disable test mode ===\n");
	if( (M_setstat(G_Path, AD78_TEST1, AD78C_TEST1_NOTEST)) < 0) {
			PrintMdisError("setstat AD78_TEST1");
		goto abort;
	}

	if( M_close(G_Path) < 0 )
		PrintMdisError("close");

	return(0);
}

/********************************* PrintMdisError **************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
*/
static void PrintMdisError(char *info)
{
	printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}

/********************************* TestSwOffOn *****************************/
/** Test SWOFF, DOWN_DELAY, OFF_DELAY, WOT
 *
 *  \param downDelay	\IN  down delay [min]
 *  \param offDelay		\IN  off delay [min]
 *  \param wot			\IN  wot [min]
 *
 *  \return				0 success or error code
*/
static int TestSwOffOn( u_int32 downDelay, u_int32 offDelay, u_int32 wot )
{
	u_int8	mode;
	u_int32	min;
	double	sec;

	printf("\n- set downDelay=%dmin, offDelay=%dmin, wot=%dmin\n",
		downDelay, offDelay, wot);

	/* set shutdown delay */
	AD78C_DOWN_DELAY_MIN2MODE( downDelay, mode );
	if( (M_setstat(G_Path, AD78_DOWN_DELAY, mode)) < 0) {
		PrintMdisError("setstat AD78_DOWN_DELAY");
		return 1;
	}

	/* set off delay */
	AD78C_OFF_DELAY_MIN2MODE( offDelay, mode );
	if( (M_setstat(G_Path, AD78_OFF_DELAY, mode)) < 0) {
		PrintMdisError("setstat AD78_OFF_DELAY");
		return 1;
	}

	/* set wake on time */
	if( (M_setstat(G_Path, AD78_WOT, wot)) < 0) {
		PrintMdisError("setstat AD78_WOT");
		return 1;
	}

	/* initiate a software shutdown */
	printf("- send SWOFF\n");
	if( (M_setstat(G_Path, AD78_SWOFF, 0)) < 0) {
		PrintMdisError("setstat AD78_SWOFF");
		return 1;
	}

	TIMER_START

	/* wot during downDelay? */
	if( wot < downDelay ){
		if( (WotDuringDowndelay( TOUT_MAX(downDelay*60*1000) )) )
			return 1;

		TIMER_GET_MIN_SEC( min, sec );

		printf("- WOT %dmin%.3fs after SWOFF\n", min, sec);
	}
	/* wot after downDelay */
	else{
		if( (Wait4DownEvent( TOUT_MAX(downDelay*60*1000) )) )
			return 1;

		TIMER_GET_MIN_SEC( min, sec );
		printf("- DOWN_EVENT %dmin%.3fs after SWOFF\n", min, sec);

		TIMER_START
		if( (Wait4PwrOnOff( PWR_OFF, TOUT_MAX(offDelay*60*1000) )) )
			return 1;

		TIMER_GET_MIN_SEC( min, sec );
		printf("- PWR_OFF %dmin%.3fs after DOWN_EVENT\n", min, sec);

		TIMER_START
		if( (Wait4PwrOnOff( PWR_ON, TOUT_MAX(
				AD78C_WAIT_OFF + (wot-(downDelay+offDelay))*60*1000) )) )
			return 1;

		TIMER_GET_MIN_SEC( min, sec );
		printf("- WOT/PWR_ON %dmin%.3fs after PWR_OFF\n", min, sec);
	}


	return 0;
}

/********************************* TestKeyOffOn ****************************/
/** Test key off/on
 *
 *  \param downDelay	\IN  down delay [min]
 *  \param offDelay		\IN  off delay [min]
 *
 *  \return				0 success or error code
*/
static int TestKeyOffOn( u_int32 downDelay, u_int32 offDelay )
{
	u_int8	mode;
	u_int32	min;
	double	sec;

	printf("- downDelay=%dmin, offDelay=%dmin\n",
		downDelay, offDelay);

	/* set shutdown delay */
	AD78C_DOWN_DELAY_MIN2MODE( downDelay, mode );
	if( (M_setstat(G_Path, AD78_DOWN_DELAY, mode)) < 0) {
		PrintMdisError("setstat AD78_DOWN_DELAY");
		return 1;
	}

	/* set off delay */
	AD78C_OFF_DELAY_MIN2MODE( offDelay, mode );
	if( (M_setstat(G_Path, AD78_OFF_DELAY, mode)) < 0) {
		PrintMdisError("setstat AD78_OFF_DELAY");
		return 1;
	}

	printf("=> Please, switch the key off within %ds\n", USERTIME/1000);

	TIMER_START
	if( (Wait4DownEvent( downDelay*60*1000 + USERTIME )) )
		goto ERROR_CLEANUP;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- DOWN_EVENT after %dmin%.3fs\n", min, sec);

	TIMER_START
	if( (Wait4PwrOnOff( PWR_OFF, TOUT_MAX(offDelay*60*1000) )) )
		goto ERROR_CLEANUP;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- PWR_OFF %dmin%.3fs after DOWN_EVENT\n", min, sec);

	printf("=> Please, switch the key on within %ds\n", USERTIME/1000);

	TIMER_START
	if( (Wait4PwrOnOff( PWR_ON, offDelay*60*1000 + USERTIME )) )
		goto ERROR_CLEANUP;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- PWR_ON %dmin%.3fs after DOWN_EVENT\n", min, sec);

	return 0;

ERROR_CLEANUP:
	printf("=> *** TEST ABORTED: please switch the key on\n");
	return 1;
}

/********************************* Wait4DownEvent **************************/
/** Wait for shutdown event
 *
 *  \param timeout	\IN  timeout [msec]
 *
 *  \return			0 success or error code
*/
static int Wait4DownEvent( u_int32 timeout )
{
	int32	val;
	u_int32	min;
	double	sec;

	while( TIMER_GET_MSEC < timeout ){

		TIMER_GET_MIN_SEC( min, sec )
		printf("- waiting for down event: %dmin%.3fs\r",
			min, sec );

		if ((M_getstat(G_Path, AD78_DOWN_EVT, &val)) < 0) {
			printf("                                                        \r");
			PrintMdisError("getstat AD78_DOWN_EVT");
			return 1;
		}

		if( val ){
			printf("                                                        \r");
			return 0;
		}

		UOS_Delay( POLL_TIME );
	}

	TIMER_GET_MIN_SEC( min, sec )
	printf("*** timeout - down event missed after %dmin%.3fs         \n",
		min, sec);

	return 1;
}

/********************************* Wait4PwrOnOff ***************************/
/** Wait for power on/off
*
*  \param state		\IN  state to wait for (PWR_ON or PWR_OFF)
*  \param timeout	\IN  timeout [msec]
*
*  \return		0 success or error code
*/
static int Wait4PwrOnOff( u_int32 state, u_int32 timeout )
{
	int32	val;
	u_int32	min;
	double	sec;

	while( TIMER_GET_MSEC < timeout ){

		TIMER_GET_MIN_SEC( min, sec )
		printf("- waiting for PWR %s: %dmin%.3fs\r",
			((state==PWR_ON) ? "on" : "off"), min, sec );

		if ((M_getstat(G_Path, AD78_TEST3, &val)) < 0) {
			printf("                                                        \r");
			PrintMdisError("getstat AD78_TEST3");
			return 1;
		}

		if( (state==PWR_ON) == (val & AD78C_TEST3_PWR) ){
			printf("                                                        \r");
			return 0;
		}

		UOS_Delay( POLL_TIME );
	}

	TIMER_GET_MIN_SEC( min, sec )
	printf("*** timeout - PWR %s missed after %dmin%.3fs         \n",
		((state==PWR_ON) ? "on" : "off"), min, sec );

	return 1;
}

/********************************* WotDuringDowndelay **********************/
/** WOT during down delay
 *
 *  \param timeout	\IN  timeout [msec]
 *
 *  \return			0 success or error code
*/
static int WotDuringDowndelay( u_int32 timeout )
{
	int32	val;
	u_int32	min;
	double	sec;

	printf("- Note: wot during downDelay\n");

	UOS_Delay( FWARE_DELAY );

	/* firmware must be in SM_STATE_WAIT_SHUTDOWN_DELAY state */
	if ((M_getstat(G_Path, AD78_TEST4, &val)) < 0) {
		PrintMdisError("getstat AD78_TEST4");
		return 1;
	}

	if( val != SM_STATE_WAIT_SHUTDOWN_DELAY ){
		printf("*** firmware not in SM_STATE_WAIT_SHUTDOWN_DELAY state\n");
		return 1;
	}

	while( TIMER_GET_MSEC < timeout ){

		TIMER_GET_MIN_SEC( min, sec )
		printf("- waiting for wot during down delay: %dmin%.3fs\r",
			min, sec );

		if ((M_getstat(G_Path, AD78_TEST4, &val)) < 0) {
			printf("                                                        \r");
			PrintMdisError("getstat AD78_TEST4");
			return 1;
		}

		if( val==SM_STATE_ON ){
			printf("                                                        \r");
			return 0;
		}

		UOS_Delay( POLL_TIME );
	}

	TIMER_GET_MIN_SEC( min, sec )
	printf("*** timeout - wot missed after %dmin%.3fs         \n",
		min, sec);

	return 1;
}

/********************************* TestOffAck ******************************/
/** Test off acknowledge
*
*  \return			0 success or error code
*/
static int TestOffAck( void )
{
	u_int32	min;
	double	sec;

	printf("=== test OFFACK ===\n");

	/* set shutdown delay */
	printf("- set shutdown delay: none\n");
	if( (M_setstat(G_Path, AD78_DOWN_DELAY, AD78C_DOWN_DELAY_MINMODE)) < 0) {
		PrintMdisError("setstat AD78_DOWN_DELAY");
		return 1;
	}

	/* set off delay */
	printf("- set off delay: maximum\n");
	if( (M_setstat(G_Path, AD78_OFF_DELAY, AD78C_OFF_DELAY_MAXMODE)) < 0) {
		PrintMdisError("setstat AD78_OFF_DELAY");
		return 1;
	}

	/* initiate a software shutdown */
	printf("- send SWOFF\n");
	if( (M_setstat(G_Path, AD78_SWOFF, 0)) < 0) {
		PrintMdisError("setstat AD78_SWOFF");
		return 1;
	}

	TIMER_START
	if( (Wait4DownEvent( DELAYTIME )) )
		return 1;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- DOWN_EVENT %dmin%.3fs after SWOFF\n", min, sec);

	/* send OFFACK */
	printf("- send OFFACK\n");
	if( (M_setstat(G_Path, AD78_OFFACK, 0)) < 0) {
		PrintMdisError("setstat AD78_ONACK");
		return 1;
	}

	TIMER_START
	if( (Wait4PwrOnOff( PWR_OFF, DELAYTIME )) )
		return 1;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- PWR_OFF %dmin%.3fs after OFFACK\n", min, sec);

	/* immediate SWON (for test only!) */
	printf("- immediate SWON (for test only!)\n");
	if( (M_setstat(G_Path, AD78_TEST2, AD78C_TEST2_SWON)) < 0) {
			PrintMdisError("setstat AD78_TEST2");
		return 1;
	}

	TIMER_START
	if( (Wait4PwrOnOff( PWR_ON, TOUT_MAX( AD78C_WAIT_OFF ) )) )
		return 1;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- PWR_ON %dmin%.3fs after SWON\n", min, sec);

	return 0;
}

/********************************* TestOnAck *******************************/
/** Test on acknowledge
*
*  \param testLevel		\IN  test level
*
*  \return			0 success or error code
*/
static int TestOnAck( u_int32 testLevel )
{
	u_int8	mode;
	u_int32	min;
	double	sec;
	u_int32	rst, errOff, ackErr, wdogErr;
	u_int32	expectedOnackResets=0;
	u_int32	timeout, onAckTout, maxMode;

	printf("=== test ONACK ===\n");

	/* set off delay */
	printf("- set off delay: none\n");
	if( (M_setstat(G_Path, AD78_OFF_DELAY, AD78C_OFF_DELAY_MINMODE)) < 0) {
		PrintMdisError("setstat AD78_OFF_DELAY");
		return 1;
	}

	/*-------------------------------+
	|  ONACK before timeout          |
	+-------------------------------*/
	printf("\n--- test ONACK before timeout ---\n");

	onAckTout = 4;
	printf("- set onAckTout=%ds\n", onAckTout);

	AD78C_ONACK_TOUT_SEC2MODE( onAckTout, mode );
	if( (M_setstat(G_Path, AD78_ONACK_TOUT, mode)) < 0) {
		PrintMdisError("setstat AD78_ONACK_TOUT");
		return 1;
	}

	/* SWOFF, SWON */
	if( (Restart()) )
		return 1;

	/* send ONACK */
	printf("- send ONACK\n");
	if( (M_setstat(G_Path, AD78_ONACK, 0)) < 0) {
		PrintMdisError("setstat AD78_ONACK");
		return 1;
	}

	/* check state */
	if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) )
		return 1;

	if( rst || errOff || ackErr ){
		printf("*** unexpected reset=%d/err-off=%d/ackErr=%d\n",
			rst, errOff, ackErr);
		return 1;
	}

	/*-------------------------------+
	|  check ONACK timeout           |
	+-------------------------------*/
	printf("\n--- check ONACK timeout ---\n");

	if( testLevel>1 )
		maxMode = AD78C_ONACK_TOUT_MAXMODE;
	else
		maxMode = 4;

	for( mode=1; mode<=maxMode; mode++ ){

		printf("\n- set onAckTout=%ds\n", AD78C_ONACK_TOUT_SEC( mode ));
		if( (M_setstat(G_Path, AD78_ONACK_TOUT, mode)) < 0) {
			PrintMdisError("setstat AD78_ONACK_TOUT");
			return 1;
		}

		/* SWOFF, SWON */
		if( (Restart()) )
			return 1;

		TIMER_START
		timeout = (AD78C_ONACK_TOUT_SEC( mode ) * 1000);

		/* state: after PWR_ON or RESET */
		while( 1 ){
			TIMER_GET_MIN_SEC( min, sec )
			printf("- waiting for ONACK error: %dmin%.3fs\r",
				min, sec );

			/* check state */
			if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) )
				return 1;

			/* time-to-short? */
			if( (rst || errOff) && ( TIMER_GET_MSEC < TOUT_MIN(timeout) )){
				TIMER_GET_MIN_SEC( min, sec )
					printf("*** WARNING: time-to-short - reset/error-off after %dmin%.3fs\n",
					min, sec);
			}

			/* reset? */
			if( rst ){
				expectedOnackResets++;

				TIMER_GET_MIN_SEC( min, sec )
				printf("- reset #%d %dmin%.3fs after PWR_ON/RESET           \n",
					expectedOnackResets, min, sec );

				/* check error count */
				if( expectedOnackResets != ackErr ){
					printf("*** ackErr=%d but %d expected                 \n",
						ackErr, expectedOnackResets);
					return 1;
				}
				if( ackErr > AD78C_ON_ACK_MAX_ERR ){
					printf("*** ackErr=%d but max-err=%d                  \n",
						ackErr, AD78C_ON_ACK_MAX_ERR);
					return 1;
				}

				TIMER_START
			}

			/* err-off? */
			if( errOff ){
				expectedOnackResets=0;

				TIMER_GET_MIN_SEC( min, sec )
				printf("- error-off %dmin%.3fs after PWR_ON                 \n",
					min, sec );

				/* check error count */
				if( expectedOnackResets != ackErr ){
					printf("*** ackErr=%d but %d expected\n", ackErr, expectedOnackResets);
					return 1;
				}
				break;
			}

			/* time-to-long? */
			if( TIMER_GET_MSEC > TOUT_MAX(timeout) ){
				TIMER_GET_MIN_SEC( min, sec )
				printf("*** time-to-long - reset/error-off missed after %dmin%.3fs\n",
					min, sec);
				return 1;
			}

			UOS_Delay( POLL_TIME );
		} /* while */
	}

	/*-------------------------------+
	|  ONACK after reset             |
	+-------------------------------*/
	printf("\n--- test ONACK after reset ---\n");

	onAckTout = 4;
	printf("- set onAckTout=%ds\n", onAckTout);

	AD78C_ONACK_TOUT_SEC2MODE( onAckTout, mode );
	if( (M_setstat(G_Path, AD78_ONACK_TOUT, mode)) < 0) {
		PrintMdisError("setstat AD78_ONACK_TOUT");
		return 1;
	}

	/* SWOFF, SWON */
	if( (Restart()) )
		return 1;

	TIMER_START
	timeout = onAckTout * 1000;

	while( 1 ){
		TIMER_GET_MIN_SEC( min, sec )
		printf("- waiting for ONACK error: %dmin%.3fs\r",
			min, sec );

		/* check state */
		if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) )
			return 1;

		/* reset? */
		if( rst ){
			expectedOnackResets++;

			TIMER_GET_MIN_SEC( min, sec )
			printf("- reset #%d %dmin%.3fs after PWR_ON/RESET         \n",
				expectedOnackResets, min, sec );

			/* check error count */
			if( expectedOnackResets != ackErr ){
				printf("*** ackErr=%d but %d expected                 \n",
					ackErr, expectedOnackResets);
				return 1;
			}
			if( ackErr > AD78C_ON_ACK_MAX_ERR ){
				printf("*** ackErr=%d but max-err=%d                  \n",
					ackErr, AD78C_ON_ACK_MAX_ERR);
				return 1;
			}

			TIMER_START
		}

		if( ackErr == AD78C_ON_ACK_MAX_ERR-1 ){

			/* send ONACK */
			printf("- send ONACK                                      \n");
			if( (M_setstat(G_Path, AD78_ONACK, 0)) < 0) {
				PrintMdisError("setstat AD78_ONACK");
				return 1;
			}

			printf("- waiting %dms before state check\n", timeout*2 );
			UOS_Delay( timeout*2 );

			/* check state */
			if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) )
				return 1;

			if( rst || errOff || (ackErr != AD78C_ON_ACK_MAX_ERR-1) ){
				printf("*** unexpected reset=%d/err-off=%d/ackErr=%d\n",
					rst, errOff, ackErr);
				return 1;
			}

			return 0;
		}

		/* time-to-long? */
		if( TIMER_GET_MSEC > TOUT_MAX(timeout) ){
			TIMER_GET_MIN_SEC( min, sec )
			printf("*** time-to-long - reset/error-off missed after %dmin%.3fs\n",
				min, sec);
			return 1;
		}

		UOS_Delay( POLL_TIME );
	} /* while */

	return 0;
}

/********************************* TestWdog ********************************/
/** Test watchdog
*
*  \param testLevel		\IN  test level
*
*  \return			0 success or error code
*/
static int TestWdog( u_int32 testLevel )
{
	u_int32	min;
	double	sec;
	u_int32	rst, errOff, ackErr, wdogErr, wdogTime, n;
	u_int32	expectedWdogResets=0;

	printf("=== test watchdog ===\n");

	/* set ONACK timeout */
	printf("- set onAckTout=none\n");
	if( (M_setstat(G_Path, AD78_ONACK_TOUT, AD78C_ONACK_TOUT_NONE)) < 0) {
		PrintMdisError("setstat AD78_ONACK_TOUT");
		return 1;
	}

	/*-------------------------------+
	|  trigger before timeout        |
	+-------------------------------*/
	printf("\n--- trigger before wdog timeout ---\n");

	wdogTime = 2000;	/* 2s */

	/* set wdog time */
	printf("- set wdog time=%dms\n", wdogTime);
	if( (M_setstat(G_Path, WDOG_TIME, wdogTime)) < 0) {
		PrintMdisError("setstat WDOG_TIME");
		return 1;
	}

	/* enable watchdog */
	if( WdogOnOff( 1 ) )
		return 1;

	TIMER_START

	for( n=0; n<5; n++ ){

		/* trigger watchdog */
		TIMER_GET_MIN_SEC( min, sec )
		printf("- trigger watchdog %dmin%.3fs after start/trigger\n",
			min, sec );
		if( (M_setstat(G_Path, WDOG_TRIG, 0)) < 0) {
			PrintMdisError("setstat WDOG_TRIG");
			WdogOnOff( 0 );
			return 1;
		}

		/* check state */
		if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) ){
			WdogOnOff( 0 );
			return 1;
		}
		if( rst || errOff || wdogErr ){
			printf("*** unexpected reset=%d/err-off=%d/wdogErr=%d\n",
				rst, errOff, wdogErr);
			WdogOnOff( 0 );
			return 1;
		}

		printf("- waiting %dms before trigger\n", wdogTime/2 );
		TIMER_START
		UOS_Delay( wdogTime/2 );
	}

	/* disable watchdog */
	if( WdogOnOff( 0 ) )
		return 1;

	printf("- waiting %dms before state check\n", wdogTime*2 );
	UOS_Delay( wdogTime*2 );

	/* check state */
	if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) )
		return 1;
	if( rst || errOff || wdogErr ){
		printf("*** unexpected reset=%d/err-off=%d/wdogErr=%d\n",
			rst, errOff, wdogErr);
		return 1;
	}

	/*-------------------------------+
	|  check wdog timeout            |
	+-------------------------------*/
	printf("\n--- check wdog timeout ---\n");

	for( wdogTime=500; (int32)wdogTime <= ((testLevel>1) ? 25500 : 10000);
		 wdogTime+= ((testLevel>1) ?  3500 :  2500) ){

		/* set wdog time */
		printf("\n- set wdog time=%dms\n", wdogTime);
		if( (M_setstat(G_Path, WDOG_TIME, wdogTime)) < 0) {
			PrintMdisError("setstat WDOG_TIME");
			return 1;
		}

		/* enable watchdog */
		if( WdogOnOff( 1 ) )
			return 1;

		TIMER_START

		/* state: after PWR_ON or RESET */
		while( 1 ){
			TIMER_GET_MIN_SEC( min, sec )
			printf("- waiting for wdog error: %dmin%.3fs\r",
				min, sec );

			/* check state */
			if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) ){
				WdogOnOff( 0 );
				return 1;
			}

			/* time-to-short? */
			if( (rst || errOff) && ( TIMER_GET_MSEC < TOUT_MIN(wdogTime) )){
				TIMER_GET_MIN_SEC( min, sec )
					printf("*** WARNING: time-to-short - reset/error-off after %dmin%.3fs\n",
					min, sec);
			}

			/* reset? */
			if( rst ){
				expectedWdogResets++;

				TIMER_GET_MIN_SEC( min, sec )
				printf("- reset #%d %dmin%.3fs after PWR_ON/RESET           \n",
					expectedWdogResets, min, sec );

				/* check error count */
				if( expectedWdogResets != wdogErr ){
					printf("*** wdogErr=%d but %d expected                 \n",
						wdogErr, expectedWdogResets);
					WdogOnOff( 0 );
					return 1;
				}
				if( wdogErr > AD78C_WDOG_MAX_ERR ){
					printf("*** wdogErr=%d but max-err=%d                  \n",
						wdogErr, AD78C_WDOG_MAX_ERR);
					WdogOnOff( 0 );
					return 1;
				}

				TIMER_START
			}

			/* err-off? */
			if( errOff ){
				expectedWdogResets=0;

				TIMER_GET_MIN_SEC( min, sec )
				printf("- error-off %dmin%.3fs after PWR_ON/RESET           \n",
					min, sec );

				/* check error count */
				if( expectedWdogResets != wdogErr ){
					printf("*** wdogErr=%d but %d expected\n", wdogErr, expectedWdogResets);
					WdogOnOff( 0 );
					return 1;
				}
				break;
			}

			/* time-to-long? */
			if( TIMER_GET_MSEC > TOUT_MAX(wdogTime) ){
				TIMER_GET_MIN_SEC( min, sec )
				printf("*** time-to-long - reset/error-off missed after %dmin%.3fs\n",
					min, sec);
				WdogOnOff( 0 );
				return 1;
			}

			UOS_Delay( POLL_TIME );
		} /* while */

		/* disable watchdog */
		if( WdogOnOff( 0 ) )
			return 1;
	}

	/*-------------------------------+
	|  trigger after reset           |
	+-------------------------------*/
	printf("\n--- test trigger after reset ---\n");

	wdogTime = 6000;	/* 6s */

	/* set wdog time */
	printf("- set wdog time=%dms\n", wdogTime);
	if( (M_setstat(G_Path, WDOG_TIME, wdogTime)) < 0) {
		PrintMdisError("setstat WDOG_TIME");
		return 1;
	}

	/* enable watchdog */
	if( WdogOnOff( 1 ) )
		return 1;

	TIMER_START

	while( 1 ){
		TIMER_GET_MIN_SEC( min, sec )
		printf("- waiting for wdog error: %dmin%.3fs\r",
			min, sec );

		/* check state */
		if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) ){
			WdogOnOff( 0 );
			return 1;
		}

		/* reset? */
		if( rst ){
			expectedWdogResets++;

			TIMER_GET_MIN_SEC( min, sec )
			printf("- reset #%d %dmin%.3fs after PWR_ON/RESET           \n",
				expectedWdogResets, min, sec );

			/* check error count */
			if( expectedWdogResets != wdogErr ){
				printf("*** wdogErr=%d but %d expected                 \n",
					wdogErr, expectedWdogResets);
				WdogOnOff( 0 );
				return 1;
			}
			if( wdogErr > AD78C_WDOG_MAX_ERR ){
				printf("*** wdogErr=%d but max-err=%d                  \n",
					wdogErr, AD78C_WDOG_MAX_ERR);
				WdogOnOff( 0 );
				return 1;
			}

			TIMER_START
		}

		if( wdogErr == AD78C_WDOG_MAX_ERR-1 ){

			printf("- waiting %dms before trigger          \n", wdogTime*2/3 );
			UOS_Delay( wdogTime*2/3 );

			/* trigger watchdog */
			printf("- trigger watchdog\n");
			if( (M_setstat(G_Path, WDOG_TRIG, 0)) < 0) {
				PrintMdisError("setstat WDOG_TRIG");
				WdogOnOff( 0 );
				return 1;
			}

			printf("- waiting %dms before wdog disable\n", wdogTime*2/3 );
			UOS_Delay( wdogTime*2/3 );

			/* disable watchdog */
			if( WdogOnOff( 0 ) )
				return 1;

			printf("- waiting %dms before state check\n", wdogTime*2/3 );
			UOS_Delay( wdogTime*2/3 );

			/* check state */
			if( (CheckRstErroffErrcount( &rst, &errOff, &ackErr, &wdogErr )) )
				return 1;

			if( rst || errOff || (wdogErr != AD78C_WDOG_MAX_ERR-1) ){
				printf("*** unexpected reset=%d/err-off=%d/wdogErr=%d\n",
					rst, errOff, wdogErr);
				return 1;
			}

			return 0;
		}

		/* time-to-long? */
		if( TIMER_GET_MSEC > TOUT_MAX(wdogTime) ){
			TIMER_GET_MIN_SEC( min, sec )
			printf("*** time-to-long - reset/error-off missed after %dmin%.3fs\n",
				min, sec);
			return 1;
		}

		UOS_Delay( POLL_TIME );
	} /* while */

	return 0;
}

/********************************* CheckRstErroffErrcount ******************/
/** Check reset, error-off, ONACK error count, WDOG error count
*
*  \param rst		\OUT  <>1: reset was set
*  \param errOff	\OUT  <>1: error-off was set
*  \param ackErr	\OUT  ONACK error count
*  \param wdogErr	\OUT  WDOG error count
*
*  \return		0 success or error code
*/
static int CheckRstErroffErrcount(
	u_int32		*rst,
	u_int32		*errOff,
	u_int32		*ackErr,
	u_int32		*wdogErr )
{
	int32	val;

	/* number of missing ON Acknowledges - ckeck before reset flag */
	if ((M_getstat(G_Path, AD78_ONACK_ERR, ackErr)) < 0) {
		PrintMdisError("getstat AD78_ONACK_ERR");
		return 1;
	}

	/* number of missing watchdog triggers - ckeck before reset flag */
	if ((M_getstat(G_Path, AD78_WDOG_ERR, wdogErr)) < 0) {
		PrintMdisError("getstat AD78_WDOG_ERR");
		return 1;
	}

	/* check error OFF state */
	if ((M_getstat(G_Path, AD78_TEST4, &val)) < 0) {
		PrintMdisError("getstat AD78_TEST4");
		return 1;
	}
	*errOff = (val == SM_STATE_ERROR_OFF) ? 1 : 0;
	if( *errOff ){
		u_int32	oldTimeStart;
		/* check power */
		if ((M_getstat(G_Path, AD78_TEST3, &val)) < 0) {
			PrintMdisError("getstat AD78_TEST3");
			return 1;
		}
		if( val & AD78C_TEST3_PWR ){
			printf("*** error OFF state <==> power is on\n");
			return 1;
		}

		/* toggle test mode to restart */
		if( (M_setstat(G_Path, AD78_TEST1, AD78C_TEST1_NOTEST)) < 0) {
				PrintMdisError("setstat AD78_TEST1");
			return 1;
		}
		if( (M_setstat(G_Path, AD78_TEST1, AD78C_TEST1_TMODE1)) < 0) {
				PrintMdisError("setstat AD78_TEST1");
			return 1;
		}
		/* save and restore old TimeStart for caller */
		oldTimeStart = G_TimeStart;
		TIMER_START
		if( (Wait4PwrOnOff( PWR_ON, DELAYTIME )) )
			return 1;
		G_TimeStart = oldTimeStart;

		/* number of missing ON Acknowledges - update after restart */
		if ((M_getstat(G_Path, AD78_ONACK_ERR, ackErr)) < 0) {
			PrintMdisError("getstat AD78_ONACK_ERR");
			return 1;
		}

		/* number of missing watchdog triggers - update after restart */
		if ((M_getstat(G_Path, AD78_WDOG_ERR, wdogErr)) < 0) {
			PrintMdisError("getstat AD78_WDOG_ERR");
			return 1;
		}
	}

	/* check reset */
	if ((M_getstat(G_Path, AD78_TEST2, &val)) < 0) {
		PrintMdisError("getstat AD78_TEST2");
		return 1;
	}

	*rst = (val & AD78C_TEST2_RST) ? 1 : 0;
	if( *rst ){
		/* clear reset flag */
		if( (M_setstat(G_Path, AD78_TEST2, 0x00)) < 0) {
				PrintMdisError("setstat AD78_TEST2");
			return 1;
		}
		/* number of missing ON Acknowledges - update after reset */
		if ((M_getstat(G_Path, AD78_ONACK_ERR, ackErr)) < 0) {
			PrintMdisError("getstat AD78_ONACK_ERR");
			return 1;
		}

		/* number of missing watchdog triggers - update after reset */
		if ((M_getstat(G_Path, AD78_WDOG_ERR, wdogErr)) < 0) {
			PrintMdisError("getstat AD78_WDOG_ERR");
			return 1;
		}
	}

	return 0;
}

/********************************* Restart *********************************/
/** Restart (SWOFF, SWON)
*
*  \return			0 success or error code
*/
static int Restart( void )
{
	u_int32	min;
	double	sec;

	/* initiate a software shutdown */
	printf("- send SWOFF\n");
	if( (M_setstat(G_Path, AD78_SWOFF, 0)) < 0) {
		PrintMdisError("setstat AD78_SWOFF");
		return 1;
	}

	TIMER_START
	if( (Wait4DownEvent( DELAYTIME )) )
		return 1;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- PWR_OFF %dmin%.3fs after SWOFF\n", min, sec);

	/* immediate SWON (for test only!) */
	printf("- immediate SWON (for test only!)\n");
	if( (M_setstat(G_Path, AD78_TEST2, AD78C_TEST2_SWON)) < 0) {
			PrintMdisError("setstat AD78_TEST2");
		return 1;
	}

	TIMER_START
	if( (Wait4PwrOnOff( PWR_ON, TOUT_MAX( AD78C_WAIT_OFF ) )) )
		return 1;

	TIMER_GET_MIN_SEC( min, sec );
	printf("- PWR_ON %dmin%.3fs after SWON\n", min, sec);

	return 0;
}

/********************************* WdogOnOff *******************************/
/** Enable/disable watchdog
*
*  \param enable	\IN  1=enable, 0=disable
*
*  \return			0 success or error code
*/
static int WdogOnOff( u_int32 enable )
{
	if( enable ){
		/* enable watchdog */
		printf("- enable watchdog\n");
		if( (M_setstat(G_Path, WDOG_START, 0)) < 0) {
			PrintMdisError("setstat WDOG_START");
			return 1;
		}
	}
	else {
		/* disable watchdog */
		printf("- disable watchdog\n");
		if( (M_setstat(G_Path, WDOG_STOP, 0)) < 0) {
			PrintMdisError("setstat WDOG_STOP");
			return 1;
		}
	}

	return 0;
}
