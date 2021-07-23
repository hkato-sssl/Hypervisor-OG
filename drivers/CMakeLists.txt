cmake_minimum_required(VERSION 3.10.2)
project(drivers)
enable_language(C ASM)
include(../scripts/cmake/aarch64-gcc.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(
    drivers STATIC
    src/arm/gic400/gic400_dump_ns_cpuif.c
    src/arm/gic400/gic400.c
    src/arm/gic400/gic400_dump_ns_distributor.c
    src/arm/gic400/gic400_initialize.c
    src/arm/gic400/gic400_set_priority_mask.c
    src/arm/smmu500/smmu500_configure_translation_stream.c
    src/arm/smmu500/smmu500_allocate_context_bank.c
    src/arm/smmu500/smmu500_initialize.c
    src/arm/smmu500/smmu500_free_context_bank.c
    src/arm/smmu500/smmu500_dump.c
    src/arm/smmu500/smmu500_create_context_bank_with_stage2.c
    src/arm/smmu500/smmu500_enable.c
    src/arm/smmu500/smmu500_disable.c
    src/arm/smmu500/smmu500_configure_context_bank_with_stage2.c
    src/arm/smmu500/smmu500_create_translation_stream.c
    src/arm/smmu500/smmu500_allocate_stream_match_register.c
    src/aarch64/aarch64_interrupts.S
    src/aarch64/exception/aarch64_exception_esr_ec_message.c
    src/aarch64/exception/aarch64_exception_dump.c
    src/aarch64/aarch64_cpu_no.S
    src/aarch64/system_register.S
    src/aarch64/stage2/aarch64_stage2_dump_descriptor.c
    src/aarch64/stage2/aarch64_stage2_descriptor.c
    src/aarch64/stage2/aarch64_stage2_initialize.c
    src/aarch64/stage2/aarch64_stage2_map.c
    src/aarch64/aarch64_fiq.S
    src/aarch64/aarch64_irq.S
    src/aarch64/cache/aarch64_dcache_clean_invd.S
    src/aarch64/cache/aarch64_icache_invd.S
    src/aarch64/cache/aarch64_dcache_invd.S
    src/aarch64/cache/aarch64_dcache_clean.S
    src/aarch64/cache/aarch64_dcache_op_range.S
    src/aarch64/cache/aarch64_dcache_op_all.S
    src/aarch64/psci/aarch64_psci.S
    src/aarch64/psci/aarch64_psci_cpu_off.c
    src/aarch64/psci/aarch64_psci_cpu_on.c
    src/aarch64/mmu/aarch64_mmu_initialize.c
    src/aarch64/mmu/aarch64_mmu_enable_asm.S
    src/aarch64/mmu/aarch64_mmu_tlbi_va.S
    src/aarch64/mmu/aarch64_mmu_block_pool.c
    src/aarch64/mmu/aarch64_mmu_map_4KB.c
    src/aarch64/mmu/aarch64_mmu_dump_descriptor.c
    src/aarch64/mmu/aarch64_mmu_descriptor.c
    src/aarch64/mmu/aarch64_mmu_map.c
    src/aarch64/mmu/aarch64_mmu_set_translation_table.c
    src/aarch64/mmu/aarch64_mmu_tlbi_all.S
    src/aarch64/mmu/aarch64_mmu_write_descriptor.S
    src/aarch64/mmu/aarch64_mmu_enable.c
    src/aarch64/mmu/aarch64_mmu_disable.S
    src/aarch64/mmu/aarch64_mmu_map_region.c
    src/aarch64/at/aarch64_va_to_pa_el2.c
    src/aarch64/at/aarch64_at_s1e2.S
    src/aarch64/at/aarch64_at_s1.S
    src/aarch64/at/aarch64_at_s12.S
    src/aarch64/aarch64_udelay.S
    src/xilinx/mpsoc/ps_uart/ps_uart_putc_poll.c
    src/xilinx/axi/uart_lite.c
)

include_directories(inc ../libs/inc)
