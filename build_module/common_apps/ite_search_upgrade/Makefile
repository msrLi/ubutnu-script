###############################################################################
# Makefile
# dlx 2011.11.21
###############################################################################

###############################################################################
# add the version number
###############################################################################
VERSION    = 1.0.1
DYNAMIC_SW = on

###############################################################################
# add your AS flags here
###############################################################################
THIS_ASFLAGS    =

###############################################################################
# only for comp local build
###############################################################################
ifeq (${ITARGE_BUILD_OBJS_PATH},)
	BUILD_DIR	= ./build
else
	BUILD_DIR	= ${ITARGE_BUILD_OBJS_PATH}
endif

PLAT_DIR    = ../../${TARGET_CPU_DIR}/makerules
RULES_DIR   = ../../makerules

###############################################################################
# add your CC flags here
###############################################################################
THIS_CCFLAGS    = -I./include -I../app_include

###############################################################################
# add your LD flags here
###############################################################################
THIS_LDFLAGS    = -losal -lhal -lmiddle

###############################################################################
# User should not modify 
###############################################################################
include ${PLAT_DIR}/plat.mk
include ${RULES_DIR}/rule.mk

# file end
