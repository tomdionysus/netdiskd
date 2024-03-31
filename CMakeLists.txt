cmake_minimum_required(VERSION 3.12)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(netdisk C CXX ASM)

include(FetchContent)

# Fetch Boost
set(BOOST_VERSION 1.83.0) # Define the Boost version
FetchContent_Declare(
  boost
  URL https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_1_83_0.tar.bz2
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_GetProperties(boost)
if(NOT boost_POPULATED)
  FetchContent_Populate(boost)
  set(BOOST_ROOT ${boost_SOURCE_DIR})
endif()

# Fetch Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Specify the required Boost components
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS program_options)

# Locate MySQL include directory and library
find_path(MYSQL_INCLUDE_DIR mysql.h
          PATHS /usr/include /usr/local/include
                /usr/include/mysql /usr/local/include/mysql
                PATH_SUFFIXES mysql)
find_library(MYSQL_LIBRARY NAMES mysqlclient
             PATHS /usr/lib /usr/local/lib
                   /usr/lib/mysql /usr/local/lib/mysql)

# Includes
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/libnetdisk
    ${CMAKE_SOURCE_DIR}/src/netdiskd
    ${MYSQL_INCLUDE_DIR} # Include MySQL directories
    ${Boost_INCLUDE_DIRS}
)

file(GLOB libnetdisk_SOURCES ${CMAKE_SOURCE_DIR}/src/libnetdisk/*.cpp)
file(GLOB netdiskd_SOURCES ${CMAKE_SOURCE_DIR}/src/netdiskd/*.cpp)
file(GLOB netdisk_test_SOURCES ${CMAKE_SOURCE_DIR}/src/tests/*.cpp)

# Dependencies
find_package(Threads REQUIRED)

# libnetdisk library
add_library(netdisk SHARED ${libnetdisk_SOURCES})
install(TARGETS netdisk LIBRARY DESTINATION lib)

# netdiskd executable
add_executable(netdiskd ${netdiskd_SOURCES})
target_link_libraries(netdiskd
    netdisk
    Threads::Threads
    Boost::program_options
    ${MYSQL_LIBRARY}
)
install(TARGETS netdiskd DESTINATION bin)

# Test executable
add_executable(netdisk_tests ${netdisk_test_SOURCES})
target_link_libraries(netdisk_tests gtest_main netdisk)

include(GoogleTest)
gtest_discover_tests(netdisk_tests)
