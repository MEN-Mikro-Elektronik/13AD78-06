/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file pu05_ctrl.c
 *
 *      \author  thomas.schnuerer@men.de
 *        $Date: 2009/09/01 09:47:00 $
 *    $Revision: 1.4 $
 *
 *        \brief Tool to control PU05 additional functions which are not
 *               covered by the basic AD78 command set
 *
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: pu05_ctrl.c,v $
 * Revision 1.4  2009/09/01 09:47:00  MRoth
 * R: MDVE warnings
 * M: removed unused global variables
 *
 * Revision 1.3  2009/08/31 14:35:49  MRoth
 * R: Porting to MDIS5
 * M: added support for 64bit (MDIS_PATH)
 *
 * Revision 1.2  2009/04/23 17:35:28  ts
 * R: possibility for changing limits on all outputs was missing
 * M: added arguments for channel select and current/volt limit values
 *    and functions
 *
 * Revision 1.1  2009/04/06 13:19:54  ts
 * R: Initial Revision, first working version reading from PU5
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2009 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <MEN/men_typs.h>
#include <MEN/usr_oss.h>
#include <MEN/usr_utl.h>
#include <MEN/mdis_api.h>
#include <MEN/ad78_drv.h>
#include <MEN/ad78c.h>
#include <MEN/pu05c.h>

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/
static MDIS_PATH G_Path;
static char *G_rail[3]={" 3.3V ",	/* helper, maps channel to output rail */
						"   5V ",
						"  12V "};

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
#define NONE        -1
#define CHAN_MAX	3

/* more compact error handling */
#define CHK(expression,errst)       \
    if((expression)<0) {            \
        PrintMdisError(errst);      \
        goto abort;                 \
    }

#define CHK_CHAN(expression) 												\
	if( expression ) {  					\
		printf("*** to set a limit pass a valid output rail! (-r=0..2)\n"); \
			goto abort;														\
    }


/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void usage(void);
static void PrintMdisError(char *info);
static int ShowIo(void);


/********************************* usage ***********************************/
/**  Print program usage
 */
static void usage(void)
{
    printf("Usage: pu05_ctrl [<opts>] <device> [<opts>]\n");
    printf("Function: Control extra functions in PU05 which are not part\n");
    printf("          of AD78 tools.\n");
    printf("Options:\n");
    printf("  device   device name\n");
    printf(" -g        get current settings\n");
    printf(" --- limit settings ---\n");
    printf(" -r=<rail> power rail to apply setting to:   0=3,3V 1=5V 2=12V\n");
    printf(" -V=<volt> set high voltage limit of rail -r              [mV]\n");
    printf(" -v=<volt> set low voltage limit of rail -r               [mV]\n");
    printf(" -I=<volt> set high current limit of rail -r              [mA]\n");
    printf(" -i=<volt> set low current limit of rail -r               [mA]\n");
    printf(" -l        display currents/voltages/temperature in a loop \n");
    printf("\n");
    printf("(c) 2009 by MEN mikro elektronik GmbH\n\n");
}


/********************************* main ************************************/
/** show current settings
 *
 *  \return           success (0) or error (1)
 */
static int32 showInfo(void)
{
	int32 val = 0;
	printf("current settings:\n");

	/* Wake ON Time */
	CHK( (M_getstat(G_Path, AD78_WOT, &val)), "AD78_WOT");
	printf(" WOT (wake-on time)              : %dmin\n", val);

	/* ON Acknowledge Timeout */
	CHK( (M_getstat(G_Path, AD78_ONACK_TOUT, &val)), "AD78_ONACK_TOUT");
	printf(" ON acknowledge timeout          : %dsec\n",
		   AD78C_ONACK_TOUT_SEC( val ));

	/* Number of missing ON Acknowledges */
	CHK( (M_getstat(G_Path, AD78_ONACK_ERR, &val)), "AD78_ONACK_ERR");
	printf(" missing ON acknowledges         : %d\n", val);

	/* Number of missing watchdog triggers */
	CHK( (M_getstat(G_Path, AD78_WDOG_ERR, &val)), "AD78_WDOG_ERR");
	printf(" missing watchdog triggers       : %d\n", val);

	/* Shutdown delay */
	CHK( (M_getstat(G_Path, AD78_DOWN_DELAY, &val)), "AD78_DOWN_DELAY");
	printf(" shutdown delay                  : %dmin\n",
		   AD78C_DOWN_DELAY_MIN( val ));

	/* Off delay */
	CHK( (M_getstat(G_Path, AD78_OFF_DELAY, &val)), "AD78_OFF_DELAY");
	printf(" off delay                       : %dmin\n",
		   AD78C_OFF_DELAY_MIN(val));

	/* Shutdown event flag*/
	CHK( (M_getstat(G_Path, AD78_DOWN_EVT, &val)), "AD78_DOWN_EVT");
	printf("- shutdown event flag            : %d\n", val);


	return(0);
abort:
	return(1);

} /* info */


/********************************* main ************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return           success (0) or error (1)
 */
