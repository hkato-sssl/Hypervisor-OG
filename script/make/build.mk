.PHONY: build build

build: $(OUTDIR)/$(TARGET)

#### build ####

define template_build_c
OBJS += $(2)
$$(OUTDIR)/$$(TARGET): $(2)
$(2): $(1)
	$$(CC) -c $$(INC_OPS) $(CFLAGS) $(ADDED_CFLAGS) $$< -o $$@

endef

define template_build_asm
OBJS += $(2)
$$(OUTDIR)/$$(TARGET): $(2)
$(2): $(1)
	$$(CC) -c $$(INC_OPS) $$(ASFLAGS) $(1) -o $$@

endef

define create_builds
$(foreach SRC,$(filter %.c,$(1)),$(call template_build_c,$(SRC),$(OUTDIR)/$(SRC:%.c=%.o)))
$(foreach SRC,$(filter %.S,$(1)),$(call template_build_asm,$(SRC),$(OUTDIR)/$(SRC:%.S=%.o)))

endef

srcs = $(shell cat $(OUTDIR)/$(1)/$(SRCS_FILE))

SRCDIRS := $(shell cat $(OUTDIR)/$(SRCDIRS_FILE))
$(eval $(foreach DIR,$(SRCDIRS),$(call create_builds,$(call srcs,$(DIR)))))

-include $(shell find $(OUTDIR) -type f -name "*.d")

#### link or archive ####

ifeq ($(suffix $(TARGET)),.a)
$(OUTDIR)/$(TARGET):
	echo $(ARFLAGS) $@ >$(AROPS_FILE)
	find $(OUTDIR) -name .srcs | xargs cat | sed -e s/\\.[csS]$$/\\.o/ -e s/^/$(OUTDIR)\\// >> $(AROPS_FILE)
	$(AR) @$(AROPS_FILE)
else
ifeq ($(suffix $(TARGET)),.elf)
$(OUTDIR)/$(TARGET): $(LD_SCRIPT_FILE)
	echo -o $@ > $(LDOPS_FILE)
	echo -T config/memory.lds >>$(LDOPS_FILE)
	echo $(LDFLAGS) >> $(LDOPS_FILE)
	find $(OUTDIR) -name .srcs | xargs cat | sed -e s/\\.[csS]$$/\\.o/ -e s/^/$(OUTDIR)\\// >> $(LDOPS_FILE)
	$(CC) -nostdlib -Wl,@$(LDOPS_FILE)
else
$(error Unknown suffix of $$(TARGET).)
endif
endif
