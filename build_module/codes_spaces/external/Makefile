PORTING_SUBDIRS = crypto_md5_sha1  openssl_aes crypto_aes

INSTALL_DIR := ${PLATFORM_RELEASE_DIRECTORY}

all:
	echo "TEST"
	for i in $(PORTING_SUBDIRS);do \
		echo "Build $$i...."; ($(MAKE) -C $$i); \
		if [ $$? != 0 ] ; then \
			exit 1; \
		fi; \
	done

install:
	for i in $(PORTING_SUBDIRS);do \
		echo "Build $$i...."; ($(MAKE) -C $$i install); \
		if [ $$? != 0 ] ; then \
			exit 1; \
		fi; \
	done

clean:
	for i in $(PORTING_SUBDIRS);do \
		echo "Build $$i...."; rm -rf $$i/build; ($(MAKE) -C $$i clean); \
	done
