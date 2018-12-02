SRC_BASE := src
INC_BASE := inc

SRCS_FILE := .srcs
SRCDIRS_FILE := .srcdirs

VARIANT ?= Debug

PROJ_HOME := $(CURDIR)
SRCDIR := $(PROJ_HOME)/$(SRC_BASE)

INC_PATHS += $(INC_BASE)
INC_OPS := $(addprefix -I,$(INC_PATHS))

ifdef O
	OUTDIR := $(O)
else
	OUTDIR := $(VARIANT)
endif

