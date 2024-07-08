gdt_start:

gdt_null: #this is mendatory null descriptor
	.quad 0x0 #8 bytes

gdt_code: #the code segment descriptor
	# base = 0x00
	# limit or size = 0xfffff
	# 1st flags : present - 1, privilege - 00, descriptor type - 1 : 1001
	# type flags : code - 1, conforming - 0, readable - 1, accessed - 0 : 1010
	# 2nd flag : granularity - 1, 32bit default - 1, 64-bit seg - 0, AVL - 0 : 1100
	
	.word 0xffff #limit
	.word 0x0000 #base
	.byte 0x00 	 #base
	.byte 0b10011010 #1st flags, type flags
	.byte 0b11001111 #2nd flags, limit
	.byte 0x0 #base

	/*
	Wonder why we write 0x0(4 bits) in last instead of 0x00(8 bits)?

	This is because our computer system reads 0x0 and 0x00 same as 1 byte.
	But Why?
	
	Because  the smallest addressable unit in most computer architectures is a byte,
	which is 8 bits or 2 hexadecimal digits,
	and each hexadecimal digit represents a nibble(4 bit).

	*/


gdt_data:
	#here type get changed
	# type flags : code - 0, expand down - 0, writable - 1, accessed - 0 -> 0010
	#Also we will keep the base address of data segment same as code segment, as for now we are making them overlapped.

	.word 0xffff
	.word 0x0
	.byte 0x0 
	.byte 0b10010010
	.byte 0b11001111
	.byte 0x0

gdt_end: # this label is only used to calculate the size of GDT for GDT descriptor


gdt_descriptor:
	.word gdt_end - gdt_start - 1 #size of GDT
	# size of our GDT always one less than true size(CHECK INTEL devloper manual for details)
	.long gdt_start		#start address of our GDT

# defining some constant

.equ CODE_SEG, gdt_code - gdt_start
.equ DATA_SEG, gdt_data - gdt_start