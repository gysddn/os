set(KERNEL_SOURCES
    io.cpp
    serial.cpp
    util.cpp
    vga.cpp
    kernel.cpp
    gui/window.cpp
    )

include_directories(include)

add_compile_options(-ffreestanding)
add_compile_options(-masm=att)

add_compile_options(-Wall)
# force coloring on warnings and errors
add_compile_options(-fdiagnostics-color)

add_link_options(-nostartfiles)
add_link_options(-nostdlib)
add_link_options(-nostdinc)
add_link_options(-ekernel_main)

add_executable(kernel ${KERNEL_SOURCES})
