cmake_minimum_required(VERSION 3.12)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(netdisk C CXX ASM)

# Includes
include_directories (
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/libnetdisk
    ${CMAKE_SOURCE_DIR}/src/netdiskd
)

file(GLOB libnetdisk_SOURCES ${CMAKE_SOURCE_DIR}/src/libnetdisk/*.cpp)
file(GLOB netdiskd_SOURCES ${CMAKE_SOURCE_DIR}/src/netdiskd/*.cpp)

# Dependencies
find_package(Threads REQUIRED)

# libnetdisk
add_library(
    netdisk SHARED 
    ${libnetdisk_SOURCES}
)
install(TARGETS netdisk LIBRARY DESTINATION lib)

# netdiskd
add_executable(
    netdiskd
    ${netdiskd_SOURCES}
)

target_link_libraries(
    netdiskd 
    netdisk
    Threads::Threads
)

install(TARGETS netdiskd DESTINATION bin)

