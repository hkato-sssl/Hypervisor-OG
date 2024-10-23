cmake_minimum_required(VERSION 3.10.2)
project(HypervisorOG)

#set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(projects/libs)
add_subdirectory(projects/drivers)
add_subdirectory(projects/hypervisor)
add_subdirectory(projects/guest)
add_subdirectory(projects/zcu104-linux)
