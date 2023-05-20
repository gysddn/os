cmake_minimum_required(VERSION 3.16)

project(OS C CXX ASM)

set(ARCH i686-elf)
set(TOOLCHAIN_ROOT /opt/i686-elf)
set(TOOLCHAIN_PATH ${TOOLCHAIN_ROOT}/bin)
set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PATH}/${ARCH}-)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}gcc-ar)
set(CMAKE_NM ${TOOLCHAIN_PREFIX}gcc-nm)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}gcc-ranlib)
set(CMAKE_STRIP ${TOOLCHAIN_PREFIX}strip)
set(CMAKE_CXXFILT ${TOOLCHAIN_PREFIX}c++filt)

set (CMAKE_CXX_STANDARD 20)

set(LINKER_SCRIPT ${SOURCE_DIR}/linker.ld)

set(GCC_VERSION "11.2.0")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(boot)
add_subdirectory(kernel)

target_link_libraries(kernel -T${LINKER_SCRIPT} boot)
set_target_properties(kernel PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_target(generate-sysroot ALL
  COMMAND "${CMAKE_COMMAND}" -E env "mkdir" "-p" "${CMAKE_BINARY_DIR}/sysroot/boot/grub"
  COMMAND "${CMAKE_COMMAND}" -E env "cp" "${CMAKE_BINARY_DIR}/kernel/kernel" "${CMAKE_BINARY_DIR}/sysroot/boot"
  COMMAND "${CMAKE_COMMAND}" -E env "cp" "${SOURCE_DIR}/boot/grub.cfg" "${CMAKE_BINARY_DIR}/sysroot/boot/grub"
  BYPRODUCTS "${CMAKE_BINARY_DIR}/sysroot"
  USES_TERMINAL
  DEPENDS kernel
)

add_custom_target(grub-image ALL
  COMMAND "${CMAKE_COMMAND}" -E env "grub-mkrescue" "-o" "${CMAKE_BINARY_DIR}/os.iso" "${CMAKE_BINARY_DIR}/sysroot"
    BYPRODUCTS "${CMAKE_BINARY_DIR}/grub_image.iso"
    USES_TERMINAL
    DEPENDS generate-sysroot
)

add_custom_target(limine-image ALL
  COMMAND "${CMAKE_COMMAND}" -E env "OS_BUILD_DIR=${CMAKE_BINARY_DIR}" "LIMINE_CFG=${CMAKE_CURRENT_SOURCE_DIR}/boot/limine.cfg" "${CMAKE_CURRENT_SOURCE_DIR}/meta/make_iso.sh"
    BYPRODUCTS "${CMAKE_BINARY_DIR}/limine_image.iso"
    USES_TERMINAL
    DEPENDS kernel
)


add_custom_target(compile-commands ALL
  COMMAND "${CMAKE_COMMAND}" -E env "cp" "${CMAKE_BINARY_DIR}/compile_commands.json" "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json"
    BYPRODUCTS "${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json"
    USES_TERMINAL
)
