# set(CMAKE_BUILD_TYPE release)
# set(CMAKE_BUILD_TYPE debug)
# or
# cmake ../project -DCMAKE_BUILD_TYPE=Release
# cmake ../project -DCMAKE_BUILD_TYPE=Debug

set(CMAKE_BUILD_TYPE release)
set(ARM_COMPILE_OPTION "${ARM_COMPILE_OPTION} -march=armv8-a+nofp -mstrict-align")
set(CMAKE_C_FLAGS "${ARM_COMPILE_OPTION} -ffunction-sections")
set(CMAKE_CXX_FLAGS "${ARM_COMPILE_OPTION}")

# cross compiler settings
set(CMAKE_CROSSCOMPILING TRUE)
set(CROSS_COMPILE "aarch64-none-elf-")
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
set(CMAKE_LINKER ${CROSS_COMPILE}gcc)
set(CMAKE_OBJCOPY ${CROSS_COMPILE}objcopy)

# root path settings
set(CMAKE_FIND_ROOT_PATH /usr/local/sssl/toolchain/aarch64)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)    # use host system root for program
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)     # use CMAKE_FIND_ROOT_PATH for library
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)     # use CMAKE_FIND_ROOT_PATH for include
