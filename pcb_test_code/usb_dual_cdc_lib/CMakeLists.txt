
add_library(usb_dual_cdc_lib INTERFACE)

target_sources(usb_dual_cdc_lib INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/usb_dual_cdc.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_stdio_cdc.c
        )

target_include_directories(usb_dual_cdc_lib INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(usb_dual_cdc_lib INTERFACE pico_stdlib)

################################################################################
# creates test_usb_dual_cdc executable
add_executable(test_usb_dual_cdc ${CMAKE_CURRENT_LIST_DIR}/test_usb_dual_cdc.c)
target_include_directories(test_usb_dual_cdc PUBLIC .)
# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(test_usb_dual_cdc pico_stdlib pico_stdlib hardware_flash tinyusb_device usb_dual_cdc_lib)

# enable usb output, disable uart output
pico_enable_stdio_usb(test_usb_dual_cdc 0)
pico_enable_stdio_uart(test_usb_dual_cdc 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test_usb_dual_cdc)

################################################################################
# creates test_usb_dual_cdc_stdio executable
add_executable(test_usb_dual_cdc_stdio ${CMAKE_CURRENT_LIST_DIR}/test_usb_dual_cdc_stdio.c)
target_include_directories(test_usb_dual_cdc_stdio PUBLIC .)
# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(test_usb_dual_cdc_stdio pico_stdlib pico_stdlib hardware_flash tinyusb_device usb_dual_cdc_lib)

# enable usb output, disable uart output
pico_enable_stdio_usb(test_usb_dual_cdc_stdio 0)
pico_enable_stdio_uart(test_usb_dual_cdc_stdio 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test_usb_dual_cdc_stdio)