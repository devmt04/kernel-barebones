/*
To jump into 32-bit mode:

1. Disable interrupts using %cli (clear intrrupt)
2. Load our GDT_DESCRIPTOR 
3. Set a bit on the CPU control register %cr0
4. Flush the CPU pipeline by issuing a carefully crafted far jump
5. Update all the segment registers
6. Update the stack
7. Call to a well-known label which contains the first useful code in 32 bits



Clearing the prefetch queue
```````````````````````````
By setting the PE bit in the CR0 register, we have just switched to protected mode.
This means that all instructions are now in 32-bit format.
As a result, some of them are encoded differently. 
Some instructions may take up more bytes in their binary form, some others maybe less, 
and other still remain unchanged. 
At any rate, we can’t continue executing any more code just yet, 
because of the prefetch queue.

You see, CPUs are built to be fast. 
One of the tricks of the trade that make CPUs ever faster is to have the CPU load a 
range of instructions from memory to be executed at the same time, rather than just one. 
This is called prefetching. After all, the CPU in the Intel 80386 processor 
can read 4 bytes (32 bits) at the same time from memory, and that might well be 
more than one instruction. For technical reasons, even more might be read and decoded 
before it’s actually executed by the CPU.

The consequence of this is that the CPU may have read some instructions from memory 
when it was still in 16-bits mode, decoded them, and is now ready to execute them. 
They won’t work, because the processor is now in protected 32-bits mode!

Luckily, there is trick to make the processor discard the instructions 
it has already prefetched, and that trick is jumping. Whenever the processor 
encounters a jump instruction, any instructions it had read past that instructions 
become worthless and must be discarded. Consequently, "jumping clears the prefetch queue:"



Now it's a thing to NOTE that while changing Control Register %cr0, we are using a 64-bit %RAX
This is because control registers can contain 64-bit values, and attempting to 
write a 32-bit value into a 64-bit register like EAX or RAX ensures that you're
not truncating any bits.


Actually setting control register like that will enable A20 lines, which are one of the 
electrical lines that make up the system bus of an x86-based computer system, and 
used to address memory beyond the 1 MB limit. It was originally imposed by the design of 
the Intel 8086 and 8088 processors.


  Linear address range (hex)	  Memory type			    Use
	0 - 3FF						RAM	Real mode, 		IVT (Interrupt Vector Table)
	400 - 4FF					RAM	BDA 			(BIOS data area)
	500 - 9FFFF					RAM	Free memory, 	7C00 used for boot sector
	A0000 - BFFFF				Video RAM			Video memory
	C0000 - C7FFF				Video ROM			Video BIOS
	C8000 - EFFFF				?					BIOS shadow area
	F0000 - FFFFF				ROM					System BIOS
(src - http://www.osdever.net/tutorials/view/the-world-of-protected-mode)

*/

.code16

switch_to_pm:
	cli #switch off interuppt
	lgdt [gdt_descriptor]

	// When you're working with control registers like CR0 in x86 assembly language, 
	// it's crucial to use a 64-bit register like EAX on 64-bit systems or RAX on 64-bit systems. 
	// See top comments for more info.
	mov %cr0, %eax
	or 0x1, %eax
	mov %eax, %cr0

	jmp $CODE_SEG, $init_pm 
	//The label in this case needs to be preceded by a $
	//far jump with offset into GDT, pointing at a 32bit PM code segment descriptor


.code32
// Now in 32-bit mode, our old segments are meaningless, and also we can't use BIOS Interrupts anymore!
init_pm:
  //Then we need to fill the segment registers with proper segment values.
  //There are six segment registers: CS, SS, DS, ES, FS and GS.
  //The CS register doesn't have to be touched, because our jump fills 
  //it with proper segment values. The SS and DS registers are the most important ones. 
  //The first is the Stack segment and the other is the Data segment,
  // where our variables are located. We just load these with our data segment.
  
  mov $DATA_SEG, %ax
  mov %ax, %ds
  mov %ax, %ss
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  //Update our stack pos so it is right at top of free space
  //For Liner address range of memory, see table in top comments.
  mov $0x90000, %ebp 
  mov %ebp, %esp

  call BEGIN_PM


BEGIN_PM:
	mov $'P', [0xB8000] //You will se a P with white foreground color and Blue Background color
						//We will discuss it, further.
	mov $0x1B, [0xB8001]
	jmp .

.include "gdt.s"

.fill 510-(. - switch_to_pm),1,0 
.word 0xaa55
