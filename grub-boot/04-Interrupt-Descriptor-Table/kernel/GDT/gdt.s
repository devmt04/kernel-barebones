.global load_gdt
gdt_start:
   gdt_null:
      .quad 0x0 #8 bytes
   gdt_code:
      .word 0xffff
      .word 0x0000
      .byte 0x00
      .byte 0b10011010
      .byte 0b11001111
      .byte 0x0
   gdt_data:
      .word 0xffff
      .word 0x0
      .byte 0x0 
      .byte 0b10010010
      .byte 0b11001111
      .byte 0x0
gdt_end:

gdt_descriptor:
   .word gdt_end - gdt_start - 1
   .long gdt_start

load_gdt:
   lgdt gdt_descriptor
   mov $0x10, %ax
   mov %ax, %ds
   mov %ax, %ss
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   jmp $0x08, $.flush

.flush:
   ret
