#***************************  M a k e f i l e  *******************************
#
#         Author: dieter.pfeuffer@men.de
#          $Date: 2010/10/08 15:49:02 $
#      $Revision: 1.2 $
#
#    Description: Makefile definitions for the AD78 driver
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: driver.mak,v $
#   Revision 1.2  2010/10/08 15:49:02  ts
#   R: MDVE run showed missing header file
#   M: added pu05c.h to MAK_INC macro
#
#   Revision 1.1  2006/05/26 15:06:27  DPfeuffer
#   Initial Revision
#
#-----------------------------------------------------------------------------
#   (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=ad78

MAK_SWITCH=$(SW_PREFIX)MAC_MEM_MAPPED

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/desc$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/dbg$(LIB_SUFFIX)	\

MAK_INCL=$(MEN_INC_DIR)/ad78_drv.h	\
		 $(MEN_INC_DIR)/men_typs.h	\
		 $(MEN_INC_DIR)/oss.h		\
		 $(MEN_INC_DIR)/mdis_err.h	\
		 $(MEN_INC_DIR)/maccess.h	\
		 $(MEN_INC_DIR)/desc.h		\
		 $(MEN_INC_DIR)/mdis_api.h	\
		 $(MEN_INC_DIR)/mdis_com.h	\
		 $(MEN_INC_DIR)/ll_defs.h	\
		 $(MEN_INC_DIR)/ll_entry.h	\
		 $(MEN_INC_DIR)/dbg.h		\
		 $(MEN_INC_DIR)/smb2.h		\
		 $(MEN_INC_DIR)/ad78c.h		\
		 $(MEN_INC_DIR)/pu05c.h		\
		 $(MEN_INC_DIR)/wdog.h		\

MAK_INP1=ad78_drv$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)
