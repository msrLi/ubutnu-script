###############################################################################
# rule.mk
# Itarge 2011.08.08
###############################################################################
-include $(PLATFORM_PATH)/build_ignore.mk

THIS_DIR        = $(shell pwd)
THIS_NAME       = $(notdir ${THIS_DIR})

BLANK_VERSION   = $(shell echo $(VERSION) | tr '.' ' ')
ifeq (on, $(DYNAMIC_SW))
THIS_LIB		 = $(THIS_NAME)
DYNAMIC_LINKNAME = lib$(THIS_LIB).so
DYNAMIC_REALNAME = lib$(THIS_LIB).so.$(VERSION)
DYNAMIC_SONAME   = lib$(THIS_LIB).so.$(word 1, $(BLANK_VERSION))
else
THIS_LIB        = lib${THIS_NAME}.a
endif

THIS_EXE        = ${THIS_NAME}.out

###############################################################################
# build tools set
###############################################################################
MAKE    = make
ECHO    = @echo
RM      = @rm -rf
MKDIR   = @mkdir -p
LN		= ln -sf
CD		= cd

ITG_INSTALL = install

AR      = ${COMPILE_PREFIX}ar
AS      = ${COMPILE_PREFIX}as
CC      = ${COMPILE_PREFIX}gcc
LD      = ${COMPILE_PREFIX}gcc

OBJDUMP = ${COMPILE_PREFIX}objdump

ifneq (debug, $(RELEASE_TYPE))
STRIP   = ${COMPILE_PREFIX}strip
else
STRIP   = echo 
endif

###############################################################################
# Output dir set
###############################################################################
LIB_SRCS_DIR    = ${THIS_DIR}/src
EXE_SRCS_DIR    = ${THIS_DIR}/app
TEST_SRCS_DIR   = ${THIS_DIR}/test

# 去除ignore文件 
LIB_IGNORE_FILE=$(addprefix $(LIB_SRCS_DIR)/,$(notdir $(filter $(subst $(PLATFORM_TOP_PATH)/,,$(LIB_SRCS_DIR))%,$(IGNORE_FILES)))) 
EXE_IGNORE_FILE=$(addprefix $(EXE_SRCS_DIR)/,$(notdir $(filter $(subst $(PLATFORM_TOP_PATH)/,,$(EXE_SRCS_DIR))%,$(IGNORE_FILES)))) 
TEST_IGNORE_FILE=$(addprefix $(TEST_SRCS_DIR)/,$(notdir $(filter $(subst $(PLATFORM_TOP_PATH)/,,$(TEST_SRCS_DIR))%,$(IGNORE_FILES)))) 

