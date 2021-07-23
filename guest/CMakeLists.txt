cmake_minimum_required(VERSION 3.10.2)
project(guest)
enable_language(C ASM)
include(../scripts/cmake/aarch64-gcc.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(
    guest STATIC
    config/memory_asm.S
    config/memory.c
    src/boot/init_processor_el1.S
    src/boot/start.S
    src/boot/initial_exception_vector.S
    src/exception/excvec_el1.S
    src/exception/handler_el1.c
    src/main.c
    src/test/hvcs/01.c
    src/test/hvcs/01_asm.S
    src/test/fp/01.c
    src/test/insn/01_parse_aarch64_ldr.S
    src/test/insn/03_parse_aarch64_ldr_literal_asm.S
    src/test/insn/03_parse_aarch64_ldr_literal.c
    src/test/insn/02_parse_aarch64_str.S
    src/test/gic/01_dist_register.c
    src/test/gic/04_spi.c
    src/test/gic/02_sgi.c
    src/test/gic/03_pending.c
    src/test/gic/05_spi_multi.c
    src/test/timer/test_timer_01.c
    src/test/vspi/08.c
    src/test/vspi/07.c
    src/test/vspi/05.c
    src/test/vspi/vspi.c
    src/test/vspi/01.c
    src/test/vspi/03.c
    src/test/vspi/11.c
    src/test/vspi/12.c
    src/test/vspi/10.c
    src/test/vspi/06.c
    src/test/vspi/13.c
    src/test/vspi/15.c
    src/test/vspi/14.c
    src/test/vspi/09.c
    src/test/vspi/04.c
    src/test/vspi/02.c
    src/test/smmu/01.c
    src/init/init_system.c
    src/init/init_hw.c
    src/init/init_memory_map.c
)

include_directories(. inc ../libs/inc ../drivers/inc ../hypervisor/inc)
