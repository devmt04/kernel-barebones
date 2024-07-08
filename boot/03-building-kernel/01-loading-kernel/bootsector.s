.code16
start:
	mov %dl, (BOOT_DRIVE)
  mov $0x9000, %bp 
  mov %bp, %sp
	call load_kernel

	call switch_to_pm


load_kernel:
    movw $KERNEL_OFFSET, %bx 
    movb $1, %dh
    movb (BOOT_DRIVE), %dl
    call disk_load
    ret


switch_to_pm:
	cli
	lgdt gdt_descriptor

	mov %cr0, %eax
	or 0x1, %eax
	mov %eax, %cr0

	jmp $CODE_SEG, $init_pm 

.include "GDT/gdt.s"
.include "disk/disk.s"

.code32
init_pm:
  mov $DATA_SEG, %ax
  mov %ax, %ds
  mov %ax, %ss
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  call BEGIN_PM

BEGIN_PM:
	call KERNEL_OFFSET
	jmp .

.equ KERNEL_OFFSET, 0x1000
BOOT_DRIVE: .byte 0x00

.fill 510-(. - start),1,0 
.word 0xaa55
