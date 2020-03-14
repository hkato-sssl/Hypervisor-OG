/*
 * test/bitmap/01.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト項目：順次検索と指定ビットのセット
 *
 * 手順：
 * 1) bitmap_search0()でmapのbit 0から0となるbitを検索
 * 2) 検索したビットをbitmap_set()で１に設定
 * 3) 1)2)の処理をエラーとなるまで繰り返す
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/printk.h"
#include "lib/bitmap.h"

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

void test_bitmap_01(void)
{
    int i;
    errno_t ret;
    uint32_t no;

    printk("<%s>\n", __func__);

    memset(map, 0, sizeof(map));
    
    i = 1;
    do {
        ret = bitmap_search0(&no, map, sizeof(map), 0);
        printk("#%d: bitmap_search0() -> %d, no=%u\n", i, ret, no);
        dump_map();
        if (ret == SUCCESS) {
            ret = bitmap_set(map, sizeof(map), no);
            printk("#%d: bitmap_set(%u) -> %d\n", i, no, ret);
            dump_map();
        }
        ++i;
    } while (ret == SUCCESS);
}
