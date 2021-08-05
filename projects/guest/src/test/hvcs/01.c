/*
 * test/hvcs/01.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容
 * Linux driverで動作確認する前に実施する事前確認が目的。
 * 基本的なAPI呼び出しと送受信、割り込みの状態を確認する。
 * 割り込みはポーリングで確認するのみ。事前確認なので、詳細な
 * 状態確認は行わない。
 */

#include "driver/arm/gic400.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* defines */

#define INT_EP0 130
#define INT_EP1 131

/* types */

/* prototypes */

errno_t test_hvcs_send(int ifno, uint64_t *buff);
errno_t test_hvcs_recv(int ifno, uint64_t *buff);

/* variables */

extern struct gic400 gic;

/* functions */

static void init_interrupts(void)
{
    errno_t ret;
    struct gic400_interrupt_configuration config;

    memset(&config, 0, sizeof(config));
    config.targets = 1;
    config.priority = 1;
    config.flag.edge = 1;
    ret = gic400_configure_interrupt(&gic, INT_EP0, &config);
    printk("gic400_configure_interrupt(INT_EP0) -> %d\n", ret);
    gic400_enable_interrupt(&gic, INT_EP0);
    ret = gic400_configure_interrupt(&gic, INT_EP1, &config);
    printk("gic400_configure_interrupt(INT_EP1) -> %d\n", ret);
    gic400_enable_interrupt(&gic, INT_EP1);
    gic400_set_priority_mask(&gic, 0xff);
}

static void ack_interrupt(void)
{
    uint32_t iar;

    iar = gic400_ack(&gic);
    printk("iar = %08x\n", iar);
    if (iar < NR_GIC400_INTERRUPTS) {
        gic400_eoi(&gic, iar);
    }
}

static void dump_buff(const uint64_t *buff)
{
    int i;

    for (i = 0; i < 16; i += 2) {
        printk("%02x: %016lx %016lx\n", i, buff[i], buff[i + 1]);
    }
    printk("\n");
}

void test_hvcs_01_p2p(void)
{
    errno_t ret;
    uint64_t buff[16];

    init_interrupts();

    memset(buff, 0, sizeof(buff));
    buff[0] = 0x1122334455667788ULL;
    buff[15] = 0xaabbccddeeff9988ULL;
    ret = test_hvcs_send(0, buff);
    printk("test_hvcs_send() -> %d\n", ret);
    ack_interrupt();

    memset(buff, 0xff, sizeof(buff));
    ret = test_hvcs_recv(0, buff);
    printk("test_hvcs_recv(0) -> %d\n", ret);
    ack_interrupt();
    dump_buff(buff);

    memset(buff, 0xff, sizeof(buff));
    ret = test_hvcs_recv(1, buff);
    printk("test_hvcs_recv(1) -> %d\n", ret);
    ack_interrupt();
    dump_buff(buff);
}
