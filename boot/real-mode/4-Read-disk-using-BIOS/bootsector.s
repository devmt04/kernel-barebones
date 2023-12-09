/*
Assemble with:
	as bootsector.s -o bootsector.o
	ld --oformat binary bootsector.o -o bootsector.bin -Ttext 0x7c00


now we run the linker command with "-Ttext 0x7c00" to explicitly let the
assembler know that our bootsector gonna load or start from 0x7c00 memory location.

Now this is crucial, without this we may get reallocation errors, which will
trying to fits address which will be out of range of 16-bit registers.

Also note the:

In GAS, we will use () to load a address from a var/reg to a var/register, like:
	mov %dl, (BOOT_DRIVE) #will load value at dl into memory address in var BOOT_DRIVE
	mov (BOOT_DRIVE), %ax #move address st BOOT_DRIVE  var into a register

But for a direct memory address to a var/regiser, we don;t use (), like:
	mov 0x1234, %ax
*/
	
.code16
start:
	movb %dl, (BOOT_DRIVE)

	movb $0x02, %ah 	#Call disk-read BIOS function

	movb (BOOT_DRIVE), %dl    	#Read drive 0, i.e, first hard drive
	movb $0x00, %ch    		#Select cylinder 0
	movb $0x00, %dh 		#Select track on first side. Count has base 0
	movb $0x02, %cl 		#Select sector 2.Sector 01(0x01) is our boot sector, 0x02 is our 1st availabe sector
	movb $0x01, %al    		#Read 1 sector from the start point
	
	# Lastly, set the address that we'd like BIOS to read the sectors to,
	# Which BIOS expects to find in ES:BX 
	# i.e, segment ES with offset BX

	mov $0x1000, %bx
	mov %bx, %es 	   	#Buffer segment address
	mov $0x1234, %bx 	#Offset address of buffer, from here the data will be stored
	# In our case, data will be read to 0x1000:0x1234, which the CPU will
	# translate to physical address 0xa1234

	int $0x13
	jnc sucessful
	jc disk_error #jc is another jumping instrcutions, that jumps only if the carry flag was set.
	# jnc : jump if no carry flag, i.e, succesfull
	# jc : jump if carry flag, i.e, error

sucessful:
	movb $0x0e, %ah #call bios function to print msg
	movb $'S', %al
	int $0x10	#call bios interuppt to rpint msg

	movb $0x0E, %ah
	// movb $0x42, %al
	
	// mov , %si  # Set SI register to point to the memory address 0x1234
	movb %es:[0x1234], %al   # Load the data at memory location 0x1234 from begeniing of 0x1000
	

	int $0x10
	jmp diskloop

disk_error:
	movb $0x0e, %ah #call bios function to print msg
	movb %ah, %al
	int $0x10	#call bios interuppt to rpint msg
	jmp diskloop

diskloop:
	jmp .


BOOT_DRIVE:
	.byte 0x00

.fill 510 - (. - start), 1, 0

.word 0xaa55
#From _start till here, it takes total 512 bytes, hence a sector is fully occupied
#so anything we write below this, will gonna store in second next sector.

.fill 256,1,0x41
.fill 256,1,0x31 
.fill 256,1,0x35
.fill 256,1,0x31 
.fill 256,1,0x35
.fill 256,1,0x31 
.fill 256,1,0x35
.fill 256,1,0x31 
.fill 256,1,0x35
