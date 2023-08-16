# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.23)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(rpi_europe_ctrl C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
set(PICO_BOARD pico)

pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(rpi_europe_ctrl src/main.c)
target_compile_options(rpi_europe_ctrl PRIVATE -g3 -O0)
target_link_options(rpi_europe_ctrl PRIVATE -g3 -O0)

pico_set_program_name(rpi_europe_ctrl "rp2040-europe-autopilot-control")
pico_set_program_version(rpi_europe_ctrl "0.2")


target_link_libraries(rpi_europe_ctrl
        pico_stdlib
        hardware_pwm
        hardware_pio
        hardware_timer
        hardware_watchdog
        hardware_clocks
        )

pico_generate_pio_header(rpi_europe_ctrl ${CMAKE_CURRENT_LIST_DIR}/src/ppm.pio)

target_include_directories(rpi_europe_ctrl PRIVATE ${CMAKE_SOURCE_DIR}/src)
#target_include_directories(rpi_europe_ctrl PRIVATE ${CMAKE_SOURCE_DIR}/generated)
pico_add_extra_outputs(rpi_europe_ctrl)
