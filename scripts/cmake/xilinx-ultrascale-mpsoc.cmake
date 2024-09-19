# set(CMAKE_BUILD_TYPE release)
# set(CMAKE_BUILD_TYPE debug)
# or
# cmake ../project -DCMAKE_BUILD_TYPE=Release
# cmake ../project -DCMAKE_BUILD_TYPE=Debug

set(ARM_COMPILE_OPTION "-DXILINX_MPSOC -Wall")

include(../../scripts/cmake/aarch64-gcc.cmake)
