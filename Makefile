export

include script/make/command.mk
include script/make/project-env.mk

.PHONY: prepare depend all

all:	prepare depend

prepare:
	make -f script/make/prepare.mk

depend:
	make -f script/make/depend.mk


