export

include script/make/command.mk
include script/make/project-env.mk

.PHONY: prepare depend default

prepare:
	make -f script/make/prepare.mk

depend:
	make -f script/make/depend.mk

