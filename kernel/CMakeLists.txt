set(KERNEL_SOURCES
    io.c
    kernel.c
    serial.c
    util.c
    vga.c
    )

include_directories(include)

add_compile_options(-ffreestanding)
add_compile_options(-masm=att)

add_link_options(-nostartfiles)
add_link_options(-nostdlib)
add_link_options(-nostdinc)

add_executable(kernel ${KERNEL_SOURCES})
