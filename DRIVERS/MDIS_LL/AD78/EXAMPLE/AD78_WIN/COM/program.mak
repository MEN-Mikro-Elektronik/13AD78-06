#***************************  M a k e f i l e  *******************************
#
#         Author: dieter.pfeuffer@men.de
#          $Date: 2006/05/26 15:06:36 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for the AD78_WIN example program
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: program.mak,v $
#   Revision 1.1  2006/05/26 15:06:36  DPfeuffer
#   Initial Revision
#
#-----------------------------------------------------------------------------
#   (c) Copyright 2006 by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=ad78_win

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/mdis_api$(LIB_SUFFIX)	\
		 $(LIB_PREFIX)$(MEN_LIB_DIR)/usr_oss$(LIB_SUFFIX)

MAK_INCL=$(MEN_INC_DIR)/ad78_drv.h	\
         $(MEN_INC_DIR)/ad78c.h		\
         $(MEN_INC_DIR)/men_typs.h	\
         $(MEN_INC_DIR)/mdis_api.h	\
         $(MEN_INC_DIR)/usr_oss.h	\

MAK_INP1=ad78_win$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)
