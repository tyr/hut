#
# General definitions
#

cmake_minimum_required (VERSION 2.8)
project (hut)

#
# Set CMake policies
#

if(POLICY CMP0037)
  cmake_policy(SET CMP0037 NEW)
endif()

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

#
# Set directories and paths
#

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(PROJECT_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_ROOT}/main)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_ROOT}/include)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_ROOT}/test)
set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

#
# Check proper build type
#

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug
      CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)
message("-- Current build type is : ${CMAKE_BUILD_TYPE}")

#
# Set C11 compile flags
#

include(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG("-std=gnu89" COMPILER_SUPPORTS_GNU89)
if(COMPILER_SUPPORTS_GNU89)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89")
else()
  message(STATUS "The compiler `${CMAKE_C_COMPILER}` has no GNU89 support. Please use a different C compiler.")
endif()

#
# Set C++11 compile flags
#

#include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#if(COMPILER_SUPPORTS_CXX11)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#elseif(COMPILER_SUPPORTS_CXX0X)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#else()
#  message(STATUS "The compiler `${CMAKE_CXX_COMPILER}` has no C++11 support. Please use a different C++ compiler.")
#endif()

#
# Set compiler flags
#

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -D_FILE_OFFSET_BITS=64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT -D_GNU_SOURCE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")

#
# Set code coverage
#

if(CMAKE_BUILD_TYPE MATCHES Coverage)

  include(CodeCoverage)
  setup_target_for_coverage(coverage make coverage "-C;${CMAKE_BINARY_DIR};test;ARGS=\"-V\"")

  set(COVERAGE_COMPILE_FLAGS "-fno-inline")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_COVERAGE} ${COVERAGE_COMPILE_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_COVERAGE} ${COVERAGE_COMPILE_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_COVERAGE}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_COVERAGE}")

endif(CMAKE_BUILD_TYPE MATCHES Coverage)

#
# Set additional linker flags
#

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")

#
# Set include directories
#

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

#
# Handle tinycthread
#

include_directories("${PROJECT_LIB_DIR}/tinycthread/source/")
add_library(tinycthread STATIC "${PROJECT_LIB_DIR}/tinycthread/source/tinycthread.c")
link_libraries(tinycthread ${CMAKE_THREAD_LIBS_INIT})

#
# Handle xxhash
#

include_directories("${PROJECT_LIB_DIR}/xxhash/")
add_library(xxhash STATIC "${PROJECT_LIB_DIR}/xxhash/xxhash.c")

#
# Handle testing
#

option(BUILD_TESTS "Whether or not to build the tests" ON)
if(BUILD_TESTS)
  if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
  endif (APPLE)
  add_subdirectory(${PROJECT_LIB_DIR}/googletest/googletest)
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  enable_testing()
else(BUILD_TESTS)
  message(STATUS "Tests will not be compiled")
endif(BUILD_TESTS)

#
# Include subdirectory
#

add_subdirectory(src)
