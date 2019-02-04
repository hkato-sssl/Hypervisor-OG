SRCS_FILE := .srcs
SRCDIRS_FILE := .srcdirs

VARIANT ?= Debug

INC_OPS := $(addprefix -I,$(INCS))
ifdef WORKSPACE_LIBRARY
INC_OPS += $(addprefix -I$(WORKSPACE_LOC)/,$(addsuffix /inc,$(WORKSPACE_LIBRARY)))
endif

ifeq ($(JOBS),)
JOBS := 1
endif

ifdef O
	OUTDIR := $(O)
else
	OUTDIR := $(VARIANT)
endif

