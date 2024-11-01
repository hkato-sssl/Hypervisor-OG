.PHONY: prepare outdir srcdirs srcs

prepare: outdir srcdirs srcs

SRCDIRS = $(shell cat $(OUTDIR)/$(SRCDIRS_FILE))

srcs_list = $(shell mkdir -p $(OUTDIR)/$(1) && find $(1) -maxdepth 1 -type f -name "*.[cS]" -fprint $(OUTDIR)/$(1)/$(SRCS_FILE))

outdir:
	mkdir -p $(OUTDIR)
	rm -f $(OUTDIR)/$(SRCDIRS_FILE)
	find $(OUTDIR) -name $(SRCS_FILE) | xargs rm -f

ifdef EXCLUDE_TEST
srcdirs:
	find $(SRC_BASES) -type d -name test -prune -o -type d -fprint $(OUTDIR)/$(SRCDIRS_FILE)
else
srcdirs:
	find $(SRC_BASES) -type d -fprint $(OUTDIR)/$(SRCDIRS_FILE)
endif

srcs:
	$(foreach DIR,$(SRCDIRS),$(call srcs_list,$(DIR)))

