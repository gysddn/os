set(KERNEL_SOURCES
    io.cpp
    serial.cpp
    util.cpp
    vga.cpp
    kernel.cpp
    )

include_directories(include)

add_compile_options(-ffreestanding)
add_compile_options(-masm=att)

add_link_options(-nostartfiles)
add_link_options(-nostdlib)
add_link_options(-nostdinc)
add_link_options(-ekernel_main)

add_executable(kernel ${KERNEL_SOURCES})
