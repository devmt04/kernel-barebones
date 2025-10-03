.section .multiboot
	.align 4
	.long  0x1BADB002 #magic number
	.long 0x00 		  #Flags
	.long -(0x1BADB002 + 0x00)  # Checksum

.section .text
	.global _start 

.extern load_gdt
.extern load_idt
.extern main

_start:
 call load_gdt
 call load_idt
 call main
 hlt
