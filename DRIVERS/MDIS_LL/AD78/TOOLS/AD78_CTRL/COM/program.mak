#***************************  M a k e f i l e  *******************************
#
#         Author: dieter.pfeuffer@men.de
#          $Date: 2006/05/26 15:06:38 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for AD78_CTRL tool
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: program.mak,v $
#   Revision 1.1  2006/05/26 15:06:38  DPfeuffer
#   Initial Revision
#
#-----------------------------------------------------------------------------
#   (c) Copyright 2006 by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=ad78_ctrl

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/mdis_api$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/usr_oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/usr_utl$(LIB_SUFFIX)

MAK_INCL=$(MEN_INC_DIR)/ad78_drv.h	\
         $(MEN_INC_DIR)/ad78c.h		\
         $(MEN_INC_DIR)/men_typs.h	\
         $(MEN_INC_DIR)/mdis_api.h	\
         $(MEN_INC_DIR)/usr_oss.h	\
         $(MEN_INC_DIR)/usr_utl.h

MAK_INP1=ad78_ctrl$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)
