file(GLOB DAY_DIRECTORIES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/day*")

foreach(day_directory ${DAY_DIRECTORIES})
    get_filename_component(day_name ${day_directory} NAME)

    foreach(part part1 part2)
        set(source "${day_directory}/${part}.cpp")
        if(NOT EXISTS ${source})
            continue()
        endif()

        set(target "2025_${day_name}_${part}")
        add_executable(${target} ${source})
        target_link_libraries(${target} PRIVATE utils)
    endforeach()
endforeach()
