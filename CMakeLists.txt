cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    execute_process(
            COMMAND brew --prefix llvm
            OUTPUT_VARIABLE LLVM_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    add_link_options(
            "-L${LLVM_PREFIX}/lib/c++"
            "-Wl,-rpath,${LLVM_PREFIX}/lib/c++"
    )
endif()

add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

project(advent-of-code CXX)

add_subdirectory(utils)
add_subdirectory(2025)

