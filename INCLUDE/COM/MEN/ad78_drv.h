/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  ad78_drv.h
 *
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2010/02/24 11:10:05 $
 *    $Revision: 3.5 $
 *
 *       \brief  Header file for AD78 driver containing
 *               AD78 specific status codes and
 *               AD78 function prototypes
 *
 *    \switches  _ONE_NAMESPACE_PER_DRIVER_
 *               _LL_DRV_
 */
 /*
 * (c) Copyright by MEN Mikro Elektronik GmbH, Nuernberg, Germany
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

#ifndef _AD78_DRV_H
#define _AD78_DRV_H

#ifdef __cplusplus
      extern "C" {
#endif

/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/** structure for #AD78_BLK_DOWN_SIG_SET setstat */
typedef struct {
	u_int32 msec;			/* poll period [msec] */
	u_int32 signal;			/* signal to send */
} AD78_BLK_DOWN_SIG;

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/** \name AD78 specific Getstat/Setstat codes
 *  \anchor ad78_getset_codes
 */
/**@{*/

#define AD78_WOT		M_DEV_OF+0x00   /**< G,S: Wake ON Time in minutes (0=no WOT)\n
											      Values: 0..65535\n
												  Default: 0\n */
#define AD78_ONACK		M_DEV_OF+0x01   /**<  S: Send ON Acknowledge\n */
#define AD78_ONACK_TOUT	M_DEV_OF+0x02   /**<G,S: ON Acknowledge Timeout mode\n
											      Values: see \ref _AD78C_ONACK_TOUT\n
												  Default: 0 (no timeout)\n*/
#define AD78_ONACK_ERR	M_DEV_OF+0x03   /**<G  : Number of missing ON Acknowledges after the last shutdown\n
											      Values: 0..n\n
												  Default: 0\n */
#define AD78_WDOG_ERR	M_DEV_OF+0x04   /**<G  : Number of missing watchdog triggers after the last shutdown\n
											      Values: 0..n\n
												  Default: 0\n */
#define AD78_SWOFF		M_DEV_OF+0x05   /**<  S: Initiate a software shutdown\n */
#define AD78_OFFACK		M_DEV_OF+0x06   /**<  S: Send OFF Acknowledge\n */
#define AD78_DOWN_DELAY	M_DEV_OF+0x07   /**<G,S: Shutdown delay mode\n
												  Values: see \ref _AD78C_DOWN_DELAY\n
												  Default: 0 (no delay)\n*/
#define AD78_OFF_DELAY	M_DEV_OF+0x08   /**<G,S: Off delay mode\n
												  Values: see \ref _AD78C_OFF_DELAY\n
												  Default: 0 (no delay)\n*/
#define AD78_DOWN_EVT	M_DEV_OF+0x09   /**<G  : Shutdown event flag\n
											      Values: 0,1\n
												  Default: 0\n */
#define AD78_DOWN_SIG_CLR	M_DEV_OF+0x0a   /**< S: Clear signal for Shutdown Event\n */
#define AD78_IN			M_DEV_OF+0x0b   /**<G  : Binary inputs state\n
											      Values: Bit 7..0 : input 7..0, 1=set, 0=not set\n */
#define AD78_IN_MASK	M_DEV_OF+0x0c   /**<G  : Binary inputs mask\n
											      Values: Bit 7..0 : input 7..0, 1=usable, 0=not implemented\n */
#define AD78_OUT		M_DEV_OF+0x0d   /**<G  : Get binary outputs state\n
											      Values: Bit 7..0 : output 7..0, 1=set, 0=not set\n */
#define AD78_OUT_LOW	M_DEV_OF+0x0e   /**<  S: Set binary outputs low\n
											      Values: Bit 7..0 : output 7..0, 1=set low, 0=old state\n */
#define AD78_OUT_HIGH	M_DEV_OF+0x0f   /**<  S: Set binary outputs high\n
											      Values: Bit 7..0 : output 7..0, 1=set high, 0=old state\n */
#define AD78_OUT_MASK	M_DEV_OF+0x10   /**<G  : Binary outputs mask\n
											      Values: Bit 7..0 : output 7..0, 1=usable, 0=not implemented\n */
#define AD78_TEMP		M_DEV_OF+0x11   /**<G  : Temperature [degree celsius]\n
											      Values: 0..90°C\n */
#define AD78_TEMP_HIGH	M_DEV_OF+0x12   /**<G,S: Temperature high limit [degree celsius]\n
											      Values: 50..90°C\n
												  PUx:    50..105°C\n									  
												  Default: 90 (AD78), 105 (PUx)\n */
