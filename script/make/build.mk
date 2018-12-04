.PHONY: depend

define template_depend_desc
depend: $(OUTDIR)/$1.d
$(OUTDIR)/$(1).d: $(1)
	$$(CC) -MM -MP -MT $$@ $$(INC_OPS) $(1) >$$@

endef

cread_depends = $(foreach SRC,$(shell cat $(OUTDIR)/$(1)/$(SRCS_FILE)),$(call template_depend_desc,$(SRC)))

DEP_SRCDIRS = $(shell cat $(OUTDIR)/$(SRCDIRS_FILE))
$(eval $(foreach DIR,$(DEP_SRCDIRS),$(call cread_depends,$(DIR))))

