/*
 * test/insn/03.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：LDR Xn|Wn, literal形式の解析結果確認
 *
 * Hypervisorがエミュレーションをサポートするメモリアクセス命令を実行した
 * 際の解析結果を確認する。実行内容に意味は無い。
 * メモリアクセスによるトラップを発生させる為にアクセスするアドレスは
 * Hypervisor領域を指定する。
 *
 * 本コードではguest/src/test/insn/03_*を実行する為の実行環境を用意する。
 * アクセスをトラップ後は何も実行せず次の命令から実行を継続する。
 *
 * テストの詳細はGuest側のテストコードに記述している。
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vpc_register.h"
#include "hypervisor/vm.h"
#include "hypervisor/mmu.h"
#include "hypervisor/emulator/insn.h"

/* defines */

#define TRAP_IPA        0x40000000

/* types */

/* prototypes */

/* variables */

static char buff[4096] __attribute__((aligned(4096)));

/* functions */

static errno_t test_handler(struct insn *insn, void *arg)
{
    printk("%s()\n", __func__);
    printk("PC=%p\n", insn->vpc->regs[VPC_PC]);
    insn_print(insn);
    vpc_update_pc(insn->vpc);
    return SUCCESS;
}

static void init(struct vm *vm)
{
    errno_t ret;
    static struct vm_region_trap trap;

    memset(&trap, 0, sizeof(trap));
    trap.condition.read = true;
    trap.condition.write = true;
    trap.memory.ipa = (uint64_t)TRAP_IPA;
    trap.memory.pa = (uint64_t)buff;
    trap.memory.size = sizeof(buff);
    trap.memory.shareability = HYP_MMU_SH_ISH;
    trap.memory.type = HYP_MMU_MT_NORMAL_WB;
    trap.emulator.arg = NULL;
    trap.emulator.handler = (vpc_emulator_t)test_handler;
    ret = vm_register_region_trap(vm, &trap);
    printk("vm_register_region_trap() -> %d\n", ret);
}

void test_insn_03(struct vm *vm)
{
    printk("%s()\n", __func__);
    init(vm);
}