int main( int argc, char *argv[])
{
    char    *device,*str,*errstr,buf[40];
    int32   n;
    int32   tempHigh;
	int32   arg_info, arg_out, arg_volt_low, arg_volt_high,
		arg_curr_low,arg_curr_high, arg_loop;

    /*--------------------+
    |  check arguments    |
    +--------------------*/

    if( (errstr = UTL_ILLIOPT("lgi=I=r=v=V=T=?", buf)) ){ /* check args */
        printf("*** %s\n", errstr);
        return(1);
    }

    if( UTL_TSTOPT("?") ){                      /* help requested ? */
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

    arg_info		= (UTL_TSTOPT("g") ? 1 : NONE);
	arg_loop		= (UTL_TSTOPT("l") ? 1 : NONE);
	arg_out 		= ((str = UTL_TSTOPT("r=")) ? atoi(str) : NONE);
	arg_volt_low    = ((str = UTL_TSTOPT("v=")) ? atoi(str) : NONE);
	arg_volt_high   = ((str = UTL_TSTOPT("V=")) ? atoi(str) : NONE);
	arg_curr_low    = ((str = UTL_TSTOPT("i=")) ? atoi(str) : NONE);
	arg_curr_high   = ((str = UTL_TSTOPT("I=")) ? atoi(str) : NONE);
    tempHigh 		= ((str = UTL_TSTOPT("T=")) ? atoi(str) : NONE);

	(void)tempHigh;   /* -Wunused-but-set-variable */

    /* -- open path  -- */
    if( (G_Path = M_open(device)) < 0) {
        PrintMdisError("open");
        return(1);
    }

	/* -- change a limit -- */
	if( arg_volt_low != NONE ) {
		CHK_CHAN( (arg_out == NONE) || (arg_out >= CHAN_MAX) );
		CHK((M_setstat(G_Path, M_MK_CH_CURRENT, arg_out)), "M_MK_CH_CURRENT" );
		CHK((M_setstat(G_Path, PU05_VOLT_LOW, arg_volt_low)), "PU05_VOLT_LOW" );
		printf("Set low voltage limit on%srail to %d mV\n",
			   G_rail[arg_out], arg_volt_low );
	}

	if( arg_volt_high != NONE ) {
		CHK_CHAN( (arg_out == NONE) || (arg_out >= CHAN_MAX) );
		CHK((M_setstat(G_Path, M_MK_CH_CURRENT, arg_out)), "M_MK_CH_CURRENT" );
		CHK((M_setstat(G_Path, PU05_VOLT_HIGH, arg_volt_high)),
			"PU05_VOLT_HIGH");
		printf("Set high voltage limit on%srail to %d mV\n",
			   G_rail[arg_out], arg_volt_low );
	}

	if( arg_curr_low != NONE ) {
		CHK_CHAN( (arg_out == NONE) || (arg_out >= CHAN_MAX) );
		CHK((M_setstat(G_Path, M_MK_CH_CURRENT, arg_out)), "M_MK_CH_CURRENT" );
		CHK((M_setstat(G_Path, PU05_CURR_LOW, arg_curr_low)), "PU05_CURR_LOW" );
		printf("Set low current limit on%srail to %d mA\n",
			   G_rail[arg_out], arg_curr_low );
	}

	if( arg_curr_high != NONE ) {
		CHK_CHAN( (arg_out == NONE) || (arg_out >= CHAN_MAX) );
		CHK((M_setstat(G_Path, M_MK_CH_CURRENT, arg_out)), "M_MK_CH_CURRENT" );
		CHK((M_setstat(G_Path, PU05_CURR_HIGH, arg_curr_high)),
			"PU05_CURR_HIGH");
		printf("Set high current limit on%srail to %d mA\n",
			   G_rail[arg_out], arg_curr_low );
	}

    /* -- current settings -- */
    if( arg_info != NONE )
		showInfo();

    /* -- show info in loop -- */
	if( arg_loop != NONE  ) {
		printf("--- press any key to abort ---\n");
		do { 	/* repeat until keypress */
			if( ShowIo() )
				goto abort;

			UOS_Delay(1000);
	    } while( (UOS_KeyPressed() == -1) );
	}

    /* --  cleanup -- */
	if( M_close(G_Path) < 0 )
        PrintMdisError("close");

    return(0);
abort:
	return(1);

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


/********************************* ShowIo **********************************/
/** show i/o, temp and volt
 *
 *  \return           \c 0 success or error code
*/
static int ShowIo( void )
{
    int32   val, chan;
    /* temp */
    CHK( (M_getstat(G_Path, AD78_TEMP, &val)), "AD78_TEMP");
    printf("\n\ntemperature              : %d degree celsius\n", val);

    /* temp high limit */
    CHK((M_getstat(G_Path, AD78_TEMP_HIGH, &val)), "AD78_TEMP_HIGH");
    printf("temperature high limit   : %d degree celsius\n", val);

	for (chan=0; chan <=2; chan++) {
		CHK( (M_setstat(G_Path, M_MK_CH_CURRENT, chan)), "M_MK_CH_CURRENT");
		printf("--%s rail values: --\n", G_rail[chan]);
		CHK( (M_getstat(G_Path, PU05_VOLT, &val)), "PU05_VOLT");
		printf("voltage: %05d mV ", val);
		CHK( (M_getstat(G_Path, PU05_VOLT_LOW, &val)), "PU05_VOLT_LOW");
		printf("low limit: %05d mV ", val);
		CHK( (M_getstat(G_Path, PU05_VOLT_HIGH, &val)), "PU05_VOLT_HIGH");
		printf("high limit: %05d mV\n", val);
		CHK( (M_getstat(G_Path, PU05_CURR, &val)), "PU05_CURR");
		printf("current: %05d mA ", val);
		CHK( (M_getstat(G_Path, PU05_CURR_LOW, &val)), "PU05_CURR_LOW");
		printf("low limit: %05d mA ", val);
		CHK( (M_getstat(G_Path, PU05_CURR_HIGH, &val)), "PU05_CURR_HIGH");
		printf("high limit: %05d mA\n", val);
	}
	CHK( (M_getstat(G_Path, PU05_POWER, &val)), "PU05_POWER");
	printf("output power: %d W\n", val);


    return(0);
abort:
	return(-1);
}

