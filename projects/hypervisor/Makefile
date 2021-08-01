export

.PHONY: build prepare depend

PROJ_HOME := $(CURDIR)
WORKSPACE_LOC ?= $(abspath $(PROJ_HOME)/../)
MAKE_PATH = $(WORKSPACE_LOC)/script/make

WORKSPACE_LIBRARY_PATH := $(WORKSPACE_LOC)/build/lib
WORKSPACE_LIBRARY := libs drivers

CROSS_COMPILE ?= aarch64-elf-
#TARGET_ARCH := aarch64
#TARGET_BOARD := zcu104
#LOAD_TYPE := ram

SRC_BASES := src
INCS := inc

include $(MAKE_PATH)/project-env.mk
include $(MAKE_PATH)/command.mk

CFLAGS += -DTRACE_ERRNO

# Depending on the target you want to build,
# uncomment either of the following two sentences.
#TARGET ?= $(notdir $(PROJ_HOME)).elf	# an executable
TARGET ?= $(notdir $(PROJ_HOME)).a	# an archive
$(if $(TARGET),,$(error TARGET is undefined))

$(OUTDIR)/$(TARGET): install

prepare:
	make -f $(MAKE_PATH)/prepare.mk

depend: prepare
	make -f $(MAKE_PATH)/depend.mk -j$(JOBS)

build: depend
	make -f $(MAKE_PATH)/build.mk -j$(JOBS)

ifeq ($(suffix $(TARGET)),.a)
.PHONY: install
install: build
	@mkdir -p $(WORKSPACE_LIBRARY_PATH)
	@cp -v $(OUTDIR)/$(TARGET) $(WORKSPACE_LIBRARY_PATH)
endif

