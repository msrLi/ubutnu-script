SUBDIRS  = ite_search_upgrade ite_board_test

INSTALL_DIR := ${PLATFORM_RELEASE_DIRECTORY}

all:
	for i in $(SUBDIRS);do \
		echo "Build $$i...."; ($(MAKE) -C $$i); \
		if [ $$? != 0 ] ; then \
			exit 1; \
		fi; \
	done

install:
	for i in $(SUBDIRS);do \
		echo "Build $$i...."; ($(MAKE) -C $$i install); \
		if [ $$? != 0 ] ; then \
			exit 1; \
		fi; \
	done

clean:
	for i in $(SUBDIRS);do \
		echo "Build $$i...."; rm -rf $$i/build; ($(MAKE) -C $$i clean); \
	done
