/***********************  I n c l u d e  -  F i l e  ************************/
/*!
 *        \file  ad78c.h
 *
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2010/02/24 11:04:12 $
 *    $Revision: 1.5 $
 *
 *  	 \brief  Header file for AD78C controller - SMB address map
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: ad78c.h,v $
 * Revision 1.5  2010/02/24 11:04:12  ts
 * R: Temperature protection switched off too early
 * M: increased limit to 105 degree, climate chamber tested
 *
 * Revision 1.4  2009/09/04 11:41:34  MRoth
 * R: MDVE warning
 * M: fixed AD78C_DOWN_DELAY_MIN2MODE makro
 *
 * Revision 1.3  2006/07/21 10:09:33  DPfeuffer
 * added defines and macros for temperature and voltage computation
 *
 * Revision 1.2  2006/05/26 15:08:32  DPfeuffer
 * cosmetics
 *
 * Revision 1.1  2006/05/24 16:20:51  ag
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
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

#ifndef _AD78C_H
#define _AD78C_H

#ifdef __cplusplus
	extern "C" {
#endif


/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/

/*! \defgroup _AD78C AD78 controller register and bit defines (cmd+byte access) */
/**@{*/
#	define AD78C_FLAG				0x01	/**< common flag */

#define AD78C_WOT_L				0x00	/**< WOT [min] - low byte (r/w) */
#define AD78C_WOT_H				0x01	/**< WOT [min] - high byte (r/w) */

#define AD78C_ONACK				0x02	/**< ON acknowledge (w) */
#define AD78C_ONACK_TOUT		0x03	/**< ON acknowledge timeout (r/w) */
#define AD78C_ONACK_ERR			0x04	/**< ON acknowledge error count (r) */

#define AD78C_WDOG_STATE		0x05	/**< watchdog state (r/w) */
#	define AD78C_WDOG_OFF			0x00	/**< watchdog OFF */
#	define AD78C_WDOG_ON			0x01	/**< watchdog ON */

#define AD78C_WDOG_TRIG			0x06	/**< watchdog trigger (w) */

#define AD78C_WDOG_TOUT			0x07	/**< WDOG timeout [100ms] (r/w) */
#	define AD78C_WDOG_TOUT_MINMODE		0x01	/**< 100ms */
#	define AD78C_WDOG_TOUT_MAXMODE		0xff	/**< 25,5sec */

#define AD78C_WDOG_ERR			0x08	/**< WDOG error count (r) */
#define AD78C_SWOFF				0x09	/**< initiate a power down (w) */
#define AD78C_OFFACK			0x0a	/**< OFF acknowledge (w) */
#define AD78C_DOWN_DELAY		0x0b	/**< shutdown delay (r/w) */
#define AD78C_OFF_DELAY			0x0c	/**< off delay (r/w) */

#define AD78C_STATUS			0x0d	/**< PSU status flags (r) */
#	define AD78C_STATUS_DOWN_EVT		0x01	/**< Shutdown Event flag */

#define AD78C_IN				0x0e	/**< binary inputs state (r) */
#define AD78C_IN_MASK			0x0f	/**< binary inputs mask (r) */

#define AD78C_OUT				0x10	/**< binary outputs state (r/w) */
#define AD78C_OUT_MASK			0x11	/**< binary outputs mask (r) */

#define AD78C_TEMP				0x12	/**< temperature [ADC-Value] (r) */

#define AD78C_TEMP_HIGH			0x13	/**< temperature high limit [ADC-Value] (r/w) */

#define AD78C_VOLT				0x14	/**< 5V supply voltage [ADC-Value] (r) */

#define AD78C_TEST1				0x77	/**< test register #1 (r/w) */
#	define AD78C_TEST1_NOTEST		0x00	/**< no test mode (default) */
#	define AD78C_TEST1_TMODE1		0x61	/**< enable test mode 1:
												 - reset redirected to AD78C_TEST2_RST flag
												 - power-on redirected to AD78C_TEST2_PWR flag  */

#define AD78C_TEST2				0x78	/**< test register #2 (r/w) */
#	define AD78C_TEST2_RST			0x01	/**< 1: indicating reset, cleared by host */
#	define AD78C_TEST2_SWON			0x02	/**< 1: signals immediate WOT, cleared by uC */

