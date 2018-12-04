.PHONY: depend build

build: depend

define template_depend_desc
depend: $(OUTDIR)/$(1).d
$(OUTDIR)/$(1).d: $(1)
ifeq ($(suffix $(1)),.c)
	$$(CC) -MM -MP -MT $$@ $$(INC_OPS) $(1) >$$@
else
	$$(CC) -MM -MP -MT $$@ $$(ASFLAGS) $$(INC_OPS) $(1) >$$@
endif

endef

define template_depend_desc2
depend: $(OUTDIR)/$(1).d
$(OUTDIR)/$(1).d: $(1)
ifeq ($(suffix $(1)),.c)
	$$(CC) -MM -MP -MT $$@ $$(INC_OPS) $(1) >$$@
else
	$$(CC) -MM -MP -MT $$@ $$(ASFLAGS) $$(INC_OPS) $(1) >$$@
endif

endef

srcs = $(shell cat $(OUTDIR)/$(1)/$(SRCS_FILE))
create_depends = $(foreach SRC,$(call srcs,$(1)),$(call template_depend_desc,$(SRC)))

define template_build_desc
ifeq ($(suffix $(1)),.c)
build: $(OUTDIR)/$(1:.c=.o)
$(OUTDIR)/$(1:.c=.o): $(1)
	$$(CC) -c $$(INC_OPS) $$< -o $$@
else
build: $(OUTDIR)/$(1:.S=.o)
$(OUTDIR)/$(1:.S=.o): $(1)
	$$(CC) -c $$(ASFLAGS) $$(INC_OPS) $$< -o $$@
endif

endef

create_builds = $(foreach SRC,$(call srcs,$(1)),$(call template_build_desc,$(SRC)))


SRCDIRS := $(shell cat $(OUTDIR)/$(SRCDIRS_FILE))
$(eval $(foreach DIR,$(SRCDIRS),$(call create_depends,$(DIR))))
$(eval $(foreach DIR,$(SRCDIRS),$(call create_builds,$(DIR))))
