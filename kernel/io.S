/*
 * I/O Management Source File
 * @gysddn <gysddn@gmail.com>
 *
 */

.globl in8
in8:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  in    %dx, %al
  pop   %ebp
  ret

.globl in16
in16:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  in    %dx, %ax
  pop   %ebp
  ret

.globl in32
in32:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  in    %dx, %eax
  pop   %ebp
  ret

.globl out8
out8:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  mov   12(%ebp), %al
  out   %al, %dx
  pop   %ebp
  ret

.globl out16
out16:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  mov   12(%ebp), %ax
  out   %ax, %dx
  pop   %ebp
  ret

.globl out32
out32:
  push  %ebp
  mov   %esp, %ebp
  mov   8(%ebp), %dx
  mov   12(%ebp), %eax
  out   %eax, %dx
  pop   %ebp
  ret
