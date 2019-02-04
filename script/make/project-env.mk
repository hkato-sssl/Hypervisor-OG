SRCS_FILE := .srcs
SRCDIRS_FILE := .srcdirs

VARIANT ?= Debug

INC_OPS := $(addprefix -I,$(INCS))

ifeq ($(JOBS),)
JOBS := 1
endif

ifdef O
	OUTDIR := $(O)
else
	OUTDIR := $(VARIANT)
endif

