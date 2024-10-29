cmake_minimum_required(VERSION 3.10.2)
project(zcu104-linux)
enable_language(C ASM)

get_filename_component(WORKSPACE_BASE ../../ ABSOLUTE)
set(TARGET_NAME "${PROJECT_NAME}.elf")

include(${WORKSPACE_BASE}/scripts/cmake/xilinx-ultrascale-mpsoc.cmake)

add_executable(
    ${TARGET_NAME}
    config/memory.c
    config/memory_asm.S
    src/boot/init_processor_el2.S
    src/boot/start.S
    src/boot/initial_exception_vector.S
    src/hypervisor/hypervisor_start.S
    src/hypervisor/hypervisor.c
    src/hypervisor/guest
    src/hypervisor/guest/guest_linux_hvc.c
    src/hypervisor/guest/guest_usb_linux_device.c
    src/hypervisor/guest/guest_linux_device.c
    src/hypervisor/guest/guest_usb_linux.c
    src/hypervisor/guest/guest_usb_linux_hvc.c
    src/hypervisor/guest/guest_linux.c
    src/hypervisor/guest/shared_resource.c
    src/main.c
    src/init/init_system.c
    src/init/init_hw.c
    src/init/init_memory_map.c
)

target_link_libraries(${TARGET_NAME} -Wl,--start-group libs drivers hypervisor -Wl,--end-group)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -nostartfiles -static -T ${PROJECT_SOURCE_DIR}/config/memory.lds -T ${WORKSPACE_BASE}/scripts/ld/aarch64-zcu104-ram.lds")

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${TARGET_NAME} ${PROJECT_NAME}.bin 
)

include_directories(.)
include_directories(inc)
include_directories(${WORKSPACE_BASE}/projects/libs/inc)
include_directories(${WORKSPACE_BASE}/projects/libs/inc/lib/system/stdc)
include_directories(${WORKSPACE_BASE}/projects/drivers/inc)
include_directories(${WORKSPACE_BASE}/projects/hypervisor/inc)
