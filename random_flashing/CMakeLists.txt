cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(random_flashing C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

if (TARGET tinyusb_device)
  add_executable(main
    main.c
  )

  target_link_libraries(main 
    pico_stdlib
    pico_cyw43_arch_none
  )

  pico_enable_stdio_usb(main 1)
  pico_enable_stdio_uart(main 0)

  pico_add_extra_outputs(main)

  target_link_libraries(main pico_stdlib)
elseif(PICO_ON_DEVICE)
  message(WARNING "not building random_flashing because TinyUSB submodule is not initialized in the SDK")
endif()