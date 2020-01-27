/*
 * vm/vm_register_region_trap.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/system.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t map_region_trap(struct vm *vm, struct vm_region_trap *region)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    memset(&attr, 0, sizeof(attr));

    attr.xn = 1;
    attr.af = 1;
    attr.sh = STAGE2_SH_OSH;
    attr.memattr = STAGE2_MEMATTR_DEVICE_GRE;

    if (region->condition.read) {
        if (region->condition.write) {
            attr.s2ap = STAGE2_S2AP_NONE;
        } else {
            attr.s2ap = STAGE2_S2AP_WO;
        }
    } else {
        attr.s2ap = STAGE2_S2AP_RO;
    }

    ret = aarch64_stage2_map(vm->stage2, (void *)region->ipa, (void *)region->pa, region->size, &attr);

    return ret;
}

static errno_t append_region_trap(struct vm_region_trap *head, struct vm_region_trap *region)
{
    errno_t ret;
    struct vm_region_trap *p;

    ret = -EINVAL;
    for (p = head; p != region; p = p->next) {
        if (p->next == NULL) {
            p->next = region;
            region->next = NULL;
            ret = SUCCESS;
            break;
        }
    }

    return ret;
}

static errno_t register_region_trap(struct vm *vm, struct vm_region_trap *region)
{
    errno_t ret;
    struct vm_region_trap *head;

    head = vm->emulator.trap.memory_region;
    if (head != NULL) {
        ret = append_region_trap(head, region);
    } else {
        vm->emulator.trap.memory_region = region; 
        region->next = NULL;
        ret = SUCCESS;
    }

    return ret;
}

static bool is_valid_condition(const struct vm_region_trap *region)
{
    bool valid;

    if (region->condition.read || region->condition.write) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

static bool is_valid_parameter(const struct vm *vm, const struct vm_region_trap *region)
{
    bool valid;
    errno_t ret;

    ret = system_validate_stack_region(region, sizeof(*region));
    if (ret != SUCCESS) {
        if (is_valid_condition(region) &&
            (region->size > 0) && ((region->size % 4096) == 0)) {
            valid = true;
        } else {
            valid = false;
        }
    } else {
        valid = false;
    }

    return valid;
}

errno_t vm_register_region_trap(struct vm *vm, struct vm_region_trap *region)
{
    errno_t ret;

    if (is_valid_parameter(vm, region)) {
        ret = register_region_trap(vm, region);
        if (ret == SUCCESS) {
            ret = map_region_trap(vm, region);
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}
