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
#include "driver/arm/smmu500.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/parameter.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern const struct soc_ops xilinx_mpsoc_ops;

/* functions */

static errno_t create_smmu_streams(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    uint8_t id;
    uint32_t i;
    struct smmu_translation_stream_configuration config;

    memset(&config, 0, sizeof(config));
    config.cbndx = chip->smmu.context_bank;

    for (i = 0; i < chip_config->smmu.nr_streams; ++i) {
        config.stream = *(chip_config->smmu.streams[i]);
        ret = smmu500_create_translation_stream(chip->smmu.device, &id, &config);
        if (ret != SUCCESS) {
            break;
        }

        ret = smmu500_enable(chip->smmu.device, id);
        if (ret != SUCCESS) {
            break;
        }

        chip->smmu.stream_ids[id / 64] |= 1ULL << (id % 64);
    }

    return ret;
}

static errno_t create_smmu_context_bank(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct smmu_context_bank_with_stage2_configuration config;

    memset(&config, 0, sizeof(config));
    config.stage2 = &(chip->soc.vm.stage2);
    config.interrupt_index = 0;
    config.vmid = chip_config->vmid;
    config.flag.interrupt = 1;
    config.flag.fault = chip_config->smmu.flag.fault;
    ret = smmu500_create_context_bank_with_stage2(chip->smmu.device, &(chip->smmu.context_bank), &config);

    return ret;
}

static errno_t map_smmu_space(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;

    if (chip_config->smmu.nr_streams > 0) {
        ret = create_smmu_context_bank(chip, chip_config);
        if (ret == SUCCESS) {
            ret = create_smmu_streams(chip, chip_config);
        }
    } else {
        ret = SUCCESS;      /* no work */
    }

    return ret;
}

