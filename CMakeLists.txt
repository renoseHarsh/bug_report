cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)
set(CMAKE_AR /opt/homebrew/opt/llvm/bin/llvm-ar)
set(CMAKE_RANLIB /opt/homebrew/opt/llvm/bin/llvm-ranlib)
set(CMAKE_CXX_STDLIB_MODULES_JSON
    /opt/homebrew/Cellar/llvm/23.1.0/lib/c++/libc++.modules.json)

project(bug_repo LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG main
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(glaze)

add_library(glaze_wrapper)
target_sources(glaze_wrapper PUBLIC FILE_SET CXX_MODULES FILES
                                    glaze_wrapper.cppm)
target_link_libraries(glaze_wrapper PRIVATE glaze::glaze)

add_executable(main)
target_sources(main PRIVATE main.cpp)
target_link_libraries(main PRIVATE glaze_wrapper)
