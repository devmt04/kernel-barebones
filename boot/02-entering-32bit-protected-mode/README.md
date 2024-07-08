# Protected Mode

* It is 32-bit mode, here we can freely use all 32-bit registers, unlike in real mode, where we were restricted to only 16-bit registers.

* 32-bit memory offsets are available, so an offset can refrence a whopping a 4GB of memory (0xffffffff).

* Here CPU supports a more sophisticated - though slightly more complex means of segmentation, which offers two big advantages:
	
	- We can protect our Kernel code from user applications.
	- The CPU can impliment "virtual memory" for user processes, such that pages(i.e, fixed-sized chunks) of a process's memory can be swapped transparently between disk and memory on an as-needed basis. This ensure main memory is used efficienlty, in that code or data that is rarely executed needn't hog valuable memory.

* Interrupt handling is also more sophisticated.


# Global Descriptor Table (GDT):

While switching from 16-bit real mode to 32-bit Protected mode, we have to prepare a data strcture in memory called GDT, which defines memory segments and their protected mode attributes.

When we successfully created GDT, using a special instruction, we have to load it into our CPU.

Once the CPU has been switched into 32-bit protected mode, the process by which it translates the logical addresses(i.e, the combination of a segment register and an offset) to physical address is completely diffrent: rather than multiply the value of a segment register by 16 and then add to it the offset, <b>a segment resiter becomes an index to a particular segment descriptor(SD) in the GDT.</b>


A segment descriptor is a 8-byte strcture that defines the following properties if a protected-mode segments:

	- Base Address : defines where the segment begins in physical memory.
	- Segment Limit : defines the size of segment.
	- Various flags, which affect how the CPU interprets the segment, such as the privillige level of code that runs within it or whether it is read-or write-only. 

** Now as we are in 32-bit mode, we can no longer use the BIOS functions, just due to the fact that they are coded to work only in 16-bit mode. Using them in 32-bit would likely crash the machine! But we can switch temprory back into 16-bit mode whereupon it may utilise the BIOS functions, but this can be more trouble that it is worth, especially in terms of performance.


The simplest workable configuration of segment registers is described by Intel as the <b><i>Basic Flat Model</i></b>, whereby two overlapping segments are defined that cover the full 4GB of addressable memory, one for code and other for data. The fact that in this model these two segments overlap means that there is no attempt to protect one segment from other, nor there os any attempt to use the paging features for virtual memory.

In Addition to code and data segments, the CPU requires that the first entry in the GDT purposely to be invalid null descriptor, i.e, a strcture of 8 zero bytes. The null descriptor is a simple mechanism to catch mistakes where we forget to set a particular segment registor before accessing an address., which is easily done if we had some segment registors set to 0x0 and forgot to update them to the appropriate segment descriptor after switching to protected mode. If an addressing attempt is made with the null descriptor, them the CPU will raise an exception, which essentially is an interrupt.



## Defining the GDT in Assembly:


Actually the CPU needs to know how long our GDT is, we don't actually directly give the CPU the start address of our GDT but instead give it the address of a much simpler strcture called the GDT descriptor(i.e, something that describe the GDT).


The GDT is a 6-byte(48-bit) strcture contating:

	* GDT size, which is of 20 bits
	* GDT address, which is of 32 bits


General Format of Descriptor:

BASE: It contains the 32-bit base address for a segment. Thus define the location of the segment within the 4 gigabyte ((232 -1)-bit ) linear address space.
    
LIMIT: It defines the size of the segment. The x86 concatenates the two fragments of the limit field to form a 20-bit value. The x86 interprets this 20-bit value in two ways, depending on the setting of the granularity bit(G).

Granularity Bit: It specifies the unit with which the limit field is interpreted. If the G bit is 0, the limit is interpreted in a unit of one byte, else limit is interpreted in a unit of 4 Kb.

O(Reserved by Intel): This neither can be defined nor can be used by the user. This bit must be zero for compatibility with future processors.

U/AVL (User Bit): This bit is completely undefined, and x86 ignores it. This is an available field/bit for the user or operating system.

P (Present Bit): The present P bit is 1 if the segment is loaded in the physical memory, if P = 0 then any attempt to access this segment causes a not-present exception.

DPL (Descriptor Privilege Level): It is a 2-bit field the level of privilege associated with the memory space that the descriptor defines – DPL0 is the most privileged whereas DPL3 is the least privileged.

S (System Bit): The segment S bit in the segment descriptor determines if a given segment is a system segment or a code or a data segment. If the S bit is 1 then the segment is either a code or data segment, if it is 0 then the segment is a system segment.

Type: The specifies the specific descriptor among various kinds of descriptors. 	

A (Accessed Bit): The x86 automatically sets this bit when a selector for the descriptor is loaded into a segment register. This means that x86 sets accessed bit whenever a memory reference is made by accessing the segment.



#### Defining them in code:


We will use following strcutre to define a GDT in our code:

0th to 15th bit : LIMIT (upto its 16 bit from 0th to 15th bit out of 20bit)
16th to 39th bit : BASE address (upto its 24 bit from 0th to 23th bit out of total 32bit)
40th and 43th bit : TYPE
44th bit : S
45th bit and 46th bit : DPL
47th : p
48th to 51th : LIMIT (16th to 19th bit)
52th : U
53rd : O
54th : X
55th : G
56th to 63rd : BASE address (from 24th to 31th bit)



#### Making the Switch:

Now once our GDT and GDT descriptor have been prepared, we are ready to instruct the CPU to switch from 16-bit real mode to 32-bit protected mode.

First thing we have to do is to disable interuppts using the <b>cli</b>(clear interrupt) instruction, which means CPU will simple ignores any future interrupts that may happens, atleast untill interuppts are later enabled. We have to do this to avoid any kind of CPU crash.

Next step is to tell the CPU about the GDT that we just prepared. We will use the single instruction for this:
	
	lgdt [gdt_descriptor]

Now that all in-place we will make the actual switch over, by setting the first bit of a special CPU control register <b>cr0</b>. Now, we cannot set that bit directly on the register, so we must load it into a genrel purpose register, set the bit, then store it back into cr0. For this we will use the <b>or</b> instructionto include certain bits into a value, without disturbing any other bits that, for some important reason, may have been set already in the control register.


	mov %cr0, %eax
	or $0x1, %eax
	mov %eax, %cr0

After cr0 has been updated, the CPU is in 32-bit mode, but there is a issue with modern processor, that is, they use a technique called pipelining, that allows them to process diffrent stages of an instructions's execution in parallel. Now there is a risk that CPU may process some stages of an instruction's execution in wrong mode. So what we need to do, immediately after instructing the CPU to switch node, is to force CPU to finish any jobs in its pipeline, so that we can be confident that all future instructions will be executed in correct mode.

Now pipelining works very well when the CPU knows about the next few instructions that will be coming over the horizon, since it can pre-fetch them, but it doesn't like instructions such as <b>jmp</b> or <b>call</b>, because untill those instructions have been executed fully the CPU can have no idea about the instructions that will follow them, especially if we use a <b>far jump</b> or <b>call</b>, which means, jump to another segment.

So immediately after instructing the CPU to switch mode, we can issue a far jump, which will force the CPU to flush the pipeline i.e, complete all of instructions currently in diffrent stages of the pipeline.

To issue a far jump, we additionally provide the target segment as:

	jmp <segment>:<address offset>
	

btw

	~ jmp is an unconditional jump to a specified location.
    
    ~ call is used to call subroutines or functions and saves the return address to allow the program to return after executing the subroutine.

