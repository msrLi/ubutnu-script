# (c) Itarge.Inc

include Rules.make

all: codes_spaces
clean: codes_spaces_clean

help:
	@echo  "libs:    build all platform libs"
	@echo  "libs_clean clean all platform libs"

common_headers:
	# copy 通用文件到对应的平台发布目录
	cp -af common_include/* ${TARGET_CPU_DIR}/platform_release/include

codes_spaces:common_headers
	make -C codes_spaces

codes_spaces_clean:
	rm -rf ${ITARGE_BUILD_OBJS_PATH}
