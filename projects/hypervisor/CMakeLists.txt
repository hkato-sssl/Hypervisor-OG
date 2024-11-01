cmake_minimum_required(VERSION 3.10.2)
project(hypervisor)
enable_language(C ASM)
get_filename_component(WORKSPACE_BASE ../../ ABSOLUTE)
include(${WORKSPACE_BASE}/scripts/cmake/xilinx-ultrascale-mpsoc.cmake)

add_library(
    hypervisor STATIC
    src/exception/excvec_hyp.S
    src/exception/handler_el2.c
    src/emulator/insn/insn_parse_aarch64_system_call.c
    src/emulator/insn/insn_emulate_indexed_addressing.c
    src/emulator/insn/insn_parse_aarch64_ldr_str.c
    src/emulator/insn/insn_emulate_str.c
    src/emulator/insn/insn_print.c
    src/emulator/insn/insn_str_src_value.c
    src/emulator/insn/insn_emulate_ldr.c
    src/emulator/simple_psci/simple_psci_initialize.c
    src/emulator/simple_psci/simple_psci_cpu_on.c
    src/emulator/simple_psci/simple_psci_version.c
    src/emulator/psci/psci_call_emulator_ops.c
    src/emulator/psci/psci_set_error.c
    src/emulator/psci/psci_error_unknown_function.c
    src/emulator/vgic400/vgic400_configure_interrupt.c
    src/emulator/vgic400/vgic400_activate_virtual_cpuif.c
    src/emulator/vgic400/vgic400_distributor_ixactiver.c
    src/emulator/vgic400/vgic400_distributor_itargetsr.c
    src/emulator/vgic400/vgic400_distributor_ctlr.c
    src/emulator/vgic400/vgic400_distributor_typer.c
    src/emulator/vgic400/vgic400_distributor_ixpendr.c
    src/emulator/vgic400/gic400_dump_virtif_control.c
    src/emulator/vgic400/gic400_dump_virtual_cpuif.c
    src/emulator/vgic400/vgic400_cpuif_write_word_register.c
    src/emulator/vgic400/vgic400_list_register.c
    src/emulator/vgic400/vgic400_distributor_byte_register.c
    src/emulator/vgic400/vgic400_inject_sgi.c
    src/emulator/vgic400/vgic400_create_interrupt_event.c
    src/emulator/vgic400/vgic400_inject_interrupt_event.c
    src/emulator/vgic400/vgic400_push_interrupt_event.c
    src/emulator/vgic400/vgic400_pop_interrupt_event.c
    src/emulator/vgic400/vgic400_irq_handler.c
    src/emulator/vgic400/vgic400_distributor_ipriorityr.c
    src/emulator/vgic400/vgic400_distributor_ixenabler.c
    src/emulator/vgic400/vgic400_distributor_error.c
    src/emulator/vgic400/vgic400_distributor_spisr.c
    src/emulator/vgic400/vgic400_distributor_icfgr.c
    src/emulator/vgic400/vgic400_cpu_map.c
    src/emulator/vgic400/vgic400_distributor_emulate_memory_access.c
    src/emulator/vgic400/vgic400_distributor_sgir.c
    src/emulator/vgic400/vgic400_distributor_ro_word_register.c
    src/emulator/vgic400/vgic400_distributor_igroupr.c
    src/emulator/vgic400/vgic400_cpuif_emulate_memory_access.c
    src/emulator/vgic400/vgic400_initialize.c
    src/emulator/vgic400/vgic400_distributor_bit_register.c
    src/emulator/vgic400/vgic400_update_list_registers.c
    src/emulator/vgic400/vgic400_write_interrupt_event.c
    src/vpc/vpc_wakeup_processor.c
    src/vpc/vpc_ctx_fpu.S
    src/vpc/vpc_dump.c
    src/vpc/vpc_wait_request.c
    src/vpc/vpc_read_instruction.c
    src/vpc/vpc_update_pc.c
    src/vpc/vpc_set_boot_parameters.c
    src/vpc/vpc_emulate_exception.c
    src/vpc/vpc_emulate_aarch64_data_abort.c
    src/vpc/vpc_send_event.c
    src/vpc/vpc_is_aarch32.c
    src/vpc/vpc_ctx_sys.S
    src/vpc/vpc_call_event.c
    src/vpc/vpc_switch_to_el1.S
    src/vpc/vpc_is_aarch64.c
    src/vpc/vpc_event_loop.c
    src/vpc/vpc_launch.c
    src/vpc/vpc_va_to_pa_r.c
    src/vpc/vpc_initialize.c
    src/vpc/vpc_resume.c
    src/thread/thread_launch_self.c
    src/thread/thread_start.S
    src/thread/thread_parameter_array.c
    src/thread/thread_standby.S
    src/thread/tls.c
    src/thread/thread_launch.c
    src/thread/thread_terminate.S
    src/soc/soc_initialize_vpc.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_assert_interrupt_vpc.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_default_vpc_hook.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_hook.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_initialize.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_emulate_hvc.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_emulate_tz_asm.S
    src/soc/xilinx/mpsoc/xilinx_mpsoc_emulate_psci.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_emulate_irq_exception.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_emulate_tz.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_default_vpc_exception_ops.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_p2p_packet_ep_ops.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_ops.c
    src/soc/xilinx/mpsoc/xilinx_mpsoc_emulate_smc.c
    src/soc/soc_initialize.c
    src/soc/soc_default_test_executable_region.c
    src/test/soc/xilinx/mpsoc/01.c
    src/test/soc/xilinx/mpsoc/02.c
    src/test/stage2.c
    src/hvc/hvc_service_id.c
    src/hvc/hvc_server.c
    src/hvc/hvc_match_service.c
    src/hvc/hvc_service_append_slist.c
    src/hvc/p128/hvc_p128_service_initialize.c
    src/hvc/p128/hvc_p128_server.c
    src/hvc/hvc_service_initialize.c
    src/hvc/hvc_set_result.c
    src/mmu/hyp_mmu_stage2_shareability.c
    src/mmu/hyp_mmu_stage2_memory_type.c
    src/vm/vm_launch.c
    src/vm/vm_launch_secondary.c
    src/vm/vm_proc_no.c
    src/vm/vm_wakeup.c
    src/vm/vm_register_vpc.c
    src/vm/vm_search_region_trap.c
    src/vm/vm_initialize.c
    src/vm/vm_acquire_vpc.c
    src/vm/vm_register_region_trap.c
    src/service/p2p_packet/p2p_packet_connect.c
    src/service/p2p_packet/p2p_packet_initialize_path.c
    src/service/p2p_packet/p2p_packet_receive.c
    src/service/p2p_packet/p2p_packet_call_handler.c
    src/service/p2p_packet/p2p_packet_send.c
    src/service/p2p_packet/p2p_packet_initialize_ep.c
)

include_directories(inc ../libs/inc ../libs/inc/lib/system/stdc ../drivers/inc)
