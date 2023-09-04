cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(random_flashing C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(lib/Config)
add_subdirectory(lib/Infrared)
add_subdirectory(lib/OLED)
add_subdirectory(lib/Icm20948)
# add_subdirectory(lib/LCD) # Not needed for OLED screens
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)
include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/Icm20948)
# include_directories(./lib/LCD) # Not needed for OLED screens
include_directories(./lib/Infrared)
include_directories(./lib/OLED)

if (TARGET tinyusb_device)
  add_executable(main
    main.c
  )

  target_link_libraries(main 
    pico_stdlib
    pico_cyw43_arch_none
  )

  pico_enable_stdio_usb(main 0) # Set second argument to 1 to enable USB serial
  pico_enable_stdio_uart(main 0)

  pico_add_extra_outputs(main)

  target_link_libraries(main pico_stdlib
    # LCD # Not needed for OLED screens
    OLED
    Infrared
    Icm20948
    GUI
    Fonts
    Config
    pico_stdlib
    hardware_spi
  )
elseif(PICO_ON_DEVICE)
  message(WARNING "not building random_flashing because TinyUSB submodule is not initialized in the SDK")
endif()