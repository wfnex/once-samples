#
# Copyright (C) 2013-2015, Nanjing WFNEX Technology Co., Ltd
#
export CURRENTPWD:=$(shell pwd)
export PRJDIR=$(CURRENTPWD)
export USER_ROOT=$(CURRENTPWD)/../../
export PROJECT_NAME=vbng
#######################################################################################
#######################################################################################
NULL_STDERR = 2>$(/dev/null) || true
MAJOR_VERSION := $(shell awk '/USER_MAJOR_VERSION/ { print $$3}' ${USER_ROOT}/include/version.h)
MINOR_VERSION := $(shell awk '/USER_MINOR_VERSION/ { print $$3}' ${USER_ROOT}/include/version.h)
BETA_VERSION := $(shell awk '/USER_BETA_VERSION/ { print $$3}' ${USER_ROOT}/include/version.h)
VERSION = $(MAJOR_VERSION).$(MINOR_VERSION).$(BETA_VERSION)
#######################################################################################
#######################################################################################

ifeq (1,$(debug))
TARNAME=vbng_debug-$(VERSION)
else
TARNAME=vbng_release-$(VERSION)
endif

all: check_dir $(buidlall)
	chmod a+x $(USER_ROOT)/make/add_rel_link.sh
	make -C $(USER_ROOT) -f WFNOSMakefile
	cp -Lf $(USER_ROOT)/bin/* $(PRJDIR)/$(PROJECT_NAME)/bin/
	cp -Lf $(USER_ROOT)/libs/* $(PRJDIR)/$(PROJECT_NAME)/libs/
	cp -rf $(USER_ROOT)/vbng_app_map.xml $(PRJDIR)/$(PROJECT_NAME)/scripts/
	cp -rf $(USER_ROOT)/LICENSE $(PRJDIR)/$(PROJECT_NAME)/
	cp -rf $(USER_ROOT)/README $(PRJDIR)/$(PROJECT_NAME)/
	cp -rf $(USER_ROOT)/AUTHORS $(PRJDIR)/$(PROJECT_NAME)/
	cp -rf $(USER_ROOT)/Documents $(PRJDIR)/$(PROJECT_NAME)/
	cp -rf $(USER_ROOT)/include/version.h $(PRJDIR)/$(PROJECT_NAME)/include/
	echo "Packing " $(TARNAME) "..."
	tar jcvf $(TARNAME).tar.bz2 -C $(PRJDIR) $(PROJECT_NAME)    
	@echo "build vbng done!"
	@echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!SUCCESS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
check_dir: 
	echo $(PRJDIR)
	@test -d $(PRJDIR)/$(PROJECT_NAME) || mkdir -p $(PRJDIR)/$(PROJECT_NAME)
	@test -d $(PRJDIR)/$(PROJECT_NAME)/bin || mkdir -p $(PRJDIR)/$(PROJECT_NAME)/bin
	@test -d $(PRJDIR)/$(PROJECT_NAME)/libs || mkdir -p $(PRJDIR)/$(PROJECT_NAME)/libs
	@test -d $(PRJDIR)/$(PROJECT_NAME)/conf || mkdir -p $(PRJDIR)/$(PROJECT_NAME)/conf
	@test -d $(PRJDIR)/$(PROJECT_NAME)/include || mkdir -p $(PRJDIR)/$(PROJECT_NAME)/include
	@test -d $(PRJDIR)/$(PROJECT_NAME)/src || mkdir -p $(PRJDIR)/$(PROJECT_NAME)/src
	@test -d $(PRJDIR)/$(PROJECT_NAME)/scripts || mkdir -p $(PRJDIR)/$(PROJECT_NAME)/scripts
#################################################################################
clean:
	make -C $(USER_ROOT) -f WFNOSMakefile realclean
	rm -rf $(TARNAME).tar.bz2
	rm -rf $(PRJDIR)/$(PROJECT_NAME)
	
