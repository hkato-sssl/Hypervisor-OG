/*
 * vm/vm_register_region_trap.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/slist.h"
#include "lib/system.h"
#include "lib/system/memio.h"
#include "lib/system/errno.h"
#include "hypervisor/mmu.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t create_stage2_attribute(struct aarch64_stage2_attr *attr, struct vm_region_trap *trap)
{
    errno_t ret;
    uint8_t sh;
    uint8_t mt;

    ret = hyp_mmu_stage2_shareability(&sh, trap->memory.shareability);

    if (ret == SUCCESS) {
        ret = hyp_mmu_stage2_memory_type(&mt, trap->memory.type);
    }

    if (ret == SUCCESS) {
        memset(attr, 0, sizeof(*attr));

        attr->xn = 1;
        attr->af = 1;
        attr->sh = sh;
        attr->memattr = mt;
        attr->smmu.wacfg = SMMU_WACFG_WA;
        attr->smmu.racfg = SMMU_RACFG_RA;

        if (trap->condition.read) {
            if (trap->condition.write) {
                attr->s2ap = STAGE2_S2AP_NONE;
            } else {
                attr->s2ap = STAGE2_S2AP_WO;
            }
        } else {
            attr->s2ap = STAGE2_S2AP_RO;
        }
    }

    return ret;
}

static errno_t map_region_trap(struct vm *vm, struct vm_region_trap *trap)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    ret = create_stage2_attribute(&attr, trap);
    if (ret == SUCCESS) {
        ret = aarch64_stage2_map(&(vm->stage2), (void *)trap->memory.ipa, (void *)trap->memory.pa, trap->memory.size, &attr);
    }

    return ret;
}

static errno_t register_region_trap(struct vm *vm, struct vm_region_trap *trap)
{
    errno_t ret;

    trap->node.element = trap;

    vm_lock(vm);
    ret = slist_append(&(vm->emulator.trap.memory_region), &(trap->node));
    vm_unlock(vm);

    return ret;
}

static bool is_valid_condition(const struct vm_region_trap *trap)
{
    bool ret;

    if (trap->condition.read || trap->condition.write) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static errno_t validate_parameters(const struct vm *vm, const struct vm_region_trap *trap)
{
    errno_t ret;

    if (system_test_valid_stack_region(trap, sizeof(*trap))) {
        ret = -EFAULT;
    } else if (! is_valid_condition(trap)) {
        ret = -EINVAL;
    } else if (trap->memory.size == 0) {
        ret = -EINVAL;
    } else if (! IS_ALIGNED(trap->memory.size, 4096)) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t vm_register_region_trap(struct vm *vm, struct vm_region_trap *trap)
{
    errno_t ret;

    ret = validate_parameters(vm, trap);
    if (ret == SUCCESS) {
        ret = register_region_trap(vm, trap);
        if (ret == SUCCESS) {
            ret = map_region_trap(vm, trap);
        }
    }

    return ret;
}

