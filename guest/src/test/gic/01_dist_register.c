/*
 * test/gic/01_dist_register.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：32-bit アクセス試験
 * GIC-400 Distributorのレジスタを32-bit単位でREAD/WRITEを実施
 * 対象レジスタはGICD_SGIR以外全て
 */

#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"

/* defines */

#define PRINT_TITLE()   print_title(__func__)

#define ALL_F           BITS(31, 0)

/* types */

/* prototypes */

/* variables */

extern struct gic400 gic;

static void *dist;

/* functions */

static uint32_t readw(uint32_t reg)
{
    uint32_t d;

    d = REG_READ32(dist, reg);

    return d;
}

static void writew(uint32_t reg, uint32_t d)
{
    REG_WRITE32(dist, reg, d);
}

static void print_title(const char *func)
{
    printk("\n<%s>\n", func);
}

static void test_ro_register(uint32_t reg)
{
    printk(" read: 0x%08x\n", readw(reg));
    printk("write: 0x00000000\n");
    writew(reg, 0);
    printk(" read: 0x%08x\n", readw(reg));
    printk("write: 0xffffffff\n");
    writew(reg, ALL_F);
    printk(" read: 0x%08x\n", readw(reg));
}

static void dump(uint32_t reg, int n)
{
    int i;

    for (i = 0; i < n; ++i) {
        printk("%-3d: 0x%08x\n", i, readw(reg));
        reg += 4;
    }
}

static void write_array(uint32_t reg, uint32_t d, int n)
{
    int i;

    for (i = 0; i < n; ++i) {
        writew(reg, d);
        reg += 4;
    }
}

static void test_array_register(uint32_t reg, int n)
{
    printk("write ALL 0\n");
    write_array(reg, 0, n);
    dump(reg, n);

    printk("write ALL F\n");
    write_array(reg, ALL_F, n);
    dump(reg, n);
}

static void test_bit_register(uint32_t reg)
{
    test_array_register(reg, NR_GIC400_INTERRUPTS / 32);
}

static void test_byte_register(uint32_t reg)
{
    test_array_register(reg, NR_GIC400_INTERRUPTS / 4);
}

static void test_dbit_register(uint32_t reg)
{
    test_array_register(reg, NR_GIC400_INTERRUPTS / 16);
}

static void test_GICD_CTLR(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_CTLR);
}

static void test_GICD_TYPER(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_TYPER);
}

static void test_GICD_IIDR(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_IIDR);
}

static void test_GICD_IGROUPR(void)
{
    PRINT_TITLE();
    test_bit_register(GICD_IGROUPR(0));
}

static void test_GICD_ISENABLER(void)
{
    PRINT_TITLE();
    test_bit_register(GICD_ISENABLER(0));
}

static void test_GICD_ICENABLER(void)
{
    PRINT_TITLE();
    test_bit_register(GICD_ICENABLER(0));
}

static void test_GICD_ISPENDR(void)
{
    PRINT_TITLE();
    test_bit_register(GICD_ISPENDR(0));
}

static void test_GICD_ICPENDR(void)
{
    PRINT_TITLE();
    test_bit_register(GICD_ICPENDR(0));
}

static void test_GICD_ISACTIVER(void)
{
    PRINT_TITLE();
    test_bit_register(GICD_ISACTIVER(0));
}

static void test_GICD_ICACTIVER(void)
{
    PRINT_TITLE();
    test_bit_register(GICD_ICACTIVER(0));
}

static void test_GICD_IPRIORITYR(void)
{
    PRINT_TITLE();
    test_byte_register(GICD_IPRIORITYR(0));
}

static void test_GICD_ITARGETSR(void)
{
    PRINT_TITLE();
    test_byte_register(GICD_ITARGETSR(0));
}

static void test_GICD_ICFGR(void)
{
    PRINT_TITLE();
    test_dbit_register(GICD_ICFGR(0));
}

static void test_GICD_PPISR(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PPISR);
}

static void test_GICD_SPISR(void)
{
    PRINT_TITLE();
    test_array_register(GICD_SPISR(0), NR_GIC400_SPIS / 32);
}

static void test_GICD_CPENDSGIR(void)
{
    PRINT_TITLE();
    test_array_register(GICD_CPENDSGIR(0), 4);
}

static void test_GICD_SPENDSGIR(void)
{
    PRINT_TITLE();
    test_array_register(GICD_SPENDSGIR(0), 4);
}

static void test_GICD_PIDR4(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR4);
}

static void test_GICD_PIDR5(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR5);
}

static void test_GICD_PIDR6(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR6);
}

static void test_GICD_PIDR7(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR7);
}

static void test_GICD_PIDR0(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR0);
}

static void test_GICD_PIDR1(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR1);
}

static void test_GICD_PIDR2(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR2);
}

static void test_GICD_PIDR3(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_PIDR3);
}

static void test_GICD_CIDR0(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_CIDR0);
}

static void test_GICD_CIDR1(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_CIDR1);
}

static void test_GICD_CIDR2(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_CIDR2);
}

static void test_GICD_CIDR3(void)
{
    PRINT_TITLE();
    test_ro_register(GICD_CIDR3);
}

void guest_test_gic_01(void)
{
    printk("<%s>\n", __func__);
    
    dist = gic400_distributor_register_base(&gic);
    printk("GICD Base: %p\n", dist);

    test_GICD_CTLR();
    test_GICD_TYPER();
    test_GICD_IIDR();
    test_GICD_IGROUPR();
    test_GICD_ISENABLER();
    test_GICD_ICENABLER();
    test_GICD_ISPENDR();
    test_GICD_ICPENDR();
    test_GICD_ISACTIVER();
    test_GICD_ICACTIVER();
    test_GICD_IPRIORITYR();
    test_GICD_ITARGETSR();
    test_GICD_ICFGR();
    test_GICD_PPISR();
    test_GICD_SPISR();
    test_GICD_CPENDSGIR();
    test_GICD_SPENDSGIR();
    test_GICD_PIDR4();
    test_GICD_PIDR5();
    test_GICD_PIDR6();
    test_GICD_PIDR7();
    test_GICD_PIDR0();
    test_GICD_PIDR1();
    test_GICD_PIDR2();
    test_GICD_PIDR3();
    test_GICD_CIDR0();
    test_GICD_CIDR1();
    test_GICD_CIDR2();
    test_GICD_CIDR3();

    printk("\nDone.\n");
}

