// SPDX-License-Identifier: Apache-2.0
/*
 * test/bitmap/02.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト項目：検索開始位置指定の順次検索
 *
 * 手順：
 * 1) bitmap_search0()でmapのbit nから検索
 * 2) nを0から始めてエラーになるまで1)を実行
 */

#include "lib/bitmap.h"
#include "lib/system/printk.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

static uint8_t map[16];

/* functions */

static void dump_map(void)
{
    int i;

    printk("    ");
    for (i = sizeof(map) - 1; i >= 0; --i) {
        printk("%02x", map[i]);
    }
    printk("\n");
}

void test_bitmap_02(void)
{
    int i;
    errno_t ret;
    uint32_t no;

    printk("<%s>\n", __func__);

    memset(map, 0, sizeof(map));

    i = 1;
    do {
        ret = bitmap_search0(&no, map, sizeof(map), (i - 1));
        printk("#%d: bitmap_search0() -> %d, no=%u\n", i, ret, no);
        dump_map();
        ++i;
    } while (ret == SUCCESS);
}
