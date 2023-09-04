cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(random_flashing C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

# Include the display library
add_subdirectory(./display/Config)
add_subdirectory(./display/Infrared)
add_subdirectory(./display/OLED)
add_subdirectory(./display/Icm20948)
add_subdirectory(./display/Fonts)
add_subdirectory(./display/GUI)
include_directories(./display/Config)
include_directories(./display/GUI)
include_directories(./display/Icm20948)
include_directories(./display/Infrared)
include_directories(./display/OLED)

add_executable(main
  src/main.c
)

target_link_libraries(main
  # Include standard libraries
  pico_stdlib
  pico_cyw43_arch_none

  # Include the display library
  OLED
  Infrared
  Icm20948
  GUI
  Fonts
  Config
  pico_stdlib
  hardware_spi
)

pico_add_extra_outputs(main)