#define AD78_VOLTAGE	M_DEV_OF+0x13   /**<G  : PSU input voltage [mV]\n
											      Values: 0..6000mV\n
												  Default: 5000mV\n */
#ifndef _DOXYGEN_
/* Test registers for PIC firmware test */
#define AD78_TEST1		M_DEV_OF+0x14   /**<G,S: Test register #1 */
#define AD78_TEST2		M_DEV_OF+0x15   /**<G,S: Test register #2 */
#define AD78_TEST3		M_DEV_OF+0x16   /**<G  : Test register #3 */
#define AD78_TEST4		M_DEV_OF+0x17   /**<G  : Test register #3 */
#endif

/* AD78 specific Getstat/Setstat block codes */
#define AD78_BLK_DOWN_SIG_SET	M_DEV_BLK_OF+0x00   /**< S: Install signal for shutdown event\n
											      Values: see #AD78_BLK_DOWN_SIG structure\n
												  Default: 0 (no signal)\n */
/**@}*/


/** \name PU5 specific Getstat/Setstat codes
 *  \anchor pu05_getset_codes
 *
 *  \attention <b>Important Note about maximum limit values</b> \n
 *
 *  Its possible with the driver to set maximum current values such that
 *  the calculated power would exceed what the PU5 is capable to deliver.
 *
 *  For example, the limits could be be set like the following:
 *   <table border="0">
 *   <tr><td><b>  3.3V:</b></td> <td>10000 mA</td></tr>
 *   <tr><td><b>    5V:</b></td> <td>10000 mA</td></tr>
 *   <tr><td><b>   12V:</b></td> <td> 5000 mA</td></tr>
 *   </table>
 *  which would result in a theoretical global power of
 *  \verbatim ( 3.3V * 10A ) + ( 5V * 10A ) + ( 12V * 5A ) = 33W + 50W + 60W = 143W\endverbatim
 *  which the PU5 can not deliver.
 *  Therefore the firmware also monitors and calculates the drawn power. If an overall power
 *  consumption of 90W is exceeded, the PSU also switches off independent from the passed limit
 *  values. This assures additional operating security.
 */
/**@{*/
#define PU05_CURR			M_DEV_OF+0x20  /**<G  : Current drawn on selected rail [mA]\n
											    Values: 0..max mA (estimated, if no load on other 2 rails)\n */
#define PU05_VOLT			M_DEV_OF+0x21  /**<G  : Voltage on selected rail [mV]\n
										        Values: 0..max mV\n */
#define PU05_VOLT_LOW		M_DEV_OF+0x22  /**<G,S: low voltage limit for selected rail [mV]\n
										 	    Default: 0mV (minimum)\n */
#define PU05_VOLT_HIGH		M_DEV_OF+0x23  /**<G,S: high voltage limit for selected rail [mV]\n
										        Default: 24000mV (maximum)\n */
#define PU05_CURR_LOW		M_DEV_OF+0x24  /**<G,S: low current limit for selected rail [mA]\n
										 	        Default: 0mV (minimum)\n */
#define PU05_CURR_HIGH		M_DEV_OF+0x25  /**<G,S: high current limit for selected rail [mA]\n
										 	        Default: 000mV (maximum)\n */
#define PU05_TEMP_ALERT		M_DEV_OF+0x26
#define PU05_POWER			M_DEV_OF+0x27  /**<G  : currently drawn power [W]\n */
/**@}*/


#ifndef  AD78_VARIANT
# define AD78_VARIANT AD78
#endif

# define _AD78_GLOBNAME(var,name) var##_##name
#ifndef _ONE_NAMESPACE_PER_DRIVER_
# define AD78_GLOBNAME(var,name) _AD78_GLOBNAME(var,name)
#else
# define AD78_GLOBNAME(var,name) _AD78_GLOBNAME(AD78,name)
#endif

#define __AD78_GetEntry    AD78_GLOBNAME(AD78_VARIANT,GetEntry)

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
#ifdef _LL_DRV_
#ifndef _ONE_NAMESPACE_PER_DRIVER_
	extern void __AD78_GetEntry(LL_ENTRY* drvP);
#endif
#endif /* _LL_DRV_ */

/*-----------------------------------------+
|  BACKWARD COMPATIBILITY TO MDIS4         |
+-----------------------------------------*/
#ifndef U_INT32_OR_64
 /* we have an MDIS4 men_types.h and mdis_api.h included */
 /* only 32bit compatibility needed!                     */
 #define INT32_OR_64  int32
 #define U_INT32_OR_64 u_int32
 typedef INT32_OR_64  MDIS_PATH;
#endif /* U_INT32_OR_64 */


#ifdef __cplusplus
      }
#endif

#endif /* _AD78_DRV_H */
