// SPDX-License-Identifier: Apache-2.0
/*
 * test/bitmap/04.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト項目：先頭からの検索＆セットを実行
 *
 * 手順：
 * 1) bitmap_search_and_set()でmapのbit 0から0となるbitを検索
 * 2) 1)の処理をエラーとなるまで繰り返す
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

void test_bitmap_04(void)
{
    int i;
    errno_t ret;
    uint32_t no;

    printk("<%s>\n", __func__);

    memset(map, 0, sizeof(map));

    i = 1;
    do {
        ret = bitmap_search_and_set(&no, map, sizeof(map), 0);
        printk("#%d: bitmap_search_and_set() -> %d, no=%u\n", i, ret, no);
        dump_map();
        ++i;
    } while (ret == SUCCESS);
}
