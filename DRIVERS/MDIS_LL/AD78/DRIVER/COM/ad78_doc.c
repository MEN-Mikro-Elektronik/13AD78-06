/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  ad78_doc.c
 *
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2010/02/24 11:01:25 $
 *    $Revision: 1.5 $
 *
 *      \brief   User documentation for AD78 device driver
 *
 *     Required: -
 *
 *     \switches -
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

/*! \mainpage
    This is the documentation of the MDIS5 low-level driver for the monitoring
    controller on the AD78 PSU and the PU5.

	\attention <b>Renaming of AD78 to PU2</b>\n\n
	Article numbers and names of MEN power supplies were restructured.\n
	The power supply formerly known as 08AD78 is now called	PU2 on the
	MEN website, to show its relation to the new PUx group of powersupplies
	from MEN. To remain compatible with previous hard- and software the name AD78 is kept throughout this document. It currently describes the features of the AD78 and PU5 power supply, more PUx variants will follow in future.

    The AD78 and PU5 PSU are power supplies with control and supervision functionality:
	- programmable Power Up Wake ON Time
	- Power Up/Down handshake
	- Watchdog functionality
	- GPIO support
	- temperature and voltage supervision

	Note: MDIS5 32bit drivers are compatible to the MDIS4 drivers but must not
          be mixed with MDIS4 drivers at one target system.

	\n
 	\attention <b>Current measurement change on PUx Rev.01</b>\n
	All power supplies PUx with Rev. R01 and up measure only the overall
	current drawn on the 12V rail and no more the single currents drawn by the
	5V and 3.3V rail. Therefore reading out 3.3V and 5V current returns 0. The
	overall drawn power can still be calculated with P[mW]=12V x I12V[mA].\n
   

    <b>Common Notes:</b>
	- The driver requires MDIS with SMB2 support
    \n
    \section Variants Variants
    In order to support different AD78 modes, the AD78 driver can be built in some
	variants at compilation time:

    \code
    Driver              Variant Description
    --------            --------------------------------
    Standard            standard variant
    \endcode

    \n \section FuncDesc Functional Description

    \n \subsection General General
	The driver supports controlling of the AD78 PSU via M_setstat() and M_getstat().
	No interrupts are used. The driver can inform applications via an MDIS signal
	about a shutdown event by polling the AD78 PSU, see #AD78_BLK_DOWN_SIG_SET.

	When the first path is opened to an AD78 device, the HW and the driver are being
	initialized with default values	(see section about \ref descriptor_entries).
	\n
	The AD78 and PU5 power supplies use the same driver and share the identical
	basic set of GetStat- and SetStat codes. The PU5 also provides 3 output
	rails ( 3.3V, 5V, 12V ) instead the two rails on the AD78. For each of
	these rails limits can be set for over/undervoltage and over/undercurrent. \n

    \n \subsection channel MDIS channels on PU5
	To reduce the amount of different GetStat and SetStat codes the MDIS
	concept of channels is used for PU5. That means that before a certain limit
	for voltage or current can be set the channel has to be selected in the
	application. The channels are defined for PU5 as below: \n
    <table border="1">
    <tr><td><b>channel 0:</b></td><td>select 3.3V rail</td></tr>
    <tr><td><b>channel 1:</b></td><td>select 5V rail</td></tr>
    <tr><td><b>channel 2:</b></td><td>select 12V rail</td></tr>
    </table> \n

	As an example the simplified code snippet below shows how the channels
	are used to read out voltage and current from the 3.3V rail and 5V rail:
	\code
	/* Attention: simplified, no error checks! */
    int32 voltage, current;
	M_setstat(G_Path, M_MK_CH_CURRENT,  0); /* next Getstats go to 3.3V rail */
	M_getstat(G_Path, PU05_VOLT, &voltage);
	M_getstat(G_Path, PU05_CURR, &current);
    printf("Now on 3,3V rail: %d volt %d mA\n", voltage, current);
    M_setstat(G_Path, M_MK_CH_CURRENT,  1); /* next Getstats go to 5V rail */
	M_getstat(G_Path, PU05_VOLT, &voltage);
	M_getstat(G_Path, PU05_CURR, &current);
    printf("Now on 5V rail: %d volt %d mA\n", voltage, current);
    \endcode

    \n \section api_functions Supported API Functions

    <table border="0">
    <tr>
        <td><b>API Function</b></td>
        <td><b>Functionality</b></td>
        <td><b>Corresponding Low-Level Function</b></td></tr>

    <tr><td>M_open()</td><td>Open device</td><td>AD78_Init()</td></tr>

    <tr><td>M_close()     </td><td>Close device             </td>
    <td>AD78_Exit())</td></tr>
    <tr><td>M_setstat()   </td><td>Set device parameter     </td>
    <td>AD78_SetStat()</td></tr>
    <tr><td>M_getstat()   </td><td>Get device parameter     </td>
    <td>AD78_GetStat()</td></tr>
    <tr><td>M_errstringTs() </td><td>Generate error message </td>
    <td>-</td></tr>
    </table>

    \n \section descriptor_entries AD78 Descriptor Entries

    The low-level driver initialization routine decodes the following entries
    ("keys") in addition to the general descriptor keys:

    <table border="0">
    <tr><td><b>Descriptor entry</b></td>
        <td><b>Description</b></td>
        <td><b>Values</b></td>
    </tr>
    <tr><td>SMB_BUSNBR</td>
        <td>SMBus bus number</td>
        <td>0, 1, 2, ...\n
			Default: 0</td>
    </tr>
    <tr><td>SMB_DEVADDR</td>
        <td>SMBus address of AD78</td>
        <td>0x00, 0xff\n
			Default: 0x12</td>
    </tr>
    <tr><td>ONACK_TOUT</td>
        <td>ON acknowledge timeout mode</td>
        <td>see \ref _AD78C_ONACK_TOUT\n
			Default: 0 (no timeout)</td>
    </tr>
    <tr><td>WDOG_TOUT</td>
        <td>Watchdog timeout in 100ms units</td>
        <td>1..255 [100ms]\n
			Default: 255</td>
    </tr>
    <tr><td>DOWN_DELAY</td>
        <td>Shutdown delay mode</td>
        <td>see \ref _AD78C_DOWN_DELAY\n
			Default: 0 (no delay)</td>
    </tr>
    <tr><td>OFF_DELAY</td>
        <td>Off delay mode</td>
        <td>see \ref _AD78C_OFF_DELAY\n
			Default: 0 (no delay)</td>
    </tr>
    <tr><td>TEMP_HIGH</td>
        <td>Temperature high limit in degree celsius</td>
        <td>50..90°C\n
			Default: 90°C</td>
    </tr>
    </table>

    \n \subsection ad78_min   Minimum descriptor
    ad78_min.dsc (see Examples section)\n
    Demonstrates the minimum set of options necessary for using the driver.

    \n \subsection ad78_max   Maximum descriptor
    ad78_max.dsc (see Examples section)\n
    Shows all possible configuration options for this driver.

    \n \section codes AD78 specific Getstat/Setstat codes

	For the watchdog functionality, the driver supports the WDOG
	Getstat/Setstat codes (defined in wdog.h):
	- #WDOG_START
	- #WDOG_STOP
	- #WDOG_TRIG
	- #WDOG_TIME
	- #WDOG_STATUS
	- #WDOG_SHOT\n

	The other driver functionality is supported by the AD78 Getstat/Setstat
	codes.
	See \ref ad78_getset_codes "section about AD78 Getstat/Setstat codes".

    \n \section pu05codes PU5 specific Getstat/Setstat codes

	For the additional PU5 functionality like setting the voltage and current
	limits, see \ref pu05_getset_codes "section about PU5 Getstat/Setstat codes".
	Getstat/Setstat codes (defined in ad78_drv.h):

    \n \section programs Overview of provided programs

    \subsection ad78_simp Simple example for using the driver
    ad78_simp.c (see example section)

    \subsection ad78_ctrl Tool to control AD78 PIC
    ad78_ctrl.c (see example section)

    \subsection pu05_ctrl Tool to control the additional PU5 functions
    pu05_ctrl.c (see example section)

    \subsection wdog_test Tool to service and test the watchdog
    wdog_test.c (see example section)
*/

/** \example ad78_simp.c */
/** \example ad78_win.c */
/** \example ad78_ctrl.c */
/** \example pu05_ctrl.c */
/** \example wdog_test.c */
/** \example ad78_min.dsc */
/** \example ad78_max.dsc */

/*! \page ad78dummy MEN logo
  \menimages
*/
