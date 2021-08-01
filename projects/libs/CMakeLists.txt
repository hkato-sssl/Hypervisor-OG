cmake_minimum_required(VERSION 3.10.2)
project(libs)
enable_language(C ASM)
include(../../scripts/cmake/aarch64-gcc.cmake)

add_library(
    libs STATIC
    src/aarch64/aarch64_spin_lock.S
    src/aarch64/atomic.S
    src/slist/slist_append.c
    src/slist/slist_search_element.c
    src/slist/slist_init.c
    src/system/system_spin_lock.c
    src/system/spin_lock.S
    src/system/system_errno.c
    src/system/stdc/stdio/vsnprintf.c
    src/system/stdc/stdio/sprintf.c
    src/system/stdc/stdio/snprintf.c
    src/system/stdc/stdio/vsprintf.c
    src/system/stdc/string/strlen.S
    src/system/stdc/string/memcpy.S
    src/system/stdc/string/memset.S
    src/system/stdc/string/strncpy.S
    src/system/system_error.S
    src/system/system_test_valid_stack_region.c
    src/system/reg_write_sync.S
    src/system/system_assert.S
    src/log/log_output_c.c
    src/log/log_output_pads.c
    src/log/log_output_string.c
    src/log/log_cformat.c
    src/log/log_output_prefix.c
    src/log/log_output_unsigned_number.c
    src/log/log_output_s.c
    src/log/log_output_di.c
    src/bitmap/bitmap_clear.c
    src/bitmap/bitmap_search_and_set.c
    src/bitmap/bitmap_set.c
    src/bitmap/bitmap_search0.c
    src/list.c
)

include_directories(inc)
