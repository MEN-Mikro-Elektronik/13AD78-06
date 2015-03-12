#***************************  M a k e f i l e  *******************************
#
#         Author: thomas.schnuerer@men.de
#          $Date: 2009/04/06 13:17:05 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for PU05_CTRL tool
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: program.mak,v $
#   Revision 1.1  2009/04/06 13:17:05  ts
#   R: Initial Revision, first working version reading from PU5
#
#
#-----------------------------------------------------------------------------
#   (c) Copyright 2009 by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=pu05_ctrl

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/mdis_api$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/usr_oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/usr_utl$(LIB_SUFFIX)

MAK_INCL=$(MEN_INC_DIR)/ad78_drv.h	\
         $(MEN_INC_DIR)/ad78c.h		\
         $(MEN_INC_DIR)/pu05c.h		\
         $(MEN_INC_DIR)/men_typs.h	\
         $(MEN_INC_DIR)/mdis_api.h	\
         $(MEN_INC_DIR)/usr_oss.h	\
         $(MEN_INC_DIR)/usr_utl.h

MAK_INP1=pu05_ctrl$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)
