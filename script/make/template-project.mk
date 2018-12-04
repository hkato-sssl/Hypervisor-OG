export

.PHONY: target prepare depend

WORKSPACE_LOC = $(abspath $(PROJ_HOME)/.)
MAKE_PATH = $(WORKSPACE_LOC)/script/make

PROJ_HOME := $(CURDIR)

include $(MAKE_PATH)/project-env.mk

TARGET ?= $(notdir $(PROJ_HOME))

$(OUTDIR)/$(TARGET): prepare build

prepare:
	make -f $(MAKE_PATH)/prepare.mk

build: prepare
	make -f $(MAKE_PATH)/build.mk build

depend:
	make -f $(MAKE_PATH)/build.mk depend
