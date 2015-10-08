/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  ad78_drv.c
 *
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2010/10/08 15:44:06 $
 *    $Revision: 1.7 $
 *
 *      \brief   Low-level driver for PIC on SMBus at AD78 PSU
 *
 *     Required: OSS, DESC, DBG, libraries
 *
 *     \switches _ONE_NAMESPACE_PER_DRIVER_
 */
/*-------------------------------[ History ]--------------------------------
 *
 * $Log: ad78_drv.c,v $
 * Revision 1.7  2010/10/08 15:44:06  ts
 * R: uninitialized arg_ptr warning during MDVE run
 * M: initialize pointer to NULL
 *
 * Revision 1.6  2010/10/08 12:18:12  ts
 * R: Declaration and Implementation of Get- and SetStat differed
 * M: changed order of parameters code and ch in Prototypes
 *
 * Revision 1.5  2010/01/25 14:53:59  ts
 * R: new Powersupplies PU6 and PU8 introduced by MEN
 * M: added support for PU IDs 0x06 (PU6) and 0x08 (PU8)
 *
 * Revision 1.4  2009/08/31 14:35:38  MRoth
 * R: Porting to MIDS5
 * M: added support for 64bit (Set/GetStat prototypes)
 *
 * Revision 1.3  2009/04/23 16:50:24  ts
 * R: 1) support new PUx powersupply family
 * M: 1) added Get/SetStats for PU5
 *
 * Revision 1.2  2006/07/21 10:09:20  DPfeuffer
 * temperature and voltage measurement implemented
 *
 * Revision 1.1  2006/05/26 15:06:24  DPfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN Mikro Elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#define _NO_LL_HANDLE		/* ll_defs.h: don't define LL_HANDLE struct */

#ifdef AD78_SW				/* swapped variant */
#	define MAC_MEM_MAPPED
#	define ID_SW
#endif

#include <MEN/men_typs.h>   /* system dependent definitions */
#include <MEN/maccess.h>    /* hw access macros and types */
#include <MEN/dbg.h>        /* debug functions */
#include <MEN/oss.h>        /* oss functions */
#include <MEN/desc.h>       /* descriptor functions */
#include <MEN/mdis_api.h>   /* MDIS global defs */
#include <MEN/mdis_com.h>   /* MDIS common defs */
#include <MEN/mdis_err.h>   /* MDIS error codes */
#include <MEN/ll_defs.h>    /* low-level driver definitions */
#include <MEN/smb2.h>		/* SMB2 definitions */
#include <MEN/ad78c.h>		/* AD78 controller definitions	*/
#include <MEN/pu05c.h>		/* additional PU05 controller definitions */
#include <MEN/wdog.h>		/* watchdog specific definitions  */

/*-----------------------------------------+
  |  DEFINES                                 |
  +-----------------------------------------*/
/* general defines */
#define CH_NUMBER		3		/**< Number of device channels */
#define CH_BYTES		1		/**< Number of bytes per channel */
#define USE_IRQ			FALSE	/**< Interrupt required  */
#define ADDRSPACE_COUNT	0		/**< Number of required address spaces */
#define ADDRSPACE_SIZE	0		/**< Size of address space */

/* currently supported Firmware IDs */
#define DEV_ID_AD78		0x78 /**< device ID of 08AD78 */
#define DEV_ID_PU05		0x05 /**< device ID of 17PU05 */
#define DEV_ID_PU06		0x06 /**< device ID of 17PU06 */
#define DEV_ID_PU07		0x07 /**< device ID of 17PU07 (internal only!) */
#define DEV_ID_PU08		0x08 /**< device ID of 17PU08 */

#define CHAN_0			0
#define CHAN_1			1
#define CHAN_2			2

/* debug defines */
#define DBG_MYLEVEL			llHdl->dbgLevel   /**< Debug level */
#define DBH					llHdl->dbgHdl     /**< Debug handle */

/* SMB access macros */
#define SMB_W_BYTE( cmd, val )							\
	error = llHdl->smbH->WriteByteData( llHdl->smbH, 0,	\
						 llHdl->smbAddr, (u_int8)(cmd), (u_int8)(val) );

#define SMB_R_BYTE( cmd, valP )							\
	*valP = 0;											\
	error = llHdl->smbH->ReadByteData( llHdl->smbH, 0,	\
							   llHdl->smbAddr, (u_int8)(cmd), (u_int8*)(valP));

/*-----------------------------------------+
  |  TYPEDEFS                                |
  +-----------------------------------------*/
/** low-level handle */
typedef struct {
	/* general */
    int32           memAlloc;		/**< Size allocated for the handle */
    OSS_HANDLE      *osHdl;         /**< OSS handle */
    OSS_IRQ_HANDLE  *irqHdl;        /**< IRQ handle */
    DESC_HANDLE     *descHdl;       /**< DESC handle */
	MDIS_IDENT_FUNCT_TBL idFuncTbl;	/**< ID function table */
    u_int32         idCheck;		/**< id check enabled */
	/* debug */
    u_int32         dbgLevel;		/**< Debug level */
	DBG_HANDLE      *dbgHdl;        /**< Debug handle */
	/* ad78 specific */
	SMB_HANDLE		*smbH;			/**< ptr to SMB_HANDLE struct */
	u_int16			smbAddr;		/**< SMB address of AD78 */
    OSS_SIG_HANDLE  *sigHdl;        /**< signal handle */
    OSS_ALARM_HANDLE *alarmHdl;		/**< alarm handle */
	u_int8			wdState;		/**< Watchdog state */
} LL_HANDLE;

/* include files which need LL_HANDLE */
#include <MEN/ll_entry.h>   /* low-level driver jump table  */
#include <MEN/ad78_drv.h>	/* AD78 driver header file */

/*-----------------------------------------+
  |  PROTOTYPES                              |
  +-----------------------------------------*/
static int32 AD78_Init(DESC_SPEC *descSpec, OSS_HANDLE *osHdl,
					   MACCESS *ma, OSS_SEM_HANDLE *devSemHdl,
					   OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP);