#define AD78C_TEST3				0x79	/**< test register #3 (r) */
#	define AD78C_TEST3_PWR			0x01	/**< 1: indicating power-on */

#define AD78C_TEST4				0x7a	/**< test register #4 (r) mirors AD78C_SM_STATES */

#define AD78C_ID				0xfe	/**< identification byte '0x78' (r) */
#define AD78C_REV				0xff	/**< firmware revision (r) */

/**
 * @defgroup _AD78C_ONACK_TOUT ON Acknowledge Timeout defines
 * Possible modes:
 * \code
   Mode          :   0  | 1 | 2 |  3 | 4 |  5 |  6 |  7 |   8 |   9 |  10 |   11
   Timeout [sec] : none | 1 | 2 |  4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 | 1024
   \endcode
 * @{ */
/** macro to convert mode to timeout [sec] */
#	define AD78C_ONACK_TOUT_SEC( mode )		((mode) ? (1<<((mode)-1)) : 0)
/** macro to convert timeout [sec] to mode */
#	define AD78C_ONACK_TOUT_SEC2MODE( sec, mode )	switch( sec ){			\
												case    1: mode= 1; break;	\
												case    2: mode= 2; break;	\
												case    4: mode= 3; break;	\
												case    8: mode= 4; break;	\
												case   16: mode= 5; break;	\
												case   32: mode= 6; break;	\
												case   64: mode= 7; break;	\
												case  128: mode= 8; break;	\
												case  256: mode= 9; break;	\
												case  512: mode=10; break;	\
												case 1024: mode=11; break;	\
												default: mode=0xff; }
#	define AD78C_ONACK_TOUT_NONE		0		/**< no timeout (wait forever)  */
#	define AD78C_ONACK_TOUT_MINMODE		0		/**< min mode */
#	define AD78C_ONACK_TOUT_MAXMODE		11		/**< max mode */
/** @} */

/**
 * @defgroup _AD78C_DOWN_DELAY Shutdown Delay defines
 * Possible modes:
 * \code
   Mode        :   0  | 1 | 2 |  3 | 4 |  5 |  6 |  7
   Delay [min] : none | 1 | 2 |  4 | 8 | 16 | 32 | 64
   \endcode
 * @{ */
/** macro to convert mode to delay [min] */
#	define AD78C_DOWN_DELAY_MIN( mode )		((mode) ? (1<<((mode)-1)) : 0)
/** macro to convert delay [min] to mode */
#	define AD78C_DOWN_DELAY_MIN2MODE( min, mode )	switch( min ){				\
												case    0:  mode=0; break;	\
												case    1:  mode=1; break;	\
												case    2:  mode=2; break;	\
												case    4:  mode=3; break;	\
												case    8:  mode=4; break;	\
												case   16:  mode=5; break;	\
												case   32:  mode=6; break;	\
												case   64:  mode=7; break;	\
												default: mode=0xff; }
#	define AD78C_DOWN_DELAY_MINMODE		0
#	define AD78C_DOWN_DELAY_MAXMODE		7
/** @} */

/**
 * @defgroup _AD78C_OFF_DELAY OFF Delay defines
 * Possible modes:
 * \code
   Mode        :   0  | 1 | 2 |  3 | 4 |  5
   Delay [min] : none | 1 | 2 |  4 | 8 | 16
   \endcode
 * @{ */
/** macro to convert mode to delay [min] */
#	define AD78C_OFF_DELAY_MIN( mode )		((mode) ? (1<<((mode)-1)) : 0)
/** macro to convert delay [min] to mode */
#	define AD78C_OFF_DELAY_MIN2MODE( min, mode )	switch( min ){				\
													case    0:  mode=0; break;	\
													case    1:  mode=1; break;	\
													case    2:  mode=2; break;	\
													case    4:  mode=3; break;	\
													case    8:  mode=4; break;	\
													case   16:  mode=5; break;	\
													default: mode=0xff; }
#	define AD78C_OFF_DELAY_MINMODE	0
#	define AD78C_OFF_DELAY_MAXMODE	5
/** @} */

#define AD78C_ON_ACK_MAX_ERR        5  /**< Max num of tries to reset system
                                            after ON-Ack was not received in time */
