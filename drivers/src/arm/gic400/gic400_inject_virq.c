/*
 * arm/gic400/gic400_inject_virq.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/aarch64.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/device/gic400.h"
#include "gic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static int free_entry(struct gic400 *gic)
{
    int no;
    uint32_t d;

    d = gic400_read_virtif_control(gic, GICH_ELSR0);
    if (d != 0) {
        no = 31 - (int)aarch64_clz(d);
    } else {
        no = -1;
    }

    return no;
}

static errno_t inject_virq(struct gic400 *gic, const struct gic400_virq *virq, int16_t no)
{
    uint32_t d;

    d = virq->virtual_id & BITS(9, 0);
    d |= (virq->physical_id & BITS(9, 0)) << 10;
    d |= (virq->priority & BITS(4, 0)) << 23;
    d |= BITS(31, 30);      /* HW, Grp1 */

    gic400_write_virtif_control(gic, GICH_LR(no), d);

    return SUCCESS;
}

errno_t gic400_inject_virq(struct gic400 *gic, const struct gic400_virq *virq)
{
    errno_t ret;
    int no;

    gic400_lock(gic);

    no = free_entry(gic);
    if (no >= 0) {
        ret = inject_virq(gic, virq, (uint16_t)no);
    } else {
        ret = -EBUSY;
    }

    gic400_unlock(gic);

    return ret;
}

