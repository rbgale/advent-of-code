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

project(advent-of-code CXX)

find_package(OpenSSL REQUIRED)

add_subdirectory(utils)

file(GLOB YEAR_DIRECTORIES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/[0-9][0-9][0-9][0-9]")

foreach(year_directory ${YEAR_DIRECTORIES})
    get_filename_component(year_name ${year_directory} NAME)
    file(GLOB DAY_DIRECTORIES CONFIGURE_DEPENDS "${year_directory}/day*")

    foreach(day_directory ${DAY_DIRECTORIES})
        get_filename_component(day_name ${day_directory} NAME)

        foreach(part part1 part2)
            set(source "${day_directory}/${part}.cpp")
            if(NOT EXISTS ${source})
                continue()
            endif()

            set(target "${year_name}_${day_name}_${part}")
            add_executable(${target} ${source})
            target_link_libraries(${target} PRIVATE utils)

            file(READ ${source} source_contents)
            if(source_contents MATCHES "openssl/md5\\.h")
                target_link_libraries(${target} PRIVATE OpenSSL::Crypto)
            endif()
        endforeach()
    endforeach()
endforeach()