static int32 AD78_Exit(LL_HANDLE **llHdlP );
static int32 AD78_Read(LL_HANDLE *llHdl, int32 ch, int32 *value);
static int32 AD78_Write(LL_HANDLE *llHdl, int32 ch, int32 value);
static int32 AD78_SetStat(LL_HANDLE *llHdl, int32 code, int32 ch,
							INT32_OR_64 value32_or_64);
static int32 AD78_GetStat(LL_HANDLE *llHdl, int32 code, int32 ch,
							INT32_OR_64 *value32_or_64P );
static int32 AD78_BlockRead(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							int32 *nbrRdBytesP);
static int32 AD78_BlockWrite(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							 int32 *nbrWrBytesP);
static int32 AD78_Irq(LL_HANDLE *llHdl );
static int32 AD78_Info(int32 infoType, ... );

static char* Ident( void );
static int32 Cleanup(LL_HANDLE *llHdl, int32 retCode);

/* AD78 specific helper functions */
static void AlarmHandler(void *arg);

/****************************** AD78_GetEntry ********************************/
/** Initialize driver's jump table
 *
 *  \param drvP     \OUT Pointer to the initialized jump table structure
 */
#ifdef _ONE_NAMESPACE_PER_DRIVER_
void LL_GetEntry( LL_ENTRY* drvP )
#else
    void __AD78_GetEntry( LL_ENTRY* drvP )
#endif /* _ONE_NAMESPACE_PER_DRIVER_ */
{
    drvP->init        = AD78_Init;
    drvP->exit        = AD78_Exit;
    drvP->read        = AD78_Read;
    drvP->write       = AD78_Write;
    drvP->blockRead   = AD78_BlockRead;
    drvP->blockWrite  = AD78_BlockWrite;
    drvP->setStat     = AD78_SetStat;
    drvP->getStat     = AD78_GetStat;
    drvP->irq         = AD78_Irq;
    drvP->info        = AD78_Info;
}

/******************************** AD78_Init **********************************/
/** Allocate and return low-level handle, initialize hardware
 *
 * The function initializes the AD78 device with the definitions made
 * in the descriptor.
 *
 * The function decodes \ref descriptor_entries "these descriptor entries"
 * in addition to the general descriptor keys.
 *
 *  \param descP      \IN  Pointer to descriptor data
 *  \param osHdl      \IN  OSS handle
 *  \param ma         \IN  HW access handle
 *  \param devSemHdl  \IN  Device semaphore handle
 *  \param irqHdl     \IN  IRQ handle
 *  \param llHdlP     \OUT Pointer to low-level driver handle
 *
 *  \return           \c 0 On success or error code
 */
