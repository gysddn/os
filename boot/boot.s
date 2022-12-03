.set magic, 0x1BADB002
.set flags, 4 #enable video mode
.set chekcsum, -(magic + flags)

.section .multiboot
  .align 4
  .long magic
  .long flags
  .long chekcsum

  # unused fields
  .long 0, 0, 0, 0, 0

  # graphics mode
  .long 0     # mode_type
  .long 0     # width
  .long 0     # height
  .long 32    # depth

.section .text
.globl _start
_start:
  call kernel_main
  jmp .
.size _start, . - _start
