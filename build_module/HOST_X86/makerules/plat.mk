#  -Wno-unused -Werror
PLAT_CCFLAGS =  \
               -Wno-unused-result \
               -I${PLATFORM_RELEASE_DIRECTORY}/include

# PLAT_LDFLAGS = -fPIC -shared -pthread

# PLAT_LDFLAGS = -L${PORTING_INSTALL_DIR}/lib \
               -L${TOOLCHAIN_PATH}/lib \
               -L${PLATFORM_RELEASE_DIRECTORY}/lib \
			   -fPIC -shared -pthread

