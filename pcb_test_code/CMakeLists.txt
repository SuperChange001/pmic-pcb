cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (TARGET tinyusb_device)

    # adds usb_dual_cdc_lib dependency for networking
    add_subdirectory(usb_dual_cdc_lib)

    # adds usb_dual_cdc_lib dependency for networking
    add_subdirectory(pmic_lib)

    ################################################################################
    # creates i2c_bus_scan executable
    add_executable(i2c_bus_scan app/i2c_bus_scan.c)
    target_include_directories(i2c_bus_scan PUBLIC .)
    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(i2c_bus_scan pico_stdlib hardware_i2c)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(i2c_bus_scan 1)
    pico_enable_stdio_uart(i2c_bus_scan 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(i2c_bus_scan)

    ################################################################################
    # creates test_max77654 executable
    add_executable(test_max77654 app/test_max77654.c)
    target_include_directories(test_max77654 PUBLIC .)
    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(test_max77654 pico_stdlib hardware_i2c pmic_lib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(test_max77654 1)
    pico_enable_stdio_uart(test_max77654 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(test_max77654)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()