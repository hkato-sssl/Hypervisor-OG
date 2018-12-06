.PHONY: build build

build: $(OUTDIR)/$(TARGET)

#### build ####

define template_build_c
OBJS += $(2)
$(2): $(1)
	$$(CC) -c $$(INC_OPS) $$< -o $$@

endef

define template_build_asm
OBJS += $(2)
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
$(OUTDIR)/$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
else
$(OUTDIR)/$(TARGET): $(OBJS)
	cat $(OBJS) >$@
endif
