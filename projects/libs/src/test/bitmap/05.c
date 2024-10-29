// SPDX-License-Identifier: Apache-2.0
/*
 * test/bitmap/05.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト項目：検索＆セットを検索開始位置を変更して実行
 *
 * 手順：
 * 1) bit 0からbitmap_search_and_set()を実行
 * 2) 検索開始位置を１ビットずらしながら1)の処理をエラーとなるまで繰り返す
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

void test_bitmap_05(void)
{
    int i;
    errno_t ret;
    uint32_t no;

    printk("<%s>\n", __func__);

    memset(map, 0, sizeof(map));

    i = 1;
    do {
        ret = bitmap_search_and_set(&no, map, sizeof(map), (i - 1));
        printk("#%d: bitmap_search_and_set() -> %d, no=%u\n", i, ret, no);
        dump_map();
        ++i;
    } while (ret == SUCCESS);
}
