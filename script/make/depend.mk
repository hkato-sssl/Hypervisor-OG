.PHONY: depend

define template_depend_c
depend: $(OUTDIR)/$(1).d
$(OUTDIR)/$(1).d: $(1)
	$$(CC) -MM -MP -MT $$@ $$(INC_OPS) $$< >$$@

endef

define template_depend_asm
depend: $(OUTDIR)/$(1).d
$(OUTDIR)/$(1).d: $(1)
	$$(CC) -MM -MP -MT $$@ $$(INC_OPS) $$(ASFLAGS) $(1) >$$@

endef

define create_depends
$(foreach SRC,$(filter %.c,$(1)),$(call template_depend_c,$(SRC)))
$(foreach SRC,$(filter %.S,$(1)),$(call template_depend_asm,$(SRC)))

endef

srcs = $(shell cat $(OUTDIR)/$(1)/$(SRCS_FILE))

SRCDIRS := $(shell cat $(OUTDIR)/$(SRCDIRS_FILE))
$(eval $(foreach DIR,$(SRCDIRS),$(call create_depends,$(call srcs,$(DIR)))))
