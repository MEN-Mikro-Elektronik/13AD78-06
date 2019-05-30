/****************************************************************************
 ************                                                    ************
 ************                   AD78_SIMP                        ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file ad78_simp.c
 *       \author dieter.pfeuffer@men.de
 *
 *        \brief Simple example program for the AD78 driver.
 *
 *               Control the AD78 PSU.
 *
 *     Required: libraries: mdis_api, usr_oss
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
#include <string.h>
#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/ad78_drv.h>
#include <MEN/ad78c.h>
#include <MEN/wdog.h>

static const char IdentString[]=MENT_XSTR(MAK_REVISION);

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
