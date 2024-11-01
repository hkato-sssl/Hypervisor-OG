CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS := -Wall -O0 -g3 -march=armv8-a+nofp -mstrict-align

ASFLAGS :=

ARFLAGS := Drs
AROPS_FILE := $(OUTDIR)/.arops

ifdef TARGET_SYSTEM
LD_SCRIPT_FILE ?= $(WORKSPACE_LOC)/script/ld/$(TARGET_ARCH)-$(TARGET_BOARD)-$(TARGET_SYSTEM)-$(LOAD_TYPE).lds
else
LD_SCRIPT_FILE ?= $(WORKSPACE_LOC)/script/ld/$(TARGET_ARCH)-$(TARGET_BOARD)-$(LOAD_TYPE).lds
endif

ifdef WORKSPACE_LIBRARY
LDFLAGS += --start-group $(addprefix $(WORKSPACE_LIBRARY_PATH)/,$(addsuffix .a,$(WORKSPACE_LIBRARY))) --end-group
endif

ifdef LD_SCRIPT_FILE
LDFLAGS += -T $(LD_SCRIPT_FILE)
endif

LDOPS_FILE := $(OUTDIR)/.ldops
