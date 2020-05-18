/*
 * soc/soc_default_test_executable_region.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t test_executable_region(struct soc *soc, uintptr_t addr, uintptr_t alignment)
{
    errno_t ret;
    uint32_t i;
    struct soc_device *dev;

    /* NOTE:
     * vpcがAArch32tで動作している時は2-byteまたは4-byteの命令長を持つが
     * 本関数では2-byte単位でアドレスの正当性をチェックしている。
     * addrが<PAGE SIZE>-2を指し、addrに配置されている命令が4-byte長の場合、
     * マッピング領域を超えてInstruction Fetchが行われる懸念がある。
     * この様な状況下では前半の2-byteと後半の2-byteは異なるキャッシュライン
     * 上に存在するので前半と後半で異なるメモリトランザクションが発生し、後
     * 半のメモリトランザクションでアクセス違反が検出されると想定しているの
     * で、2-byte単位でのアドレス判定で問題無いと判断する。
     * 加えて、AXI Busの仕様では4KiBの境界を跨ぐバースト転送は許可していない。
     */

    ret = -EPERM;
    for (i = 0; i < soc->nr_devices; ++i) {
        dev = soc->devices[i];
        if (IS_ALIGNED(addr, alignment) && (dev->region.ipa <= addr) && (addr <= (dev->region.ipa + dev->region.size - alignment)) && (dev->region.access.flag.exec != 0)) {
            ret = SUCCESS;
            break;
        }
    }

    return ret;
}

errno_t soc_default_test_executable_region(struct vpc *vpc, uintptr_t addr)
{
    errno_t ret;
    struct soc *soc;

    soc = vpc->vm->soc;
    if (soc->nr_devices > 0) {
        if (vpc_is_aarch64(vpc) || vpc_is_aarch32a(vpc)) {
            ret = test_executable_region(soc, addr, 4);
        } else if (vpc_is_aarch32t(vpc)) {
            ret = test_executable_region(soc, addr, 2);
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EPERM;
    }
    
    return ret;
}