static errno_t create_stage2_attribute(struct aarch64_stage2_attr *attr, const struct soc_device *dev)
{
    errno_t ret;

    ret = SUCCESS;
    attr->af = 1;

    if (dev->region.access.flag.exec != 0) {
        attr->xn = 0;
    } else {
        attr->xn = 1;
    }

    if (dev->region.access.flag.read != 0) {
        if (dev->region.access.flag.write != 0) {
            attr->s2ap = STAGE2_S2AP_RW;
        } else {
            attr->s2ap = STAGE2_S2AP_RO;
        }
    } else {
        if (dev->region.access.flag.write != 0) {
            attr->s2ap = STAGE2_S2AP_WO;
        } else {
            attr->s2ap = STAGE2_S2AP_NONE;
        }
    }

    if (dev->region.shareability == SOC_NSH) {
        attr->sh = STAGE2_SH_NSH;
    } else if (dev->region.shareability == SOC_ISH) {
        attr->sh = STAGE2_SH_ISH;
    } else if (dev->region.shareability == SOC_OSH) {
        attr->sh = STAGE2_SH_OSH;
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t map_stage2_space(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)

{
    errno_t ret;
    uint16_t i;
    struct aarch64_stage2_attr attr;
    struct soc_device *dev;

    memset(&attr, 0, sizeof(attr));

    for (i = 0; i < chip_config->nr_devices; ++i) {
        dev = chip_config->devices[i];

        ret = create_stage2_attribute(&attr, dev);
        if (ret != SUCCESS) {
            break;
        }

        ret = aarch64_stage2_map(&(chip->soc.vm.stage2), (void *)(dev->region.ipa), (void *)(dev->region.pa), dev->region.size, &attr);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static errno_t init_virtual_ppis(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    uint32_t i;
    struct vgic400_interrupt_configuration config;

    memset(&config, 0, sizeof(config));
    config.flag.hw = 1;

    for (i = 0; i < chip_config->gic.nr_ppis; ++i) {
        config.virtual_id = chip_config->gic.ppis[i].virtual_id;
        config.physical_id = chip_config->gic.ppis[i].physical_id;
        ret = vgic400_configure_interrupt(&(chip->vgic400), &config);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static errno_t init_virtual_interrupts(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    uint32_t i;
    uint32_t j;
    struct soc_device *dev;
    struct vgic400_interrupt_configuration config;

    memset(&config, 0, sizeof(config));
    config.flag.hw = 1;

    ret = SUCCESS;
    for (i = 0; i < chip_config->nr_devices; ++i) {
        dev = chip_config->devices[i];
        for (j = 0; j < dev->nr_irqs; ++j) {
            config.virtual_id = dev->irqs[j];
            config.physical_id = dev->irqs[j];
            ret = vgic400_configure_interrupt(&(chip->vgic400), &config);
            if (ret != SUCCESS) {
                return ret;
            }
        }
    }

    return ret;
}

static errno_t init_vgic400(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct vgic400_configuration config;

    memset(&config, 0, sizeof(config));

    config.vm = &(chip->soc.vm);
    config.gic = chip_config->gic.device;
    config.base.virtif_control = (void *)REG_GIC400H;
    config.base.virtual_cpuif = (void *)REG_GIC400V;
    config.boolean.trap_cpuif = true;

    ret = vgic400_initialize(&(chip->vgic400), &config);

    if ((ret == SUCCESS) && (chip_config->nr_devices > 0)) {
        ret = init_virtual_interrupts(chip, chip_config);
    }

    if ((ret == SUCCESS) && (chip_config->gic.nr_ppis > 0)) {
        ret = init_virtual_ppis(chip, chip_config);
    }

    return ret;
}

static errno_t init_vpc(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    uint8_t i;
    struct vpc_configuration config;

    memset(&config, 0, sizeof(config));

    config.vm = &(chip->soc.vm);
    if (chip_config->vpc.hook ==  NULL) {
        config.hook = xilinx_mpsoc_default_vpc_hook();
    } else {
        config.hook = chip_config->vpc.hook;
    }
    if (chip_config->vpc.ops ==  NULL) {
        config.exception.ops = xilinx_mpsoc_default_vpc_exception_ops();
    } else {
        config.exception.ops = chip_config->vpc.ops;
    }

    for (i = 0; i < chip_config->nr_procs; ++i) {
        config.regs = chip_config->vpc.register_arrays[i];
        config.proc_no = i;
        ret = soc_initialize_vpc(&(chip->soc), chip_config->vpc.vpcs[i], &config);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static errno_t init_soc(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *chip_config)
{
    errno_t ret;
    struct soc_configuration config;
    struct aarch64_stage2_configuration stage2;

    memset(&config, 0, sizeof(config));
    memset(&stage2, 0, sizeof(stage2));

    stage2.base.type = AARCH64_MMU_STAGE2;
    stage2.base.granule = AARCH64_MMU_4KB_GRANULE;
    stage2.base.pool = chip_config->stage2.pool;
    stage2.vmid = chip_config->vmid;
    stage2.pa_range = STAGE2_PA_RANGE_40BITS;
    stage2.start_level = 1;
    stage2.sh = STAGE2_SH_ISH;
    stage2.orgn = STAGE2_RGN_NORMAL_WBWA;
    stage2.irgn = STAGE2_RGN_NORMAL_WBWA;
    stage2.first_table = chip_config->stage2.level1_table;

    config.chip = chip;
    config.ops = &xilinx_mpsoc_ops;
    config.nr_procs = chip_config->nr_procs;
    config.stage2 = &stage2;

    ret = soc_initialize(&(chip->soc), &config);

    return ret;
}

static errno_t initialize(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *config)
{
    errno_t ret;

    memset(chip, 0, sizeof(*chip));

    chip->smmu.device = config->smmu.device;

    ret = init_soc(chip, config);

    if (ret == SUCCESS) {
        ret = init_vpc(chip, config);
    }

    if (ret == SUCCESS) {
        ret = init_vgic400(chip, config);
    }

    if (ret == SUCCESS) {
        map_stage2_space(chip, config);
    }

    if (ret == SUCCESS) {
        ret = map_smmu_space(chip, config);
    }

    return ret;
}

static errno_t validate_parameters(const struct xilinx_mpsoc_configuration *config)
{
    errno_t ret;

    if ((config->nr_procs == 0) || (config->nr_procs > NR_XILINX_MPSOC_CPUS)) {
        ret = -EINVAL;
    } else if (config->gic.nr_ppis > NR_GIC400_PPIS) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t xilinx_mpsoc_initialize(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(config);
    if (ret == SUCCESS) {
        ret = initialize(chip, config);
    }

    return ret;
}
