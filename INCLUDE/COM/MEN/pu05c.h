/***********************  I n c l u d e  -  F i l e  ************************/
/*!  
 *        \file  pu05c.h
 *
 *      \author  thomas.schnuerer@men.de
 *        $Date: 2009/10/08 18:24:55 $
 *    $Revision: 3.2 $
 * 
 *  	 \brief  Header file for 17PUx onboard controller. This
 *               files contains the additional defines that are added
 *               to the AD78's functionality 
 *                      
 *     Switches: -
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

#ifndef _XC02_H
#define _XC02_H

#ifdef __cplusplus
	extern "C" {
#endif

/*--------------------------+
|   DEFINES                 |
+--------------------------*/

/*! \defgroup _PU05 PU5 PIC Firmware Register defines (cmd+byte access) */
/**@{*/
#define PU05C_CURR3V3	 0x20 /**< read 3,3V current ADRESH value (r)	*/
#define PU05C_CURR5V	 0x21 /**< read 5V current ADRESH value (r)	*/
#define PU05C_CURR12V	 0x22 /**< read 12V current ADRESH value (r) */
#define PU05C_VOLT3V3	 0x23 /**< read 3,3V voltage ADRESH value (r) */
#define PU05C_VOLT5V	 0x24 /**< read 5V current ADRESH value (r)	*/
#define PU05C_VOLT12V	 0x25 /**< read 12V current ADRESH value (r) */

#define PU05C_VOLT3V3LO	 0x26 /**< set 3,3V low voltage ADRESH value (r/w) */
#define PU05C_VOLT3V3HI	 0x27 /**< set 3,3V high voltage ADRESH value (r/w) */
#define PU05C_VOLT5VLO	 0x28 /**< set 5V low voltage ADRESH value (r/w) */	
#define PU05C_VOLT5VHI	 0x29 /**< set 5V high voltage ADRESH value (r/w) */
#define PU05C_VOLT12VLO	 0x2a /**< set 12V low voltage ADRESH value (r/w) */	
#define PU05C_VOLT12VHI	 0x2b /**< set 12V high voltage ADRESH value (r/w) */ 

#define PU05C_CURR3V3LO	 0x2c /**< set 3,3V low current ADRESH value (r/w)*/	
#define PU05C_CURR3V3HI	 0x2d /**< set 3,3V high current ADRESH value (r/w)*/
#define PU05C_CURR5VLO	 0x2e /**< set 5V low current ADRESH value (r/w) */	
#define PU05C_CURR5VHI	 0x2f /**< set 5V high current ADRESH value (r/w) */
#define PU05C_CURR12VLO	 0x30 /**< set 12V low current ADRESH value (r/w)*/	
#define PU05C_CURR12VHI	 0x31 /**< set 12V high current ADRESH value (r/w)*/ 
#define PU05C_TEMP_ALERT 0x32 /**< set warning temperature ADRESH value (r/w)*/
#define PU05C_POWER      0x38 /**< get currenly drawn output power (r)*/

/** @} */



/** \name PU5 specific Macros 
 *  \anchor pu5_macros 
 *
 *  <b>Notes about the values in the macros below</b> \n
 *
 *  The conversion between raw ADC values read from firmware and physical
 *  values (mA, mV) must be done such that no floating point arithmetic
 *  is necessary.
 *
 * Origin of values below: 
 * \n 
   \verbatim
  3.3V measured via divider   1k /1k  (x2) => 3000mV x   2 = 6000
    5V measured via divider   2k7/1k  (x3.7) => 3000mV x 3.7 = 11100
   12V measured via divider  10k /2k7 (x4.7) => 3000mV x 4.7 = 14100

    I=0..20A
   |           
   +---------------
   |  		   	 
   -   	   	   	  
  | | Shunt	 
  |	| 0,002 Ohm	 
   - 	 
   |           
   +---------------
   |  
   |  
	  
   \endverbatim

 *
 */
/**@{*/

#define ADC_REF_MV	 3000  /**< 3V Reference */
#define ADC_MAX_VAL  0xff  /**< only 8-bits of ADC-Value are used */

#define ADC2V(adc) ((ADC_REF_MV*(adc))/ADC_MAX_VAL) /*get 0-3000mV from ADC */

/** macro to calculate voltage [mV] from raw ADC-Value returned by FW */
#define PU05C_3V3_VOLT(adc) ((int32)((6000*(adc))/ADC_MAX_VAL)) /**< get 3,3V voltage in mV from raw ADC-Value */	
/** macro to calculate 5V voltage in mV from raw ADC-Value */
#define PU05C_5V_VOLT(adc) ((int32)((11100*(adc))/ADC_MAX_VAL)) /**< get 5V voltage in mV from raw ADC-Value */	

/** macro to calculate 12V voltage in mV from raw ADC-Value */
#define PU05C_12V_VOLT(adc) ((int32)((14100*(adc))/ADC_MAX_VAL)) /**< get 12V voltage in mV from raw ADC-Value */	

		/* Formula: INA194 voltage = 100mV/A => I[mA] = Uadc*10 [mV] */
#define PU05C_3V3_CURR(adc) ((int32)( ADC2V(adc) * 10)) /**< get 3,3V current in mA from raw ADC-Value */	
		/* Formula: INA194 voltage = 100mV/A => I[mA] = Uadc*10 [mV] */
#define PU05C_5V_CURR(adc) ((int32)(  ADC2V(adc) * 10)) /**< get 5V current in mA from raw ADC-Value */	

/* 
 * => I12V[mA] = ((ADC*3000)/255) *  -> 3000 * 6.89 = 20680mA 
 * however, prevent floating point math in drivers, scale to int values:
 *
 *  I[mA] = ((ADC*3000)/255) * 6.89 
 *        = ((ADC*3000*6.89)/255) = (20670*ADC)/255 = 81*ADC
 *                                                    ------
 */

/** get 12V current in mA from raw ADC-Value */
#define PU05C_12V_CURR(adc) ((int32)((adc)*81))

/** macro to calculate temperature [degree celsius] from ADC-Value */
#define PU05C_TEMP_FROM_ADC( adc )	(AD78C_TEMP_FROM_INV(AD78C_ADC2INV(adc )))

/** macro to calculate IN-Voltage [mV] from temperature [degree celsius] */
#define AD78C_TEMP_TO_INV( c )	( (((c) * 156) + 4800) / 10 )
/** macro to calculate ADC-Value from temperature [degree celsius] */
#define AD78C_TEMP_TO_ADC( c )	(AD78C_INV2ADC( AD78C_TEMP_TO_INV( c ) ))

/** @} */

#ifdef __cplusplus
	}
#endif

#endif	/* _PU05C_H */
