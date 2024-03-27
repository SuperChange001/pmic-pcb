
add_library(pmic_lib INTERFACE)

target_sources(pmic_lib INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/max77654.c
        )


target_include_directories(pmic_lib INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(pmic_lib INTERFACE pico_stdlib hardware_i2c)