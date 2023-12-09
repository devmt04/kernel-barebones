# Segmentation:

In real mode, we use only 16-bit registers, i.e, we are limited to store and process the data(this data can also be address of some memory block) upto 16-bit-at-a-time only.

In a 16-bit register, you can store 16 bits of data. Each bit can represent one of two values: 0 or 1. Therefore, a 16-bit register can hold 2^16 (or 65,536) different binary values..

So if the maximum size of register is 16-bit, it means that the highest address we can refrenced using it in an instruction is 0xffff

Now we can overcome this limitation using Segmentation. Segmentation in real mode is a memory management mechanism used in x86-based CPUs from the early days of computing.

To get around this limitation, the CPU designers added a few more special registers. cs, ds, ss, and ss. they are called segmentation registers. We can imagine main memory as being divided into segments that are indexed by segment registers, such that when we specify a 16-bit address, the CPU automatically calculated the absolute address as the appropriate segment's start address offseted by out specified address.


CS - Code Segment
DS - Data Segment
SS - Stack Segment
ES - Extra Segment

FS, GS - General Purpose Segments

DS (Data Segment): The DS register is typically used for addressing data segments. It specifies the base address of the segment from which data is fetched. When you access variables or data in memory, the DS register is used in conjunction with the offset to calculate the physical address.

CS (Code Segment): The CS register, as mentioned earlier, specifies the base address of the code segment. It is used for program execution. The CS register, when combined with the IP register (Instruction Pointer), determines the memory address from which the CPU fetches instructions to execute.

SS (Stack Segment): The SS register is used to specify the base address of the stack segment. When the CPU needs to push or pop data onto or from the stack, the SS register is used to calculate the memory address of the stack segment. The stack is used for function call and return addresses, local variables, and more. The BP(base pointer) sets by default to the location specified in SS.

ES (Extra Segment): The ES register, often considered a general-purpose segment, can be used to address additional data segments as needed. It doesn't have a specific predefined role and can be used for various data storage purposes


In x86 real mode, when the CPU is powered on or reset, the default values of the segment registers are typically set as follows:

    CS (Code Segment): The default value of the CS register is usually 0xFFFF (in hexadecimal). This points to the top of memory, typically 0xFFFF0, which is where the BIOS startup code is located. The offset, in this case, would be 0x0000.

    DS (Data Segment): The DS register's default value is also typically set to 0xFFFF.

    SS (Stack Segment): The SS register usually has a default value of 0xFFFF as well.

    ES (Extra Segment): The ES register often has a default value of 0xFFFF, like the other segment registers.

These default values set the segment registers to the top of the 1 MB memory space, allowing the CPU to access the BIOS code and data structures at startup. Program execution starts at the offset 0x0000 within the code segment

In x86 assembly programming, especially in real mode, memory addressing is based on a combination of a segment and an offset, which together form a physical memory address. Here's a brief explanation of these terms:

Segment: In the x86 architecture, a segment is a 16-bit value that specifies a segment of memory. Segments are used to break the memory into distinct blocks or segments. Each segment is 64 kilobytes (KB) in size. The segment register (e.g., ES, CS, DS, SS) holds the segment value, and it is shifted left by 4 bits to form a 20-bit address.
For example, if the segment register ES holds the value 0x1000, it points to a specific 64 KB segment of memory.

Offset: The offset is another 16-bit value that specifies a location within the segment. The offset addresses individual bytes within the segment. You can think of it as the distance from the start of the segment to the specific byte you want to access.
For example, if the offset value is 0x1234, it specifies the position 0x1234 bytes from the start of the segment pointed to by the segment register.

Physical Address: The physical memory address is formed by combining the segment and offset values. You multiply the segment value by 16 and then add the offset value to get the physical memory address.
For instance, if ES is set to 0x1000 and BX is set to 0x1234, the physical memory address would be (0x1000 * 16) + 0x1234 = 0x10000 + 0x1234 = 0x11234.