LIB_SRCS        = $(basename $(notdir $(filter-out $(LIB_IGNORE_FILE),$(wildcard ${LIB_SRCS_DIR}/*.c))))
EXE_SRCS        = $(basename $(notdir $(filter-out $(EXE_IGNORE_FILE),$(wildcard ${EXE_SRCS_DIR}/*.c))))
TEST_SRCS       = $(basename $(notdir $(filter-out $(TEST_IGNORE_FILE),$(wildcard ${TEST_SRCS_DIR}/*.c))))
TEST_BINS       = $(basename $(notdir $(filter-out $(TEST_IGNORE_FILE),$(wildcard ${TEST_SRCS_DIR}/*.c))))

LIB_OBJS_DIR    = ${BUILD_DIR}/${THIS_NAME}/objs/lib
EXE_OBJS_DIR    = ${BUILD_DIR}/${THIS_NAME}/objs/app
TEST_OBJS_DIR   = ${BUILD_DIR}/${THIS_NAME}/objs/test
LIB_OBJS        = $(addprefix ${LIB_OBJS_DIR}/,$(addsuffix .o,${LIB_SRCS}))
EXE_OBJS        = $(addprefix ${EXE_OBJS_DIR}/,$(addsuffix .o,${EXE_SRCS}))
TEST_OBJS       = $(addprefix ${TEST_OBJS_DIR}/,$(addsuffix .o,${TEST_SRCS}))

LIB_DEPS_DIR    = ${BUILD_DIR}/${THIS_NAME}/deps/lib
EXE_DEPS_DIR    = ${BUILD_DIR}/${THIS_NAME}/deps/app
TEST_DEPS_DIR   = ${BUILD_DIR}/${THIS_NAME}/deps/test
LIB_DEPS        = $(addprefix ${LIB_DEPS_DIR}/,$(addsuffix .d,${LIB_SRCS}))
EXE_DEPS        = $(addprefix ${EXE_DEPS_DIR}/,$(addsuffix .d,${EXE_SRCS}))
TEST_DEPS       = $(addprefix ${EXE_DEPS_DIR}/, ${TEST_SRCS})

LIBS_DIR        = ${BUILD_DIR}/${THIS_NAME}/libs
LIB_BUILD       = ${LIBS_DIR}/${THIS_LIB}

EXES_DIR        = ${BUILD_DIR}/${THIS_NAME}/exes
EXE_BUILD       = ${EXES_DIR}/${THIS_EXE}

TESTS_DIR       = ${BUILD_DIR}/${THIS_NAME}/tests
TESTS_BUILD     = $(addprefix ${TESTS_DIR}/, ${TEST_SRCS})

BUILD_DIR_FILES = $(shell ls -A ${BUILD_DIR})

###############################################################################
# build marco
###############################################################################
define OBJ_BUILD_RULE
# Rule to build object file
# param 1: object dir
# param 2: source dir
# param 3: source and object name
${1}/${3}.o: ${2}/${3}.c
	${AT}${ECHO} Compiling [ ${3} ]
	${AT}${CC} ${CCFLAGS} -c $$< -o $$@
endef

define DEP_BUILD_RULE
# Rule to build denpend file
# param 1: depend dir
# param 2: source dir
# param 3: object dir
# param 4: source object and depend name
${1}/${4}.d: ${2}/${4}.c
	${AT}${ECHO} Dependence [ ${4} ]
	${AT}${MKDIR} ${1}
	${AT}${ECHO} -n ${3}/ > $$@
	${AT}${CC} ${CCFLAGS} -MM $$< >> $$@
endef

define TEST_BUILD_RULE
# Rule to build object file
# param 1: target dir
# param 2: object dir
# param 3: source dir
# param 4: source and object name
${2}/${4}.o: ${3}/${4}.c
	${AT}${ECHO} Compiling [ ${4} ]
	${AT}${CC} ${CCFLAGS} -c $$< -o $$@
	${AT}${LD} -o ${1}/${4} ${2}/${4}.o ${LDFLAGS}
endef

###############################################################################
# build flags set
###############################################################################
ASFLAGA += ${THIS_ASFLAGS} ${PLAT_ASFLAGS}
ifeq (on, $(DYNAMIC_SW))
CCFLAGS += -fPIC ${THIS_CCFLAGS} ${PLAT_CCFLAGS} -DITARGE_LIB_VERSION=\"$(VERSION)\"
else
CCFLAGS += ${THIS_CCFLAGS} ${PLAT_CCFLAGS} -DITARGE_LIB_VERSION=\"$(VERSION)\"
endif

############判断GCC版本是否大于4.9.0##############################
GCC_VER_GT490 := $(shell echo `${CC} -dumpversion | cut -f1-2 -d.` \>= 4.9 | sed -e 's/\./*100+/g' | bc )
ifeq ($(GCC_VER_GT490), 1)
CCFLAGS += -fdiagnostics-color=auto
endif

ifneq (${LIB_OBJS},)
LDFLAGS	:= -L${LIBS_DIR} -l${THIS_NAME} ${THIS_LDFLAGS} ${PLAT_LDFLAGS}
else
LDFLAGS	:= ${THIS_LDFLAGS} ${PLAT_LDFLAGS}
endif

###############################################################################
# build rule 
###############################################################################
.PHONY: all clean lib demo

ifeq (install, $(wildcard install))
all: 	lib exe test
	${AT}${ECHO} build ${THIS_NAME} done
	sh -C ${THIS_DIR}/${ITG_INSTALL} ${BUILD_DIR} ${THIS_NAME}
else
all: 	lib exe test
	${AT}${ECHO} build ${THIS_NAME} done
endif


clean:
ifeq (${BUILD_DIR_FILES}, ${THIS_NAME})
	${AT}${RM} ${BUILD_DIR}
else
	${AT}${RM} ${BUILD_DIR}/${THIS_NAME}
endif
	${AT}${ECHO} clean ${THIS_NAME} done

ifneq (${LIB_OBJS},)
lib:	build_path ${THIS_LIB}
else
lib:	build_path
endif

ifneq (${EXE_OBJS},)
exe:	${THIS_EXE}
else
exe:
	${AT}${ECHO} Empty exe source dir!
endif

ifneq (${TEST_OBJS},)
test:	${TEST_BINS}
	    ${AT}${STRIP} ${TESTS_BUILD}
else
test:
	${AT}${ECHO} Empty test source dir!
endif

ifeq (on, $(DYNAMIC_SW))
${THIS_LIB}: ${LIB_OBJS}
	${AT}${LD} -shared -Wl,-soname,$(DYNAMIC_SONAME) -o $(LIBS_DIR)/$(DYNAMIC_REALNAME) $(LIB_OBJS)
	${AT}${CD} $(LIBS_DIR); ${AT}${LN} $(DYNAMIC_REALNAME) $(DYNAMIC_SONAME)
	${AT}${CD} $(LIBS_DIR); ${AT}${LN} $(DYNAMIC_SONAME)   $(DYNAMIC_LINKNAME)
	${AT}${ECHO} build ${THIS_LIB} done
else
${THIS_LIB}: ${LIB_OBJS}
	${AT}${AR} -rc ${LIB_BUILD} ${LIB_OBJS}
	${AT}${ECHO} build ${THIS_LIB} done
endif

${THIS_EXE}: ${EXE_OBJS}
	${AT}${LD} -o ${EXE_BUILD} ${EXE_OBJS} ${LDFLAGS}
	${AT}${STRIP} ${EXE_BUILD}
	${AT}${ECHO} build ${THIS_EXE} done

${TEST_BINS}: ${TEST_OBJS}

$(foreach dep,${LIB_SRCS},$(eval $(call DEP_BUILD_RULE,${LIB_DEPS_DIR},${LIB_SRCS_DIR},${LIB_OBJS_DIR},${dep})))
$(foreach dep,${EXE_SRCS},$(eval $(call DEP_BUILD_RULE,${EXE_DEPS_DIR},${EXE_SRCS_DIR},${EXE_OBJS_DIR},${dep})))
$(foreach dep,${TEST_SRCS},$(eval $(call DEP_BUILD_RULE,${TEST_DEPS_DIR},${TEST_SRCS_DIR},${TEST_OBJS_DIR},${dep})))

$(foreach obj,${LIB_SRCS},$(eval $(call OBJ_BUILD_RULE,${LIB_OBJS_DIR},${LIB_SRCS_DIR},${obj})))
$(foreach obj,${EXE_SRCS},$(eval $(call OBJ_BUILD_RULE,${EXE_OBJS_DIR},${EXE_SRCS_DIR},${obj})))
$(foreach obj,${TEST_SRCS},$(eval $(call TEST_BUILD_RULE,${TESTS_DIR},${TEST_OBJS_DIR},${TEST_SRCS_DIR},${obj})))

-include ${LIB_DEPS}
-include ${EXE_DEPS}

build_path:
	${AT}${MKDIR} ${LIBS_DIR}
	${AT}${MKDIR} ${EXES_DIR}
	${AT}${MKDIR} ${TESTS_DIR}
	${AT}${MKDIR} ${LIB_OBJS_DIR}
	${AT}${MKDIR} ${EXE_OBJS_DIR}
	${AT}${MKDIR} ${TEST_OBJS_DIR}
	${AT}${MKDIR} ${LIB_DEPS_DIR}
	${AT}${MKDIR} ${EXE_DEPS_DIR}
	${AT}${MKDIR} ${TEST_DEPS_DIR}