static int32 AD78_Init(
    DESC_SPEC       *descP,
    OSS_HANDLE      *osHdl,
    MACCESS         *ma,
    OSS_SEM_HANDLE  *devSemHdl,
    OSS_IRQ_HANDLE  *irqHdl,
    LL_HANDLE       **llHdlP
	)
{
    LL_HANDLE	*llHdl = NULL;
    u_int32		gotsize, smbBusNbr;
    int32		error;
	u_int8		value8;
	u_int32		value32;
	u_int32		onackTout;
	u_int32		wdogTout;
	u_int32		downDelay;
	u_int32		offDelay;
	u_int32		tempHigh;

    /*------------------------------+
	 |  prepare the handle          |
	 +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */

	/* alloc */
    if((llHdl = (LL_HANDLE*)OSS_MemGet(
			osHdl, sizeof(LL_HANDLE), &gotsize)) == NULL)
		return(ERR_OSS_MEM_ALLOC);

	/* clear */
    OSS_MemFill(osHdl, gotsize, (char*)llHdl, 0x00);

	/* init */
    llHdl->memAlloc   = gotsize;
    llHdl->osHdl      = osHdl;
    llHdl->irqHdl     = irqHdl;

    /*------------------------------+
	  |  init id function table     |
	  +-----------------------------*/
	/* driver's ident function */
	llHdl->idFuncTbl.idCall[0].identCall = Ident;
	/* library's ident functions */
	llHdl->idFuncTbl.idCall[1].identCall = DESC_Ident;
	llHdl->idFuncTbl.idCall[2].identCall = OSS_Ident;
	/* terminator */
	llHdl->idFuncTbl.idCall[3].identCall = NULL;

    /*------------------------------+
	  |  prepare debugging          |
	  +-----------------------------*/
	DBG_MYLEVEL = OSS_DBG_DEFAULT;	/* set OS specific debug level */
	DBGINIT((NULL,&DBH));

    /*------------------------------+
	  |  scan descriptor            |
	  +-----------------------------*/
	/* prepare access */
    if((error = DESC_Init(descP, osHdl, &llHdl->descHdl)))
		return( Cleanup(llHdl,error) );

    /* DEBUG_LEVEL_DESC */
    if((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
							   &value32, "DEBUG_LEVEL_DESC")) &&
	   error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	DESC_DbgLevelSet(llHdl->descHdl, value32);	/* set level */

    /* DEBUG_LEVEL */
    if((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
							   &llHdl->dbgLevel, "DEBUG_LEVEL")) &&
	   error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

    DBGWRT_1((DBH, "LL - AD78_Init\n"));

    /* ID_CHECK */
    if ((error = DESC_GetUInt32(llHdl->descHdl, TRUE,
								&llHdl->idCheck, "ID_CHECK")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

    /* SMB_BUSNBR (required) */
    if((error = DESC_GetUInt32(llHdl->descHdl, 0,
							   &smbBusNbr, "SMB_BUSNBR")))
		return( Cleanup(llHdl,error) );

    /* SMB_DEVADDR (required) */
    if((error = DESC_GetUInt32(llHdl->descHdl, 0,
							   &value32, "SMB_DEVADDR")))
		return( Cleanup(llHdl,error) );
	llHdl->smbAddr = (u_int16)value32;

    /* ONACK_TOUT */
    if((error = DESC_GetUInt32(llHdl->descHdl, AD78C_ONACK_TOUT_NONE,
							   &onackTout, "ONACK_TOUT")) &&
	   error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );
	if( ((int32)onackTout < AD78C_ONACK_TOUT_MINMODE) ||
		(onackTout > AD78C_ONACK_TOUT_MAXMODE) )
		return( Cleanup(llHdl,ERR_LL_DESC_PARAM) );

    /* WDOG_TOUT in 100ms units */
    if((error = DESC_GetUInt32(llHdl->descHdl, AD78C_WDOG_TOUT_MAXMODE,
							   &wdogTout, "WDOG_TOUT")) &&
	   error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );
	if( (wdogTout < AD78C_WDOG_TOUT_MINMODE) ||
		(wdogTout > AD78C_WDOG_TOUT_MAXMODE) )
		return( Cleanup(llHdl,ERR_LL_DESC_PARAM) );

    /* DOWN_DELAY */
    if((error = DESC_GetUInt32(llHdl->descHdl, AD78C_DOWN_DELAY_MINMODE,
							   &downDelay, "DOWN_DELAY")) &&
	   error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );
	if( ((int32)downDelay < AD78C_DOWN_DELAY_MINMODE) ||
		(downDelay > AD78C_DOWN_DELAY_MAXMODE) )
		return( Cleanup(llHdl,ERR_LL_DESC_PARAM) );

    /* OFF_DELAY */
    if((error = DESC_GetUInt32(llHdl->descHdl, AD78C_OFF_DELAY_MINMODE,
							   &offDelay, "OFF_DELAY")) &&
	   error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );
	if( ((int32)offDelay < AD78C_OFF_DELAY_MINMODE) ||
		(offDelay > AD78C_OFF_DELAY_MAXMODE) )
		return( Cleanup(llHdl,ERR_LL_DESC_PARAM) );
	/* TEMP_HIGH */
    if((error = DESC_GetUInt32(llHdl->descHdl, AD78C_TEMP_HIGH_MAX_C,
							   &tempHigh, "TEMP_HIGH")) &&
	   error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );
	if( (tempHigh < AD78C_TEMP_HIGH_MIN_C) ||
		(tempHigh > AD78C_TEMP_HIGH_MAX_C) )
		return( Cleanup(llHdl,ERR_LL_DESC_PARAM) );

    /*------------------------------+
	  |  init alarm                 |
	  +-----------------------------*/
    if((error = OSS_AlarmCreate(llHdl->osHdl, AlarmHandler, llHdl,
								&llHdl->alarmHdl)) )
		return( Cleanup(llHdl,error));

    /*------------------------------+
	  |  get SMB handle             |
	  +-----------------------------*/
	if ((error = OSS_GetSmbHdl( llHdl->osHdl, smbBusNbr,(void**)&llHdl->smbH)))
		return( Cleanup(llHdl,error) );

    /*------------------------------+
	  |  check module id            |
	  +-----------------------------*/
	if (llHdl->idCheck) {

		SMB_R_BYTE( AD78C_ID, &value8 );
		if( error )
			return( error );

		switch (value8) {

		case DEV_ID_AD78:
			DBGWRT_2((DBH, " AD78_Init: 08AD78 found.\n"));
			break;

		case DEV_ID_PU05:
			DBGWRT_2((DBH, " AD78_Init: 17PU05 found.\n"));
			break;

		case DEV_ID_PU06:
			DBGWRT_2((DBH, " AD78_Init: 17PU06 found.\n"));
			break;

		case DEV_ID_PU07:
			DBGWRT_2((DBH, " AD78_Init: 17PU07 found.\n"));
			break;

		case DEV_ID_PU08:
			DBGWRT_2((DBH, " AD78_Init: 17PU08 found.\n"));
			break;

		default:
			DBGWRT_ERR((DBH," *** AD78_Init: illegal id=0x%x\n", value8));
			error = ERR_LL_ILL_ID;
			return( Cleanup(llHdl,error) );
		}
	}

#if DBG
	/* get and print firmware revision */
	SMB_R_BYTE( AD78C_REV, &value8 );
	DBGWRT_2((DBH, "AD78C_REV=0x%02x\n", value8));
#endif

    /*------------------------------+
	  |  init hardware              |
	  +-----------------------------*/

	/* ONACK_TOUT */
	SMB_W_BYTE( AD78C_ONACK_TOUT, onackTout );
	if( error  )
		return( Cleanup(llHdl,error) );

	/* WDOG_TOUT */
	SMB_W_BYTE( AD78C_WDOG_TOUT, wdogTout );
	if( error  )
		return( Cleanup(llHdl,error) );

	/* DOWN_DELAY */
	SMB_W_BYTE( AD78C_DOWN_DELAY, downDelay );
	if( error  )
		return( Cleanup(llHdl,error) );

	/* OFF_DELAY */
	SMB_W_BYTE( AD78C_OFF_DELAY, offDelay );
	if( error  )
		return( Cleanup(llHdl,error) );

	/* TEMP_HIGH */
	SMB_W_BYTE( AD78C_TEMP_HIGH, AD78C_TEMP_TO_ADC( tempHigh ) );
	if( error  )
		return( Cleanup(llHdl,error) );

	*llHdlP = llHdl;	/* set low-level driver handle */

	return(ERR_SUCCESS);
}

/****************************** AD78_Exit ************************************/
/** De-initialize hardware and clean up memory
 *
 *  The function deinitializes the AD78 device.
 *
 *  \param llHdlP      \IN  Pointer to low-level driver handle
 *
 *  \return           \c 0 On success or error code
 */
static int32 AD78_Exit(
	LL_HANDLE    **llHdlP
	)
{
    LL_HANDLE *llHdl = *llHdlP;
	int32 error = 0;

    DBGWRT_1((DBH, "LL - AD78_Exit\n"));

    /*------------------------------+
	  |  de-init hardware           |
	  +----------------------------*/

    /*------------------------------+
	  |  clean up memory            |
	  +-----------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */
	error = Cleanup(llHdl,error);

	return(error);
}

/****************************** AD78_Read ************************************/
/** Read a value from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param valueP     \OUT Read value
 *
 *  \return           \c 0 On success or error code
 */
static int32 AD78_Read(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 *valueP
	)
{
    DBGWRT_1((DBH, "LL - AD78_Read: ch=%d\n",ch));

	return(ERR_LL_ILL_FUNC);
}

/****************************** AD78_Write ***********************************/
/** Write a value to the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param value      \IN  Read value
 *
 *  \return           \c ERR_LL_ILL_FUNC
 */
static int32 AD78_Write(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 value
	)
{
    DBGWRT_1((DBH, "LL - AD78_Write: ch=%d\n",ch));

	return(ERR_LL_ILL_FUNC);
}

/****************************** AD78_SetStat *********************************/
/** Set the driver status
 *
 *  The driver supports \ref ad78_getset_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl  	  \IN  Low-level handle
 *  \param code       \IN  \ref ad78_getset_codes "status code"
 *  \param ch         \IN  Current channel
 *  \param value32_or_64  \IN  Data or pointer to block data structure (M_SG_BLOCK)
 *                             for block status codes
 *  \return           \c 0 On success or error code
 */
static int32 AD78_SetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 value32_or_64
	)
{
	int32	error = ERR_SUCCESS;
	u_int8	oldState;

	int32		value  	= (int32)value32_or_64;	/* 32bit value */
	INT32_OR_64	valueP 	= value32_or_64;		/* stores 32/64bit pointer */

    DBGWRT_1((DBH, "LL - AD78_SetStat: ch=%d code=0x%04x value=0x%x\n",
			  ch,code,value));

    switch(code) {
        /*--------------------------+
		  |  debug level            |
		  +-------------------------*/
	case M_LL_DEBUG_LEVEL:
		llHdl->dbgLevel = value;
		break;

        /*--------------------------+
		  |  channel direction      |
		  +-------------------------*/

	case M_LL_CH_DIR:
		if( value != M_CH_INOUT )
			error = ERR_LL_ILL_DIR;
		break;
        /*--------------------------+
		  |  WOT                    |
		  +-------------------------*/

	case AD78_WOT:
		if( value > 0xffff ) {
			error = ERR_LL_ILL_PARAM;
			break;
		}

		SMB_W_BYTE( AD78C_WOT_L, value );
		if( error )
			break;

		SMB_W_BYTE( AD78C_WOT_H, value>>8 );
		break;
        /*--------------------------+
		  |  ONACK                  |
		  +-------------------------*/
	case AD78_ONACK:
		SMB_W_BYTE( AD78C_ONACK, AD78C_FLAG );
		break;
        /*--------------------------+
		  |  ONACK_TOUT             |
		  +-------------------------*/
	case AD78_ONACK_TOUT:
		/* check range */
		if( (value < AD78C_ONACK_TOUT_MINMODE) ||
			(value > AD78C_ONACK_TOUT_MAXMODE) ){
			error = ERR_LL_ILL_PARAM;
			break;
		}
		SMB_W_BYTE( AD78C_ONACK_TOUT, value );
		break;
        /*--------------------------+
		  |  SWOFF                  |
		  +-------------------------*/
	case AD78_SWOFF:
		SMB_W_BYTE( AD78C_SWOFF, AD78C_FLAG );
		break;
        /*--------------------------+
		  |  OFFACK                 |
		  +-------------------------*/
	case AD78_OFFACK:
		SMB_W_BYTE( AD78C_OFFACK, AD78C_FLAG );
		break;
        /*--------------------------+
		  |  DOWN_DELAY             |
		  +-------------------------*/
	case AD78_DOWN_DELAY:
		if( (value < AD78C_DOWN_DELAY_MINMODE) ||
			(value > AD78C_DOWN_DELAY_MAXMODE) ){
			error = ERR_LL_ILL_PARAM;
			break;
		}
		SMB_W_BYTE( AD78C_DOWN_DELAY, value );
		break;
        /*--------------------------+
		  |  OFF_DELAY              |
		  +-------------------------*/
	case AD78_OFF_DELAY:
		if( (value < AD78C_OFF_DELAY_MINMODE) ||
			(value > AD78C_OFF_DELAY_MAXMODE) ){
			error = ERR_LL_ILL_PARAM;
			break;
		}
		SMB_W_BYTE( AD78C_OFF_DELAY, value );
		break;
        /*------------------------------------+
		  | install signal for shutdown event |
		  +-----------------------------------*/
	case AD78_BLK_DOWN_SIG_SET:
	{
		M_SG_BLOCK			*blk = (M_SG_BLOCK*)valueP;
		AD78_BLK_DOWN_SIG	*alm = (AD78_BLK_DOWN_SIG*)blk->data;
		u_int32				realMsec;

		/* check buf size */
		if( blk->size < sizeof(AD78_BLK_DOWN_SIG) )
			return(ERR_LL_USERBUF);

		/* illegal signal code ? */
		if( alm->signal == 0 ){
			DBGWRT_ERR((DBH, " *** AD78_SetStat: illegal signal code=0x%x",
						alm->signal));
			return(ERR_LL_ILL_PARAM);
		}

		/* already defined ? */
		if( llHdl->sigHdl != NULL ){
			DBGWRT_ERR((DBH,
						" *** AD78_SetStat: signal already installed"));
			return(ERR_OSS_SIG_SET);
		}

		/* install signal+alarm */
		if( (error = OSS_SigCreate(llHdl->osHdl,
								   alm->signal,
								   &llHdl->sigHdl)) )
			return(error);

		if( (error = OSS_AlarmSet(llHdl->osHdl, llHdl->alarmHdl,
								  alm->msec, 1, &realMsec)) )
			return(error);
		break;
	}
	/*---------------------------------------+
	  | deinstall signal for shutdown event  |
	  +--------------------------------------*/
	case AD78_DOWN_SIG_CLR:
		/* not defined ? */
		if( llHdl->sigHdl == NULL ){
			DBGWRT_ERR((DBH, " *** AD78_SetStat: signal not installed"));
			return(ERR_OSS_SIG_CLR);
		}

		/* remove signal+alarm */
		if( (error = OSS_SigRemove(llHdl->osHdl, &llHdl->sigHdl)) )
			return(error);

		if( (error = OSS_AlarmClear(llHdl->osHdl, llHdl->alarmHdl)) )
			return(error);

		break;
        /*--------------------------+
		  |  OUT_LOW                |
		  +-------------------------*/
	case AD78_OUT_LOW:
		SMB_R_BYTE( AD78C_OUT, &oldState );
		SMB_W_BYTE( AD78C_OUT, oldState & ~value );
		break;
        /*--------------------------+
		  |  OUT_HIGH               |
		  +-------------------------*/
	case AD78_OUT_HIGH:
		SMB_R_BYTE( AD78C_OUT, &oldState );
		SMB_W_BYTE( AD78C_OUT, oldState | value );
		break;
        /*--------------------------+
		  |  TEMP_HIGH              |
		  +-------------------------*/
	case AD78_TEMP_HIGH:
		if( (value < AD78C_TEMP_HIGH_MIN_C) ||
			(value > AD78C_TEMP_HIGH_MAX_C) ){
			error = ERR_LL_ILL_PARAM;
			break;
		}
		SMB_W_BYTE( AD78C_TEMP_HIGH, AD78C_TEMP_TO_ADC( value ) );
		break;
        /*--------------------------+
		  |  start WDOG             |
		  +-------------------------*/
	case WDOG_START:
		SMB_W_BYTE( AD78C_WDOG_STATE, AD78C_WDOG_ON );
		if( error  )
			break;
		llHdl->wdState = 1;
		break;
        /*--------------------------+
		  |  stop WDOG              |
		  +-------------------------*/
	case WDOG_STOP:
		SMB_W_BYTE( AD78C_WDOG_STATE, AD78C_WDOG_OFF );
		if( error  )
			break;
		llHdl->wdState = 0;
		break;
        /*--------------------------+
		  |  trigger WDOG           |
		  +-------------------------*/
	case WDOG_TRIG:
		/* watchdog not enabled ? */
		if ( llHdl->wdState == 0 ) {
			error = ERR_LL_DEV_NOTRDY;
			break;
		}
		SMB_W_BYTE( AD78C_WDOG_TRIG, AD78C_FLAG );
		break;
        /*--------------------------+
		  |  WDOG time in ms        |
		  +-------------------------*/
	case WDOG_TIME:
		/* check range */
		if( (value % 100) ||
			((value/100) < AD78C_WDOG_TOUT_MINMODE) ||
			((value/100) > AD78C_WDOG_TOUT_MAXMODE) ){
			error = ERR_LL_ILL_PARAM;
			break;
		}

		SMB_W_BYTE( AD78C_WDOG_TOUT, value/100 );
		break;
        /*--------------------------+
		  |  Test registers #1..2   |
		  |  (for PIC firmware test)|
		  +-------------------------*/
	case AD78_TEST1:
		DBGWRT_1((DBH, "SetStat: AD78_TEST1\n"));
		SMB_W_BYTE( AD78C_TEST1, value );
		break;


	case AD78_TEST2:
		SMB_W_BYTE( AD78C_TEST2, value );
		break;


        /*---------------------------+
		  | additional PU05 SetStats |
		  +--------------------------*/

	case PU05_CURR:
		switch (ch) {
		case CHAN_0:
			SMB_W_BYTE( PU05C_CURR3V3, value );
			break;
		case CHAN_1:
			SMB_W_BYTE( PU05C_CURR5V, value );
			break;
		case CHAN_2:
			SMB_W_BYTE( PU05C_CURR12V , value );
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_VOLT:
		switch (ch) {
		case CHAN_0:
			SMB_W_BYTE( PU05C_VOLT3V3, value );
			break;
		case CHAN_1:
			SMB_W_BYTE( PU05C_VOLT5V, value );
			break;
		case CHAN_2:
			SMB_W_BYTE( PU05C_VOLT12V , value );
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_VOLT_LOW:
		switch (ch) {
		case CHAN_0:
			SMB_W_BYTE( PU05C_VOLT3V3LO, value );
			break;
		case CHAN_1:
			SMB_W_BYTE( PU05C_VOLT5VLO, value );
			break;
		case CHAN_2:
			SMB_W_BYTE( PU05C_VOLT12VLO, value );
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_VOLT_HIGH:
		switch (ch) {
		case CHAN_0:
			SMB_W_BYTE( PU05C_VOLT3V3HI, value );
			break;
		case CHAN_1:
			SMB_W_BYTE( PU05C_VOLT5VHI,  value );
			break;
		case CHAN_2:
			SMB_W_BYTE( PU05C_VOLT12VHI, value );
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_CURR_LOW:
		switch (ch) {
		case CHAN_0:
			SMB_W_BYTE( PU05C_CURR3V3LO, value );
			break;
		case CHAN_1:
			SMB_W_BYTE( PU05C_CURR5VLO, value );
			break;
		case CHAN_2:
			SMB_W_BYTE( PU05C_CURR12VLO, value );
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_CURR_HIGH:
		switch (ch) {
		case CHAN_0:
			SMB_W_BYTE( PU05C_CURR3V3HI, value );
			break;
		case CHAN_1:
			SMB_W_BYTE( PU05C_CURR5VHI,  value );
			break;
		case CHAN_2:
			SMB_W_BYTE( PU05C_CURR12VHI, value );
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_TEMP_ALERT:
		SMB_W_BYTE( PU05C_TEMP_ALERT, value );
		break;

		/*-----------------------+
		  |  unknown              |
		  +-----------------------*/
	default:
		error = ERR_LL_UNK_CODE;
    }

	return(error);
}

/****************************** AD78_GetStat *********************************/
/** Get the driver status
 *
 *  The driver supports \ref ad78_getset_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param code       \IN  \ref ad78_getset_codes "status code"
 *  \param ch         \IN  Current channel
 *  \param value32_or_64P  \IN  Pointer to block data structure (M_SG_BLOCK) for
 *                              block status codes
 *  \param value32_or_64P  \OUT  Data pointer or pointer to block data structure
 *                               (M_SG_BLOCK) for block status codes
 *  \return           \c 0 On success or error code
 */
static int32 AD78_GetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 *value32_or_64P
	)
{
	int32	error = ERR_SUCCESS;
	u_int8  val = 0;
	u_int8	regVal;

	int32		*valueP	  = (int32*)value32_or_64P;	/* pointer to 32bit value  */
	INT32_OR_64	*value64P = value32_or_64P;		 	/* stores 32/64bit pointer */

    DBGWRT_1((DBH, "LL - AD78_GetStat: ch=%d code=0x%04x\n",
			  ch,code));

    switch(code)
    {
        /*--------------------------+
		  |  debug level            |
		  +-------------------------*/
	case M_LL_DEBUG_LEVEL:
		*valueP = llHdl->dbgLevel;
		break;
        /*--------------------------+
		  |  number of channels     |
		  +-------------------------*/
	case M_LL_CH_NUMBER:
		*valueP = CH_NUMBER;
		break;
        /*--------------------------+
		  |  channel direction      |
		  +-------------------------*/
	case M_LL_CH_DIR:
		*valueP = M_CH_INOUT;
		break;
        /*--------------------------+
		  |  channel length [bits]  |
		  +-------------------------*/
	case M_LL_CH_LEN:
		*valueP = 8;
		break;
        /*--------------------------+
		  |  channel type info      |
		  +-------------------------*/
	case M_LL_CH_TYP:
		*valueP = M_CH_UNKNOWN;
		break;
        /*--------------------------+
		  |  id check enabled       |
		  +-------------------------*/
	case M_LL_ID_CHECK:
		*valueP = llHdl->idCheck;
		break;
        /*--------------------------+
		  |   ident table pointer   |
		  |   (treat as non-block!) |
		  +-------------------------*/
	case M_MK_BLK_REV_ID:
		*value64P = (INT32_OR_64)&llHdl->idFuncTbl;
		break;
        /*--------------------------+
		  |  WOT                    |
		  +-------------------------*/
	case AD78_WOT:
	{
		u_int8 lVal, hVal;

		SMB_R_BYTE( AD78C_WOT_L, &lVal );
		if( error  )
			break;

		SMB_R_BYTE( AD78C_WOT_H, &hVal );
		if( error  )
			break;

		*valueP = (int32)(hVal<<8 | lVal);
		break;
	}
	/*--------------------------+
	  |  ONACK_TOUT             |
	  +-------------------------*/
	case AD78_ONACK_TOUT:
		SMB_R_BYTE( AD78C_ONACK_TOUT, &val );
		*valueP = val;
		break;

		/*--------------------------+
		  |  ONACK_ERR              |
		  +-------------------------*/
	case AD78_ONACK_ERR:
		SMB_R_BYTE( AD78C_ONACK_ERR, &val );
		*valueP = val;
		break;

        /*--------------------------+
		  |  WDOG_ERR               |
		  +-------------------------*/
	case AD78_WDOG_ERR:
		SMB_R_BYTE( AD78C_WDOG_ERR, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  DOWN_DELAY             |
		  +-------------------------*/
	case AD78_DOWN_DELAY:
		SMB_R_BYTE( AD78C_DOWN_DELAY, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  OFF_DELAY              |
		  +-------------------------*/
	case AD78_OFF_DELAY:
		SMB_R_BYTE( AD78C_OFF_DELAY, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  DOWN_EVT               |
		  +-------------------------*/
	case AD78_DOWN_EVT:
		SMB_R_BYTE( AD78C_STATUS, &regVal );
		*valueP = (int32)(regVal & AD78C_STATUS_DOWN_EVT ? 1 : 0);
		break;
        /*--------------------------+
		  |  IN                     |
		  +-------------------------*/
	case AD78_IN:
		SMB_R_BYTE( AD78C_IN, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  IN_MASK                |
		  +-------------------------*/
	case AD78_IN_MASK:
		SMB_R_BYTE( AD78C_IN_MASK, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  OUT                    |
		  +-------------------------*/
	case AD78_OUT:
		SMB_R_BYTE( AD78C_OUT, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  OUT_MASK               |
		  +-------------------------*/
	case AD78_OUT_MASK:
		SMB_R_BYTE( AD78C_OUT_MASK, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  TEMP                     |
		  +--------------------------*/
	case AD78_TEMP:
		SMB_R_BYTE( AD78C_TEMP, &regVal );
		*valueP = (int32)( AD78C_TEMP_FROM_ADC( regVal ) );
		break;
        /*--------------------------+
		  |  TEMP_HIGH              |
		  +-------------------------*/
	case AD78_TEMP_HIGH:
		SMB_R_BYTE( AD78C_TEMP_HIGH, &regVal );
		*valueP = (int32)(AD78C_TEMP_FROM_ADC( regVal ) );
		break;
        /*--------------------------+
		  |  VOLTAGE                |
		  +-------------------------*/
	case AD78_VOLTAGE:
		SMB_R_BYTE( AD78C_VOLT, &regVal );
		*valueP = (int32)( AD78C_VOLT_FROM_ADC( regVal ) );
		break;
        /*--------------------------+
		  |  WDOG time in ms        |
		  +-------------------------*/
	case WDOG_TIME:
		SMB_R_BYTE( AD78C_WDOG_TOUT, &regVal );
		*valueP = (int32)(regVal * 100);
		break;
        /*--------------------------+
		  |  WDOG status            |
		  +-------------------------*/
	case WDOG_STATUS:
		SMB_R_BYTE( AD78C_WDOG_STATE, &val );
		*valueP = val;
		break;
        /*--------------------------+
		  |  WDOG shot              |
		  +-------------------------*/
	case WDOG_SHOT:
		SMB_R_BYTE( AD78C_WDOG_ERR, &regVal );
		*valueP = (int32)(regVal ? 1 : 0);
		break;
        /*--------------------------+
		  | Test registers #1..3    |
		  | (for PIC firmware test) |
		  +-------------------------*/
	case AD78_TEST1:
		SMB_R_BYTE( AD78C_TEST1, &val );
		*valueP = val;
		break;
	case AD78_TEST2:
		SMB_R_BYTE( AD78C_TEST2, &val );
		*valueP = val;
		break;
	case AD78_TEST3:
		SMB_R_BYTE( AD78C_TEST3, &val );
		*valueP = val;
		break;
	case AD78_TEST4:
		SMB_R_BYTE( AD78C_TEST4, &val);
		*valueP = val;
		break;

        /*--------------------------+
		  |additional PU05 GetStats |
		  +-------------------------*/
	case PU05_CURR:
		switch (ch) {
		case CHAN_0:
			SMB_R_BYTE( PU05C_CURR3V3, &val );
			*valueP = PU05C_3V3_CURR(val);
			break;
		case CHAN_1:
			SMB_R_BYTE( PU05C_CURR5V, &val );
			*valueP = PU05C_5V_CURR(val);
			break;
		case CHAN_2:
			SMB_R_BYTE( PU05C_CURR12V, &val );
			*valueP = PU05C_12V_CURR(val);
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		DBGWRT_2((DBH, "PU05_CURR = %ld mA, raw=0x%02x\n", *valueP, val));
		break;

	case PU05_VOLT:
		switch (ch) {
		case CHAN_0:
			SMB_R_BYTE( PU05C_VOLT3V3, &val );
			*valueP = PU05C_3V3_VOLT(val & 0xff);
			break;
		case CHAN_1:
			SMB_R_BYTE( PU05C_VOLT5V, &val );
			*valueP = PU05C_5V_VOLT(val);
			break;
		case CHAN_2:
			SMB_R_BYTE( PU05C_VOLT12V , &val );
			*valueP = PU05C_12V_VOLT(val);
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		DBGWRT_2((DBH, "PU05_VOLT = %ld mV, raw=0x%02x\n", *valueP,val));
		break;

	case PU05_VOLT_LOW:
		switch (ch) {
		case CHAN_0:
			SMB_R_BYTE( PU05C_VOLT3V3LO, &val );
			*valueP = PU05C_3V3_VOLT(val);
			break;
		case CHAN_1:
			SMB_R_BYTE( PU05C_VOLT5VLO, &val );
			*valueP = PU05C_5V_VOLT(val);
			break;
		case CHAN_2:
			SMB_R_BYTE( PU05C_VOLT12VLO, &val );
			*valueP = PU05C_12V_VOLT(val);
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_VOLT_HIGH:
		switch (ch) {
		case CHAN_0:
			SMB_R_BYTE( PU05C_VOLT3V3HI, &val );
			*valueP = PU05C_3V3_VOLT(val);
			break;
		case CHAN_1:
			SMB_R_BYTE( PU05C_VOLT5VHI,  &val );
			*valueP = PU05C_5V_VOLT(val);
			break;
		case CHAN_2:
			SMB_R_BYTE( PU05C_VOLT12VHI, &val );
			*valueP = PU05C_12V_VOLT(val);
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_CURR_LOW:
		switch (ch) {
		case CHAN_0:
			SMB_R_BYTE( PU05C_CURR3V3LO, &val );
			*valueP = PU05C_3V3_CURR(val);
			break;
		case CHAN_1:
			SMB_R_BYTE( PU05C_CURR5VLO, &val );
			*valueP = PU05C_5V_CURR(val);
			break;
		case CHAN_2:
			SMB_R_BYTE( PU05C_CURR12VLO, &val );
			*valueP = PU05C_12V_CURR(val);
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_CURR_HIGH:
		switch (ch) {
		case CHAN_0:
			SMB_R_BYTE( PU05C_CURR3V3HI, &val );
			*valueP = PU05C_3V3_CURR(val);
			break;
		case CHAN_1:
			SMB_R_BYTE( PU05C_CURR5VHI,  &val );
			*valueP = PU05C_5V_CURR(val);
			break;
		case CHAN_2:
			SMB_R_BYTE( PU05C_CURR12VHI, &val );
			*valueP = PU05C_12V_CURR(val);
			break;
		default:
			error = ERR_LL_ILL_CHAN;
		}
		break;

	case PU05_TEMP_ALERT:
		SMB_R_BYTE( PU05C_TEMP_ALERT, &val );
		*valueP = val;
		break;

		/* currently drawn power */
	case PU05_POWER:
		SMB_R_BYTE( PU05C_POWER, &val );
		*valueP = val;
		break;

		/*--------------------------+
		  |  unknown                |
		  +-------------------------*/
	default:
		error = ERR_LL_UNK_CODE;
    }

	return(error);
}

/******************************* AD78_BlockRead ******************************/
/** Read a data block from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl       \IN  Low-level handle
 *  \param ch          \IN  Current channel
 *  \param buf         \IN  Data buffer
 *  \param size        \IN  Data buffer size
 *  \param nbrRdBytesP \OUT Number of read bytes
 *
 *  \return            \c 0 On success or error code
 */
static int32 AD78_BlockRead(
	LL_HANDLE *llHdl,
	int32     ch,
	void      *buf,
	int32     size,
	int32     *nbrRdBytesP
	)
{
    DBGWRT_1((DBH, "LL - AD78_BlockRead: ch=%d, size=%d\n",ch,size));

	return(ERR_LL_ILL_FUNC);

}

/****************************** AD78_BlockWrite ******************************/
/** Write a data block from the device
 *
 *  The function is not supported and always returns an ERR_LL_ILL_FUNC error.
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \param ch          \IN  Current channel
 *  \param buf         \IN  Data buffer
 *  \param size        \IN  Data buffer size
 *  \param nbrWrBytesP \OUT Number of written bytes
 *
 *  \return            \c ERR_LL_ILL_FUNC
 */
static int32 AD78_BlockWrite(
	LL_HANDLE *llHdl,
	int32     ch,
	void      *buf,
	int32     size,
	int32     *nbrWrBytesP
	)
{
    DBGWRT_1((DBH, "LL - AD78_BlockWrite: ch=%d, size=%d\n",ch,size));

	/* return number of written bytes */
	*nbrWrBytesP = 0;

	return(ERR_LL_ILL_FUNC);
}


/****************************** AD78_Irq ************************************/
/** Interrupt service routine - unused
 *
 *  If the driver can detect the interrupt's cause it returns
 *  LL_IRQ_DEVICE or LL_IRQ_DEV_NOT, otherwise LL_IRQ_UNKNOWN.
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \return LL_IRQ_DEVICE	IRQ caused by device
 *          LL_IRQ_DEV_NOT  IRQ not caused by device
 *          LL_IRQ_UNKNOWN  Unknown
 */
static int32 AD78_Irq(
	LL_HANDLE *llHdl
	)
{
	return(LL_IRQ_DEV_NOT);
}

/****************************** AD78_Info ***********************************/
/** Get information about hardware and driver requirements
 *
 *  The following info codes are supported:
 *
 * \code
 *  Code                      Description
 *  ------------------------  -----------------------------
 *  LL_INFO_HW_CHARACTER      Hardware characteristics
 *  LL_INFO_ADDRSPACE_COUNT   Number of required address spaces
 *  LL_INFO_ADDRSPACE         Address space information
 *  LL_INFO_IRQ               Interrupt required
 *  LL_INFO_LOCKMODE          Process lock mode required
 * \endcode
 *
 *  The LL_INFO_HW_CHARACTER code returns all address and
 *  data modes (ORed) which are supported by the hardware
 *  (MDIS_MAxx, MDIS_MDxx).
 *
 *  The LL_INFO_ADDRSPACE_COUNT code returns the number
 *  of address spaces used by the driver.
 *
 *  The LL_INFO_ADDRSPACE code returns information about one
 *  specific address space (MDIS_MAxx, MDIS_MDxx). The returned
 *  data mode represents the widest hardware access used by
 *  the driver.
 *
 *  The LL_INFO_IRQ code returns whether the driver supports an
 *  interrupt routine (TRUE or FALSE).
 *
 *  The LL_INFO_LOCKMODE code returns which process locking
 *  mode the driver needs (LL_LOCK_xxx).
 *
 *  \param infoType	   \IN  Info code
 *  \param ...         \IN  Argument(s)
 *
 *  \return            \c 0 On success or error code
 */
static int32 AD78_Info(
	int32  infoType,
	...
	)
{
    int32   error = ERR_SUCCESS;
    va_list argptr;

    va_start(argptr, infoType );

    switch(infoType) {
		/*-------------------------------+
		  |  hardware characteristics    |
		  |  (all addr/data modes ORed)  |
		  +------------------------------*/
	case LL_INFO_HW_CHARACTER:
	{
		u_int32 *addrModeP = va_arg(argptr, u_int32*);
		u_int32 *dataModeP = va_arg(argptr, u_int32*);

		*addrModeP = MDIS_MA08;
		*dataModeP = MDIS_MD08 | MDIS_MD16;
		break;
	}
	/*-------------------------------+
	  | nr of required address spaces|
	  | (total spaces used)          |
	  +------------------------------*/
	case LL_INFO_ADDRSPACE_COUNT:
	{
		u_int32 *nbrOfAddrSpaceP = va_arg(argptr, u_int32*);

		*nbrOfAddrSpaceP = ADDRSPACE_COUNT;
		break;
	}
	/*-------------------------------+
	  |  address space type          |
	  |  (widest used data mode)     |
	  +------------------------------*/
	case LL_INFO_ADDRSPACE:
	{
		u_int32 addrSpaceIndex = va_arg(argptr, u_int32);
		u_int32 *addrModeP = va_arg(argptr, u_int32*);
		u_int32 *dataModeP = va_arg(argptr, u_int32*);
		u_int32 *addrSizeP = va_arg(argptr, u_int32*);

		if((int32)addrSpaceIndex >= ADDRSPACE_COUNT)
			error = ERR_LL_ILL_PARAM;
		else {
			*addrModeP = MDIS_MA08;
			*dataModeP = MDIS_MD16;
			*addrSizeP = ADDRSPACE_SIZE;
		}

		break;
	}
	/*-------------------------------+
	  |   interrupt required         |
	  +------------------------------*/
	case LL_INFO_IRQ:
	{
		u_int32 *useIrqP = va_arg(argptr, u_int32*);

		*useIrqP = USE_IRQ;
		break;
	}
	/*-------------------------------+
	  |   process lock mode          |
	  +------------------------------*/
	case LL_INFO_LOCKMODE:
	{
		u_int32 *lockModeP = va_arg(argptr, u_int32*);

		*lockModeP = LL_LOCK_CALL;
		break;
	}
	/*-------------------------------+
	  |   (unknown)                  |
	  +------------------------------*/
	default:
		error = ERR_LL_ILL_PARAM;
    }

    va_end(argptr);
    return(error);
}

/*******************************  Ident  ***********************************/
/** Return ident string
 *
 *  \return            Pointer to ident string
 */
static char* Ident( void )
{
    return( "AD78 - AD78 low-level driver: $Id: ad78_drv.c,v 1.7 2010/10/08 15:44:06 ts Exp $" );
}

/********************************* Cleanup *********************************/
/** Close all handles, free memory and return error code
 *
 *	\warning The low-level handle is invalid after this function is called.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param retCode    \IN  Return value
 *
 *  \return           \IN   retCode
 */
static int32 Cleanup(
	LL_HANDLE    *llHdl,
	int32        retCode
	)
{
    /*------------------------------+
	  |  close handles              |
	  +-----------------------------*/
	/* clean up desc */
	if(llHdl->descHdl)
		DESC_Exit(&llHdl->descHdl);

	/* clean up alarm */
	if (llHdl->alarmHdl)
		OSS_AlarmRemove(llHdl->osHdl, &llHdl->alarmHdl);

	/* clean up signal */
	if (llHdl->sigHdl)
		OSS_SigRemove(llHdl->osHdl, &llHdl->sigHdl);

	/* clean up debug */
	DBGEXIT((&DBH));

    /*------------------------------+
	  |  free memory                |
	  +-----------------------------*/
    /* free my handle */
    OSS_MemFree(llHdl->osHdl, (int8*)llHdl, llHdl->memAlloc);

    /*------------------------------+
	  |  return error code          |
	  +-----------------------------*/
	return(retCode);
}

/******************************* AlarmHandler *******************************
 *
 *  Description: Handler for alarm
 *
 *---------------------------------------------------------------------------
 *  Input......: arg		ll handle
 *  Output.....: -
 *  Globals....: -
 ****************************************************************************/
static void AlarmHandler(void *arg)
{
	LL_HANDLE	*llHdl = (LL_HANDLE*)arg;
	int32		error;
	u_int8		status;

	DBGWRT_1((DBH,">>> LL - AD78 AlarmHandler:\n"));

	SMB_R_BYTE( AD78C_STATUS, &status );

	if( status & AD78C_STATUS_DOWN_EVT ){
		OSS_SigSend( llHdl->osHdl, llHdl->sigHdl );
		DBGWRT_2((DBH, " shutdown event --> send signal\n"));
	}
}

