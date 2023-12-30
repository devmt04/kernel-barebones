# Protected Mode

* It is 32-bit mode, herr we can freely use all 32-bit registers, unline in real mode, where we were restricted to only 16-bit registers.

* 32-bit memory offsets are available, so an offset can refrence a whopping a 4GB of memory (0xffffffff).

* Here CPU supports a more sophisticated - though slightly more complex means of segmentation, which offers two big advantages:
	
	- We can protect our Kernel code from user applications.
	- The CPU can impliment "virtual memory" for user processes, such that pages(i.e, fixed-sized chunks) of a process's memory can be swapped transparently between disk and memory on an as-needed basis. This ensure main memory is used efficienlty, in that code or data that is rarely executed needn't hog valuable memory.

* Interrupt handling is also more sophisticated.


# Global Descriptor Table (GDT):

While switching from 16-bit real mode to 32-bit Protected mode, we have to prepare a data strcture in memory called GDT, which defines memory segments and their procted mode attributes.

When we successfully created GDT, using a special instruction, we have to load it into our CPU.

Once the CPU has been switched into 32-bit protected mode, the process by which it translates the logical addresses(i.e, the combination of a segment register and an offset) to physical address is completely diffrent: rather than multiply the value of a segment register by 16 and then add to it the offset, <b>a segment resiter becomes an indexc to a particular segment descriptor(SD) in the GDT.</b>


A segment descriptoy is a 8-byte strcture that defines the following properties if a protected-mode segments:

	- Base Address (32 bits) : defines where the segment begins in physical memory.
	- Segment Limit (20 bits) : defines the size of segment.
	- Various flags, which affect how the CPU interprets the segment, such as the privillige level of code that runs within it or whether it is read-or write-only. 

** Now as we are in 32-bit mode, we can no longer use the BIOS functions, just due to the fact that they are coded to work only in 16-bit mode. Using them in 32-bit would likely crash the machine! But we can switch temprory back into 16-bit mode whereupon it may utilise the BIOS functions, but this can be more trouble that it is worth, especially in terms of performance.