#define AD78C_WDOG_MAX_ERR          5  /**< Max num of tries to reset system
                                            after WDOG-Trigger was not received
                                            in time */
#define AD78C_WAIT_OFF              1000 /**< Time in ms to ensure proper power-off */

/**
 * @defgroup _AD78C_TEMP_VOLT defines and macros for temperature and voltage computation
 * @{ */
#define AD78C_ADC_REF_MV		3000    /**< 3V Reference */
#define AD78C_ADC_REF_VALUE		0xff	/**< only 8-bits of ADC-Value are used */

/** macro to calculate IN-Voltage [mV] from ADC-Value */
#define AD78C_ADC2INV( adc )	( (u_int32)( (AD78C_ADC_REF_MV * (adc)) / AD78C_ADC_REF_VALUE) )

/** macro to calculate ADC-Value from IN-Voltage [mV] */
#define AD78C_INV2ADC( mV )		( (u_int8)( (AD78C_ADC_REF_VALUE * (mV)) / AD78C_ADC_REF_MV) )

/** macro to calculate 5V supply voltage [mV] from IN-Voltage [mV]
    voltage divider: 1:2 */
#define AD78C_VOLT_FROM_INV( inVolt )	((inVolt) * 2)
/** macro to calculate 5V supply voltage [mV] from ADC-Value */
#define AD78C_VOLT_FROM_ADC( adc )		(AD78C_VOLT_FROM_INV( AD78C_ADC2INV( adc ) ))

/** macro to calculate temperature [degree celsius] from IN-Voltage [mV]
    LM62 temp sensor: +15.6 mV/°C, DC offset of +480 mV */
#define AD78C_TEMP_FROM_INV( inVolt )	( ((((inVolt*10) - ((inVolt < 480) ? (inVolt*10) : 4800)) + 156/2) / 156) )
/** macro to calculate temperature [degree celsius] from ADC-Value */
#define AD78C_TEMP_FROM_ADC( adc )	(AD78C_TEMP_FROM_INV( AD78C_ADC2INV( adc ) ))

/** macro to calculate IN-Voltage [mV] from temperature [degree celsius] */
#define AD78C_TEMP_TO_INV( c )	( (((c) * 156) + 4800) / 10 )
/** macro to calculate ADC-Value from temperature [degree celsius] */
#define AD78C_TEMP_TO_ADC( c )	(AD78C_INV2ADC( AD78C_TEMP_TO_INV( c ) ))

#define AD78C_TEMP_HIGH_MIN_C		50		/**< min temp high limit:  50°C */
#define AD78C_TEMP_HIGH_MAX_C	   105		/**< max temp high limit: 105°C */
#define AD78C_TEMP_HIGH_MIN			AD78C_TEMP_TO_ADC( AD78C_TEMP_HIGH_MIN_C )	/**< ADC value for min temp high limit */
#define AD78C_TEMP_HIGH_MAX			AD78C_TEMP_TO_ADC( AD78C_TEMP_HIGH_MAX_C )	/**< ADC value for max temp high limit */
#define AD78C_TEMP_HIGH_DEFAULT		AD78C_TEMP_HIGH_MAX		/**< ADC value for default temp limit (=max) */
/** @} */

/*--------------------------------------*/
/*	TYPEDEFS		            		*/
/*--------------------------------------*/

/** States of AD78-Firmware state machine */
typedef enum {
	SM_STATE_OFF,                 /**< Power OFF: Wait for ON-Event */
	SM_STATE_WAIT_SMB_ON_ACK,     /**< Power ON: Wait for ON-Ack */
	SM_STATE_ON,                  /**< Power ON: Wait for OFF-Event, handle WDOG */
	SM_STATE_WAIT_SHUTDOWN_DELAY, /**< Power ON: Wait for SD-Time */
	SM_STATE_WAIT_OFF_DELAY,      /**< Power ON: Wait for OFF-Delay-Time */
	SM_STATE_WAIT_OFF,            /**< Power OFF: Wait for defined time to
	                                   ensure proper power-off */
	SM_STATE_ERROR_OFF,           /**< Power OFF: Error OFF state - never left */
	SM_STATE_RESET                /**< Power ON: Error undervoltage reset */
} AD78C_SM_STATES;

/**@}*/

#ifdef __cplusplus
	}
#endif

#endif	/* _AD78C_H */
