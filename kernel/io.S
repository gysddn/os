/*
 * I/O Management Source File
 * @gysddn <gysddn@gmail.com>
 *
 */

.globl read_port_byte
read_port_byte:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  in    %dx, %al
  pop   %ebp
  ret

.globl read_port_word
read_port_word:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  in    %dx, %ax
  pop   %ebp
  ret

.globl read_port_dword
read_port_dword:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  in    %dx, %eax
  pop   %ebp
  ret

.globl write_port_byte
write_port_byte:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  mov   12(%ebp), %al
  out   %al, %dx
  pop   %ebp
  ret

.globl write_port_word
write_port_word:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  mov   12(%ebp), %ax
  out   %ax, %dx
  pop   %ebp
  ret

.globl write_port_dword
write_port_dword:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  mov   12(%ebp), %eax
  out   %eax, %dx
  pop   %ebp
  ret
