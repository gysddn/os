.set magic, 0x1BADB002
.set flags, 0
.set chekcsum, -(magic + flags)

.section .multiboot
  .align 4
  .long magic
  .long flags
  .long chekcsum

.section .text
.globl _start
_start:
  call kernel_main
  jmp .
.size _start, . - _start
