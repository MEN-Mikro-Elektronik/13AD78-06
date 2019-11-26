/****************************************************************************
 ************                                                    ************
 ************                    AD78_CTRL                        ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file ad78_ctrl.c
 *       \author dieter.pfeuffer@men.de
 *
 *        \brief Tool to control AD78 PIC
 *
 *     Required: libraries: mdis_api, usr_oss, usr_utl
 *     \switches (none)
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright 2006-2019, MEN Mikro Elektronik GmbH
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

static const char IdentString[]=MENT_XSTR(MAK_REVISION);

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/
static MDIS_PATH  G_Path;
static u_int32	  G_SigCountSdEvt;	/* count of shutdown event signals */
static u_int32	  G_SigCountOthers;	/* count of other signals */

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
#define NONE		-1
#define	IO_MAX_NBR	8

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void usage(void);
static void PrintMdisError(char *info);
static void PrintUosError(char *info);
static int ShowIo(void);
static void __MAPILIB SigHandler(u_int32 sigCode);

/********************************* usage ***********************************/
/**  Print program usage
 */
static void usage(void)
{
	printf("Usage: ad78_ctrl [<opts>] <device> [<opts>]\n");
	printf("Function: Control AD78 PIC\n");
	printf("Options:\n");
	printf("    device       device name                               \n");
	printf("    -i           get current settings                      \n");
	printf(" --- power-up settings ---\n");
	printf("    -w=<min>     set wake on time [min] (0=none)           \n");
	printf("    -n=<mode>    set ON acknowledge timeout                \n");
	printf("                   mode       : 0   | 1 | 2 | 3 |...| 11   \n");
	printf("                   time [sec] : none| 1 | 2 | 4 |...| 1024 \n");
	printf("    -a           send ON acknowledge             \n");
	printf(" --- power-off settings ---\n");
	printf("    -d=<mode>    set shutdown delay:                       \n");
	printf("                   mode       : 0 | 1 | 2 | 3 |...| 7      \n");
	printf("                   time [min] : 0 | 1 | 2 | 4 |...| 64     \n");
	printf("    -f=<mode>    set off delay:                            \n");
	printf("                   mode       : 0 | 1 | 2 | 3 | 4 | 5      \n");
	printf("                   time [min] : 0 | 1 | 2 | 4 | 8 | 16     \n");
	printf("    -s           initiate a software shutdown              \n");
	printf("    -e=<ptime>   handle shutdown event signal              \n");
	printf("                 - drv polls the event flag all ptime [ms] \n");
	printf("                 - install signal for shutdown event       \n");
	printf("                 - send OFF acknowledge if signal received \n");
	printf(" --- binary i/o, temp settings ---\n");
	printf("    -H=<port>    set specified binary output (0..7) high   \n");
	printf("    -L=<port>    set specified binary output (0..7) low    \n");
	printf("    -T=<limit>   set temperature high limit [degree celsius]\n");
	printf("    -I           get binary i/o, temp and volt (in a loop) \n");
	printf("\n");
	printf("Copyright 2006-2019, MEN Mikro Elektronik GmbH\n%s\n", IdentString);
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
	int32	info, ioInfo;
	int32	wot, onAckTime, onAck;
	int32	sdDelay, offDelay, swSd, sdEvtPollTime;
	int32	binGet, binHigh, binLow, tempHigh;
	int32	n, val;

	/*--------------------+
    |  check arguments    |
    +--------------------*/
	if( (errstr = UTL_ILLIOPT("iIw=n=ad=f=se=H=L=T=?", buf)) ){	/* check args */
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

	info = (UTL_TSTOPT("i") ? 1 : NONE);
	ioInfo = (UTL_TSTOPT("I") ? 1 : NONE);

	wot  = ((str = UTL_TSTOPT("w=")) ? atoi(str) : NONE);
	onAckTime = ((str = UTL_TSTOPT("n=")) ? atoi(str) : NONE);
	onAck = (UTL_TSTOPT("a") ? 1 : NONE);

	sdDelay = ((str = UTL_TSTOPT("d=")) ? atoi(str) : NONE);
	offDelay = ((str = UTL_TSTOPT("f=")) ? atoi(str) : NONE);
	swSd = (UTL_TSTOPT("s") ? 1 : NONE);
	sdEvtPollTime = ((str = UTL_TSTOPT("e=")) ? atoi(str) : NONE);

	binGet = (UTL_TSTOPT("b") ? 1 : NONE);
	binHigh = ((str = UTL_TSTOPT("H=")) ? atoi(str) : NONE);
	binLow = ((str = UTL_TSTOPT("L=")) ? atoi(str) : NONE);
	tempHigh = ((str = UTL_TSTOPT("T=")) ? atoi(str) : NONE);

	(void)binGet;   /* -Wunused-but-set-variable */

	/*--------------------+
    |  open path          |
    +--------------------*/
	if( (G_Path = M_open(device)) < 0) {
		PrintMdisError("open");
		return(1);
	}

	/*--------------------+
    |  current settings   |
    +--------------------*/
	if( info != NONE ){
		printf("current settings:\n");

		/* Wake ON Time */
		if ((M_getstat(G_Path, AD78_WOT, &val)) < 0) {
			PrintMdisError("getstat AD78_WOT");
			goto abort;
		}
		printf("- wake on time                    : %dmin\n", val);

		/* ON Acknowledge Timeout */
		if ((M_getstat(G_Path, AD78_ONACK_TOUT, &val)) < 0) {
			PrintMdisError("getstat AD78_ONACK_TOUT");
			goto abort;
		}
		printf("- ON acknowledge timeout          : %dsec\n",
			AD78C_ONACK_TOUT_SEC( val ));

		/* Number of missing ON Acknowledges */
		if ((M_getstat(G_Path, AD78_ONACK_ERR, &val)) < 0) {
			PrintMdisError("getstat AD78_ONACK_ERR");
			goto abort;
		}
		printf("- missing ON acknowledges         : %d\n", val);

		/* Number of missing watchdog triggers */
		if ((M_getstat(G_Path, AD78_WDOG_ERR, &val)) < 0) {
			PrintMdisError("getstat AD78_WDOG_ERR");
			goto abort;
		}
		printf("- missing watchdog triggers       : %d\n", val);

		/* Shutdown delay */
		if ((M_getstat(G_Path, AD78_DOWN_DELAY, &val)) < 0) {
			PrintMdisError("getstat AD78_DOWN_DELAY");
			goto abort;
		}
		printf("- shutdown delay                  : %dmin\n",
			AD78C_DOWN_DELAY_MIN( val ));

		/* Off delay */
		if ((M_getstat(G_Path, AD78_OFF_DELAY, &val)) < 0) {
			PrintMdisError("getstat AD78_OFF_DELAY");
			goto abort;
		}
		printf("- off delay                       : %dmin\n",
			AD78C_OFF_DELAY_MIN( val ));

		/* Shutdown event flag*/
		if ((M_getstat(G_Path, AD78_DOWN_EVT, &val)) < 0) {
			PrintMdisError("getstat AD78_DOWN_EVT");
			goto abort;
		}
		printf("- shutdown event flag             : %d\n", val);

	} /* info */

	/*--------------------+
    |  power-up           |
    +--------------------*/
	/* set wake on time */
	if( wot != NONE ){
		printf("- set wake on time to: %dmin\n", wot);
		if( (M_setstat(G_Path, AD78_WOT, wot)) < 0) {
			PrintMdisError("setstat AD78_WOT");
			goto abort;
		}
	}

	/* set ON acknowledge timeout */
	if( onAckTime != NONE ){
		sprintf( buf, "%dsec", AD78C_ONACK_TOUT_SEC(onAckTime) );
		printf("- set ON acknowledge timeout to: mode=%d (%s)\n",
			onAckTime, onAckTime ? buf : "none");
		if( (M_setstat(G_Path, AD78_ONACK_TOUT, onAckTime)) < 0) {
			PrintMdisError("setstat AD78_ONACK_TOUT");
			goto abort;
		}
	}

	/* send ON acknowledge */
	if( onAck != NONE ){
		printf("- send ON acknowledge\n");
		if( (M_setstat(G_Path, AD78_ONACK, 0)) < 0) {
			PrintMdisError("setstat AD78_ONACK");
			goto abort;
		}
	}

	/*--------------------+
    |  power-off          |
    +--------------------*/
	/* set shutdown delay */
	if( sdDelay != NONE ){
		printf("- set shutdown delay to: mode=%d (%dmin)\n",
			sdDelay, AD78C_DOWN_DELAY_MIN(sdDelay));
		if( (M_setstat(G_Path, AD78_DOWN_DELAY, sdDelay)) < 0) {
			PrintMdisError("setstat AD78_DOWN_DELAY");
			goto abort;
		}
	}

	/* set off delay */
	if( offDelay != NONE ){
		printf("- set off delay to: mode=%d (%dmin)\n",
			offDelay, AD78C_OFF_DELAY_MIN(offDelay));
		if( (M_setstat(G_Path, AD78_OFF_DELAY, offDelay)) < 0) {
			PrintMdisError("setstat AD78_OFF_DELAY");
			goto abort;
		}
	}

	/* initiate a software shutdown */
	if( swSd != NONE ){
		printf("- initiate a software shutdown\n");
		if( (M_setstat(G_Path, AD78_SWOFF, 0)) < 0) {
			PrintMdisError("setstat AD78_SWOFF");
			goto abort;
		}
	}

	/*--------------------+
    |  binary i/o, temp   |
    +--------------------*/
	/* set binary output high */
	if( binHigh != NONE ){
		printf("- set binary output %d high\n",
			binHigh);
		if( (M_setstat(G_Path, AD78_OUT_HIGH, (INT32_OR_64)(0x01 << binHigh))) < 0 ) {
			PrintMdisError("setstat AD78_OUT_HIGH");
			goto abort;
		}
	}

	/* set binary output low */
	if( binLow != NONE ){
		printf("- set binary output %d low\n",
			binLow);
		if( (M_setstat(G_Path, AD78_OUT_LOW, (INT32_OR_64)(0x01 << binLow))) < 0 ) {
			PrintMdisError("setstat AD78_OUT_LOW");
			goto abort;
		}
	}

	/* set temperature high limit */
	if( tempHigh != NONE ){
		printf("- set temperature high limit: %d\n", tempHigh);
		if( (M_setstat(G_Path, AD78_TEMP_HIGH, tempHigh)) < 0 ) {
			PrintMdisError("setstat AD78_TEMP_HIGH");
			goto abort;
		}
	}

	/*-------------------------------+
    |  handle shutdown event signal  |
    +-------------------------------*/
	if( sdEvtPollTime != NONE ){

		M_SG_BLOCK			blk;
		AD78_BLK_DOWN_SIG	downSig;

		printf("- handle shutdown event signal:\n");
		printf("  The driver polls all %dms for an shutdown event\n", sdEvtPollTime);
		printf("  and sends an signal to the app. if the event-flag is set.\n");
		printf("  The app. sends the OFF acknowledge if the signal was received\n");

		/* clear signal counters */
		G_SigCountSdEvt = 0;
		G_SigCountOthers = 0;

		blk.size = sizeof(AD78_BLK_DOWN_SIG);
		blk.data = (void*)&downSig;

		downSig.msec = sdEvtPollTime;
		downSig.signal = UOS_SIG_USR1;

		/* install signal handler */
		if( UOS_SigInit(SigHandler) ){
			PrintUosError("SigInit");
			return(1);
		}

		/* install signal */
		if( UOS_SigInstall(UOS_SIG_USR1) ){
			PrintUosError("SigInstall");
			goto abort;
		}

		/* install signal for shutdown event */
		if( (M_setstat(G_Path, AD78_BLK_DOWN_SIG_SET, (INT32_OR_64)&blk)) < 0 ) {
			PrintMdisError("setstat AD78_BLK_DOWN_SIG_SET");
			goto abort;
		}

		printf("--- press any key to abort ---\n");
	}

	/*--------------------------+
    | i/o, temp and volt states |
	|      AND / OR             |
	| wait event signal         |
    +--------------------------*/
	if( ( ioInfo != NONE ) ||
		(sdEvtPollTime != NONE) ){

		/* repeat until keypress */
		do {

			if( ioInfo != NONE ){
				if( (ShowIo()) )
					goto abort;
				printf("--- press any key to abort ---\n");
			}

			if( sdEvtPollTime != NONE ){
				if( G_SigCountSdEvt ){
					printf("--- OFF Acknowledge sent ---\n");
					printf("--- system will be powered-off after off delay time ---\n");
					goto abort;
				}
			}

			UOS_Delay(1000);
		} while( (UOS_KeyPressed() == -1) );
	}

	/*--------------------+
    |  cleanup            |
    +--------------------*/
	abort:

	if( sdEvtPollTime != NONE ){
		/* terminate signal handling */
		M_setstat(G_Path, AD78_DOWN_SIG_CLR, 0);
		UOS_SigExit();
		printf("\n");
		printf("Count of shutdown event signals : %d \n", G_SigCountSdEvt);
		printf("Count of other signals          : %d \n", G_SigCountOthers);
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

/********************************* PrintUosError ***************************/
/** Print User OSS error message
 *
 *  \param info       \IN  info string
*/
static void PrintUosError(char *info)
{
	printf("*** can't %s: %s\n", info, UOS_ErrString(UOS_ErrnoGet()));
}

/********************************* ShowIo **********************************/
/** show i/o, temp and volt
 *
 *  \return           \c 0 success or error code
*/
static int ShowIo( void )
{
	int32	n, val;

	printf("binary i/o, temp and volt states:\n");

	printf("- binary inputs:\n");
	printf("  port  : 7 6 5 4 3 2 1 0\n");

	/* binary inputs mask */
	if ((M_getstat(G_Path, AD78_IN_MASK, &val)) < 0) {
		PrintMdisError("getstat AD78_IN_MASK");
		return(1);
	}
	printf("  mask  : ");
	for( n=IO_MAX_NBR-1; n>=0; n-- )
		printf("%d ", 1 & (val>>n) );
	printf("\n");

	/* binary inputs state */
	if ((M_getstat(G_Path, AD78_IN, &val)) < 0) {
		PrintMdisError("getstat AD78_IN");
		return(1);
	}
	printf("  state : ");
	for( n=IO_MAX_NBR-1; n>=0; n-- )
		printf("%d ", 1 & (val>>n) );
	printf("\n");

	printf("- binary outputs:\n");
	printf("  port  : 7 6 5 4 3 2 1 0\n");

	/* binary outputs mask */
	if ((M_getstat(G_Path, AD78_OUT_MASK, &val)) < 0) {
		PrintMdisError("getstat AD78_OUT_MASK");
		return(1);
	}
	printf("  mask  : ");
	for( n=IO_MAX_NBR-1; n>=0; n-- )
		printf("%d ", 1 & (val>>n) );
	printf("\n");

	/* binary outputs state */
	if ((M_getstat(G_Path, AD78_OUT, &val)) < 0) {
		PrintMdisError("getstat AD78_OUT");
		return(1);
	}
	printf("  state : ");
	for( n=IO_MAX_NBR-1; n>=0; n-- )
		printf("%d ", 1 & (val>>n) );
	printf("\n");

	/* temp */
	if ((M_getstat(G_Path, AD78_TEMP, &val)) < 0) {
		PrintMdisError("getstat AD78_TEMP");
		return(1);
	}
	printf("- temperature           : %d degree celsius\n", val);

	/* temp high limit */
	if ((M_getstat(G_Path, AD78_TEMP_HIGH, &val)) < 0) {
		PrintMdisError("getstat AD78_TEMP_HIGH");
		return(1);
	}
	printf("- temperature high limit: %d degree celsius\n", val);

	/* voltage */
	if ((M_getstat(G_Path, AD78_VOLTAGE, &val)) < 0) {
		PrintMdisError("getstat AD78_VOLTAGE");
		return(1);
	}
	printf("- PSU input voltage     : %d mV\n", val);

	return(0);
}

/********************************* SigHandler *******************************/
/** Signal handler
 *
 *  \param sigCode       \IN  signal code received
*/
static void __MAPILIB SigHandler(u_int32 sigCode)
{
	if(  sigCode == UOS_SIG_USR1){
		/* send OFF Acknowledge */
		M_setstat(G_Path, AD78_OFFACK, 0);

		G_SigCountSdEvt++;
	}
	else
		G_SigCountOthers++;
}

