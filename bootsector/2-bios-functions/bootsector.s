/*
Assemble with:
	as bootsector.s -o bootsector.o
	ld --oformat binary bootsector.o -o bootsector.bin

*/

.code16

/*
So as soon as the computer starts, the first ever program run is BIOS which is
pre-loaded by manufacturer. The BIOS provides us some functions which help
to do some BASIC INPUT AND OUTPUT operations.

To functions are also called BIOS INTERRUPTS.

Like to Write a Charcter at the current position of Cursor:

	set: AH = 0EH
		 AL = ASCII code of charcter
		 BL = Foreground color of charcter (only graphic mode)
	and then call the BIOS by Interuppt 10H by
		int $0x10

This is Interuppt 10h (Video Functions) and function 0Eh (write a charcter)

You can get the list of such BIOS interupt online easily
Or get a list at "https://ostad.nit.ac.ir/payaidea/ospic/file1615.pdf"
*/


start:
	movb $0x0e, %ah #call bios function
	
	movb $42, %al 
	int $0x10	#call bios interuppt

	movb $'A', %al
	int $0x10

jmp .	#infinite loop

.fill 510-(. - start),1,0 

.word 0xaa55
