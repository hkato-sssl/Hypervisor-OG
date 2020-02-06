/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_initialize.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/arm/gic400.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern const struct soc_ops xilinx_mpsoc_ops;

/* functions */

static errno_t map_ram_region(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.xn = 0;
    attr.af = 1;
    attr.sh = STAGE2_SH_ISH;
    attr.s2ap = STAGE2_S2AP_RW;
    attr.memattr = STAGE2_MEMATTR_NORMAL_WB;
    ret = aarch64_stage2_map(&(chip->soc.stage2), (void *)(chip_config->ram.ipa), (void *)(chip_config->ram.pa), chip_config->ram.size, &attr);

    return ret;
}

static errno_t init_vgic400(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct vgic400_configuration config;

    memset(&config, 0, sizeof(config));
    config.vm = &(chip->soc.vm);
    config.gic = chip_config->gic;
    config.base.virtif_control = (void *)GIC400H_BASE;
    config.base.virtual_cpuif = (void *)GIC400V_BASE;
    config.boolean.trap_cpuif = true;
    ret = vgic400_initialize(&(chip->vgic400), &config);
    if ((ret == SUCCESS) && (chip_config->ram.size > 0)) {
        ret = map_ram_region(chip, chip_config);
    }

    return ret;
}

static errno_t init_vm(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct vm_configuration config;

    memset(&config, 0, sizeof(config));
    config.soc = &(chip->soc);
    config.nr_procs = chip_config->nr_procs;
    config.stage2 = &(chip->soc.stage2);
    ret = vm_initialize(&(chip->soc.vm), &config);
    if (ret == SUCCESS) {
        ret = init_vgic400(chip, chip_config);
    }

    return ret;
}

static errno_t init_stage2(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct aarch64_stage2_configuration config;

    memset(&config, 0, sizeof(config));
    config.base.type = AARCH64_MMU_STAGE2;
    config.base.granule = AARCH64_MMU_4KB_GRANULE;
    config.base.pool = chip_config->stage2.pool;
    config.vmid = chip_config->vmid;
    config.pa_range = STAGE2_PA_RANGE_40BITS;
    config.start_level = 1;
    config.sh = STAGE2_SH_ISH;
    config.orgn = STAGE2_RGN_NORMAL_WBWA;
    config.irgn = STAGE2_RGN_NORMAL_WBWA;
    config.first_table = chip_config->stage2.level1_table;
    ret = aarch64_stage2_init(&(chip->soc.stage2), &config);
    if (ret == SUCCESS) {
        ret = init_vm(chip, chip_config);
    }

    return ret;
}

static errno_t init_xilinx_mpsoc(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;

    chip->ram.ipa = chip_config->ram.ipa;
    chip->ram.size = chip_config->ram.size;

    ret = init_stage2(chip, chip_config);

    return ret;
}

static errno_t init_soc(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct soc_configuration config;

    memset(&config, 0, sizeof(config));
    config.chip = chip;
    config.ops = &xilinx_mpsoc_ops;
    config.nr_procs = chip_config->nr_procs;

    ret = soc_initialize(&(chip->soc), &config);
    if (ret == SUCCESS) {
        ret = init_xilinx_mpsoc(chip, chip_config);
    }

    return ret;
}

errno_t xilinx_mpsoc_initialize(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *config)
{
    errno_t ret;

    if ((chip != NULL) && (config != NULL)) {
        memset(chip, 0, sizeof(*chip));
        ret = init_soc(chip, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

