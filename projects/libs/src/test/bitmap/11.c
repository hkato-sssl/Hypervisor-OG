// SPDX-License-Identifier: Apache-2.0
/*
 * test/bitmap/11.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト項目：指定ビットのクリア
 *
 * 手順：
 * 1) map[]を0xffで初期化
 * 2) エラーになるまでbit 0からbitmap_clear()を実行
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

void test_bitmap_11(void)
{
    int i;
    errno_t ret;

    printk("<%s>\n", __func__);

    memset(map, 0xff, sizeof(map));

    i = 1;
    for (i = 0;; ++i) {
        ret = bitmap_clear(map, sizeof(map), i);
        printk("bitmap_clear(%d) -> %d\n", i, ret);
        dump_map();
        if (ret != SUCCESS) {
            break;
        }
    }
}
