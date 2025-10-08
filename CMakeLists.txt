# SPDX-FileCopyrightText: Copyright 2025 Yamane Kazuma
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION "3.20")

include(FetchContent)

set(SAPISRESOLVER_VERSION_MAJOR 1)
set(SAPISRESOLVER_VERSION_MINOR 1)
set(SAPISRESOLVER_VERSION_PATCH 1)

project(sapisresolver VERSION ${SAPISRESOLVER_VERSION_MAJOR}.${SAPISRESOLVER_VERSION_MINOR}.${SAPISRESOLVER_VERSION_PATCH} LANGUAGES CXX)

# ==== Config ====

option(SAPISRESOLVER_BUILD_ALL "Build all artifacts" OFF)

option(SAPISRESOLVER_BUILD_EXAMPLE "Build example" OFF)
option(SAPISRESOLVER_BUILD_EXAMPLE_HO "Build example using header only version" OFF)

option(SAPISRESOLVER_BUILD_TESTS "Build tests" OFF)
option(SAPISRESOLVER_BUILD_TESTS_HO "Build tests for header only version" OFF)

# ================

set(SAPISRESOLVER_SOURCES "include/apiset/impl/resolver.cpp")
set(SAPISRESOLVER_HEADERS "include/apiset/resolver.h")

add_library(sapisresolver ${SAPISRESOLVER_SOURCES})
add_library(sapisresolver::sapisresolver ALIAS sapisresolver)

target_compile_features(sapisresolver PUBLIC cxx_std_23)

target_include_directories(sapisresolver PUBLIC include/)

target_compile_definitions(
  sapisresolver
  PUBLIC
  $<$<CONFIG:Debug>:
    _DEBUG
    DEBUG
  >
)

if(MSVC)
  set_property(TARGET sapisresolver PROPERTY MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug>:EditAndContinue>")
  target_compile_options(
    sapisresolver
    PUBLIC
    /constexpr:steps8000000
    /W4 /arch:AVX /fp:fast /GS /GT /MP /nologo /Oi /Qpar /utf-8
    $<$<CONFIG:Debug>:/JMC /LDd /Ob0 /Od /RTC1>
    $<$<CONFIG:Release>:/GA /GL /Gy /guard:cf- /LD /Os>
  )
  if(CMAKE_CXX_FLAGS MATCHES "/W3")
    STRING(REPLACE "/W3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  endif(CMAKE_CXX_FLAGS MATCHES "/W3")
  if(CMAKE_CXX_FLAGS MATCHES "/GR")
    STRING(REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else(CMAKE_CXX_FLAGS MATCHES "/GR")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
  endif(CMAKE_CXX_FLAGS MATCHES "/GR")
else(MSVC)
  target_compile_options(
    sapisresolver
    PUBLIC
    -fconstexpr-steps=10000000
    -Wall -Wextra -mavx -mfpmath=sse
    -finput-charset=UTF-8 -fexec-charset=UTF-8 -municode
    $<$<CONFIG:Debug>:-Og -g>
    $<$<CONFIG:Release>:-O2 -ftree-vectorize -fomit-frame-pointer>
  )
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # for x64
    target_compile_options(sapisresolver PUBLIC -m64)
  else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # for x86
    target_compile_options(sapisresolver PUBLIC -m32)
  endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif(MSVC)

FetchContent_Declare(
  frozen
  GIT_REPOSITORY https://github.com/serge-sans-paille/frozen.git
  TAG 1.2.0
)
FetchContent_MakeAvailable(frozen)
target_link_libraries(sapisresolver PUBLIC frozen::frozen)

# Define for header only version

add_library(sapisresolver_header_only INTERFACE)
add_library(sapisresolver::sapisresolver_header_only ALIAS sapisresolver_header_only)

target_compile_definitions(sapisresolver_header_only INTERFACE SAPISRESOLVER_HEADER_ONLY=1)
target_compile_features(sapisresolver_header_only INTERFACE cxx_std_23)

target_include_directories(sapisresolver_header_only INTERFACE include/)

if(MSVC)
  target_compile_options(
    sapisresolver_header_only
    INTERFACE
    /constexpr:steps8000000
    /utf-8
  )
else(MSVC)
  target_compile_options(
    sapisresolver_header_only
    INTERFACE
    -fconstexpr-steps=10000000
  )
endif(MSVC)

target_link_libraries(sapisresolver_header_only INTERFACE frozen::frozen-headers)

# Define examples

if(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_EXAMPLE OR SAPISRESOLVER_BUILD_EXAMPLE_HO)

  if(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_EXAMPLE)
    add_subdirectory(example/sapisresolver)
  endif(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_EXAMPLE)

  if(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_EXAMPLE_HO)
    add_subdirectory(example/sapisresolver_header_only)
  endif(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_EXAMPLE_HO)

endif(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_EXAMPLE OR SAPISRESOLVER_BUILD_EXAMPLE_HO)

# Define tests

if(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_TESTS OR SAPISRESOLVER_BUILD_TESTS_HO)

  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  set(SAPISRESOLVER_TEST_SOURCES test/resolver.cpp)

  if(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_TESTS)
    add_executable(${PROJECT_NAME}-googletest ${SAPISRESOLVER_TEST_SOURCES})
    target_link_libraries(${PROJECT_NAME}-googletest sapisresolver)
    target_link_libraries(${PROJECT_NAME}-googletest gtest_main)
    add_test(NAME test COMMAND ${PROJECT_NAME}-googletest)
  endif(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_TESTS)

  if(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_TESTS_HO)
    add_executable(${PROJECT_NAME}_header_only-googletest ${SAPISRESOLVER_TEST_SOURCES})
    target_link_libraries(${PROJECT_NAME}_header_only-googletest sapisresolver_header_only)
    target_link_libraries(${PROJECT_NAME}_header_only-googletest gtest_main)
    add_test(NAME test_header_only COMMAND ${PROJECT_NAME}_header_only-googletest)
  endif(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_TESTS_HO)

  enable_testing()

endif(SAPISRESOLVER_BUILD_ALL OR SAPISRESOLVER_BUILD_TESTS OR SAPISRESOLVER_BUILD_TESTS_HO)
