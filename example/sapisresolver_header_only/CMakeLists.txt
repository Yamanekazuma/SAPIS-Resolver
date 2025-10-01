cmake_minimum_required(VERSION 3.20)
project(sapisresolver_header_only-example VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

# For stand-alone build
# (For example, if you use a git submodule)
if(NOT TARGET sapisresolver)
  find_package(sapisresolver REQUIRED)
endif()

## Recommended build style
# include(FetchContent)
# FetchContent_Declare(
#   sapisresolver
#   GIT_REPOSITORY https://github.com/Yamanekazuma/SAPIS-Resolver.git
#   GIT_TAG v1.0.0  # Choose any version
# )
# FetchContent_MakeAvailable(sapisresolver)

target_link_libraries(${PROJECT_NAME} sapisresolver::sapisresolver_header_only)
