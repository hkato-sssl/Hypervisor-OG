export

.PHONY: build prepare depend

PROJ_HOME := $(CURDIR)
WORKSPACE_LOC ?= $(abspath $(PROJ_HOME)/../)
MAKE_PATH = $(WORKSPACE_LOC)/script/make

include $(MAKE_PATH)/command.mk
include $(MAKE_PATH)/project-env.mk

# Depending on the target you want to build,
# uncomment either of the following two sentences.
#TARGET ?= $(notdir $(PROJ_HOME))	# an executable
#TARGET ?= $(notdir $(PROJ_HOME)).a	# an archive
$(if $(TARGET),,$(error TARGET is undefined))

$(OUTDIR)/$(TARGET): build

prepare:
	make -f $(MAKE_PATH)/prepare.mk

depend: prepare
	make -f $(MAKE_PATH)/depend.mk -j$(JOBS)

build: depend
	make -f $(MAKE_PATH)/build.mk -j$(JOBS)
