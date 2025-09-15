# SEGMENT DESCRIPTORS

## Overview

On the IA-32 and x86-64 architectures, and more precisely in Protected Mode or Long Mode, Interrupt Service Routines and a good deal of memory management are controlled through tables of descriptors. Each descriptor stores information about a single object (e.g. a service routine, a task, a chunk of code or data) the CPU might need at some time.

In real mode, memory is accessed using a combination of segment registers and offsets. For example, the CS register holds the code segment base, and the CPU calculates the physical address as CS * 16 + IP.

When we switch to protected mode, segment registers no longer directly store base addresses. Instead, they hold selectors that index into the Global Descriptor Table (GDT) or Local Descriptor Table (LDT). The GDT contains descriptors, which define each segment's base address, size, and access rights.

After entering protected mode, we must load new selectors into the segment registers so they point to valid descriptors in the GDT, giving us a proper memory model for protected mode.


## Memory Management Models

There are basically three memory management model. 

1. Flat Model (Linear Addressing)
2. Real-Address Mode Model (Segment Selector + offset)
3. Segmented Model

- **Flat Memory Model** : Memory appears to a program as a single, continuous address space. This space is called a linear address space. Code, data, and stacks are all contained in this address space. When using the flat (unsegmented) memory model, segment registers are loaded with segment selectors that point to overlapping segments, each of which begins at address 0 of the linear address space. These overlapping segments then comprise the linear address space for the program. Typically, two overlapping segments are defined: one for code and another for data and stacks. The CS segment register points to the code segment and all the other segment registers point to the data and stack segment.

- **Real-address mode memory mode** : This is the memory model for the Intel 8086 processor. The real-address mode uses a specific implementation of segmented memory in which the linear address space for the program and the operating system/executive consists of an array of segments of up to 64 KBytes in size each. 

- **Segmented memory model** : Memory appears to a program as a group of independent address spaces called segments. Code, data, and stacks are typically contained in separate segments. To address a byte in a segment, a program issues a logical address. This consists of a **segment selector** and an offset. When using the segmented memory model, each segment register is ordinarily loaded with a different segment selector so that each segment register points to a different segment within the linear address space.

**Note** :  In Real Address Mode, a 16-bit segment selector is a physical base address. In Protected Mode, a segment selector follow an entirely different format, which selects a segment of the user's virtual address space.

Format of segment selector in protected mode:

- bit 15 to bit 3 : INDEX - Index into Descriptor Table (GDT or LDT). CPU multiplies this by 8 (size of descriptor) and adds the result to the base address of the descriptor table to find the appropriate segment descriptor entry.
- bit 2 : TI - Table Indicator, Selects which table to use: 0 = GDT, 1 = LDT
- bit 1 to bit 0 : RPL - Requested Privilege Level, Used for privilege checks.

## VIRTUAL-TO-PHYSICAL ADDRESS TRANSLATION

The translation of a full 32-bit virtual address pointer(selector+offset) is done into a real 24-bit(in Intel 80286) physical address. When the segment's base address is determined as a result of the mapping process, the offset value is added to the result to obtain the physical address.

The actual mapping is performed on the selector component of the virtual address. The 16-bit segment selector is mapped to a 24-bit segment base address via a segment descriptor maintained in one of the **descriptor tables**.

**Note** : In chips like Intel 80286, there were only 24 physical address lines. However modern chips has more address lines present. So in Intel 80286, however there was 32-bit virtual address space, only first 16 MiB is actually usable because of 24 address lines.

## Virtual Address Space

The entirety of a program's virtual address space is subdivided into two separate halves, as distinguished by the TI ("table indicator") bit in the virtual address. These two halves are the **global address space** and the **local address space**.

The global address space is used for system-wide data and procedures including operating system software, library routines, runtime language support and other commonly shared system services. 

The other half of the virtual address space comprising those addresses with the TI bit set-is separately mapped for each task in the system. Because such an address space is local to the task for which it is defined, it is referred to as a local address space. In general, code and data segments within a task's local address space are private to that particular task or user.


## System Address Registers

- **Global Descriptor Table Register (GDTR)** : is a dedicated 40 bit (5 byte) register used to record
the base and size of a system's global descriptor table (GDT). Thus, two of these bytes define the size of the GDT, and three bytes define its base address.

The register contents are "hidden" only in the sense that they are not accessible by means of ordinary instructions. Instead, the dedicated protected instructions **LGDT** and **SGDT** are reserved for loading and storing, respectively, the contents of the GDTR at Protected Mode initialization.


- **Local Descriptor Table Register (LDTR)** : is a dedicated 40-bit register that contains, at any given moment, the base and size of the local descriptor table (LDT) associated with the currently executing task. 

Unlike GDTR, the LDTR register contains both a "visible" and a "hidden" component. Only the visible component is accessible, while the hidden component remains truly inaccessible even to dedicated instructions.

The visible component of the LDTR is a 16·bit "selector" field. The format of these 16 bits corresponds exactly to that of a segment selector in a virtual address pointer. Thus, it contains a 13·bit INDEX field, a l·bit TI field, and a 2·bit RPL field. The TI "table indicator" bit must be zero, indicating a reference to the GDT (i.e., to global address space). The INDEX field consequently provides an index to a particular entry within the GDT. This entry, in,turn, must be an LDT descriptor (or descriptor table descriptor), as defined in the previous section. In this way, the visible "selector" field of the LDTR, by selecting an LDT descriptor, uniquely designates a particular LDT in the system.

The dedicated, protected instructions __LLDT__ and __SLDT__ are reserved for loading and storing, respectively, the visible selector component of the LDTR register.

The processor locates the GDT and the current LDT in memory by means of the GDTR and LDTR registers. These registers store the base addresses of the tables in the linear address space and store the segment limits. The instructions LGDT and SGDT give access to the GDTR; the instructions LLDT and SLDT give access to the LDTR.

## Descriptors

The segment descriptor provides the processor with the data it needs to map a logical address into a linear address.

Segment-descriptor fields are:

- BASE: Defines the location of the segment within the 4 gigabyte linear address space. The processor concatenates the three fragments of the base address to form a single 32-bit value.

- LIMIT: Defines the size of the segment. When the processor concatenates the two parts of the limit field, a 20-bit value results. The processor interprets the limit field in one of two ways, depending on the setting of the granularity bit:

1. In units of one byte, to define a limit of up to 1 megabyte.

2. In units of 4 Kilobytes, to define a limit of up to 4 gigabytes. The limit is shifted left by 12 bits when loaded, and low-order one-bits are inserted.

- Granularity bit: Specifies the units with which the LIMIT field is interpreted. When the bit is clear, the limit is interpreted in units of one byte; when set, the limit is interpreted in units of 4 Kilobytes.

- TYPE: Distinguishes between various kinds of descriptors.

- DPL (Descriptor Privilege Level): Used by the protection mechanism.

- Segment-Present bit: If this bit is zero, the descriptor is not valid for use in address transformation; the processor will signal an exception when a selector for the descriptor is loaded into a segment register.

- Accessed bit: The processor sets this bit when the segment is accessed; i.e., a selector for the descriptor is loaded into a segment register or used by a selector test instruction.


#### DESCRIPTOR DATA STRUCTURE

0th to 15th bit : LIMIT (upto its 16 bit from 0th to 15th bit out of 20bit)

16th to 39th bit : BASE address (upto its 24 bit from 0th to 23th bit out of total 32bit)

40th and 43th bit : TYPE

44th bit : S

45th bit and 46th bit : DPL

47th : P

48th to 51th : LIMIT (16th to 19th bit)

52th : U

53rd : O

54th : X

55th : G

56th to 63rd : BASE address (from 24th to 31th bit)


---

- O(Reserved by Intel): This bit must be zero for compatibility with future processors.

- U/AVL (User Bit): This bit is completely undefined, and x86 ignores it.

- P (Present Bit): The present P bit is 1 if the segment is loaded in the physical memory, if P = 0 then any attempt to access this segment causes a not-present exception.

- DPL (Descriptor Privilege Level): It is a 2-bit field the level of privilege associated with the memory space that the descriptor defines – DPL0 is the most privileged whereas DPL3 is the least privileged.

- S (System Bit): The segment S bit in the segment descriptor determines if a given segment is a system segment or a code or a data segment. If the S bit is 1 then the segment is either a code or data segment, if it is 0 then the segment is a system segment.

- A (Accessed Bit): The x86 automatically sets this bit when a selector for the descriptor is loaded into a segment register. This means that x86 sets accessed bit whenever a memory reference is made by accessing the segment.


## DESCRIPTOR TABLES

The descriptor table contents govern the interpretation of virtual addresses.

Within a **Protected Mode system**, there are ordinarily several descriptor tables resident in memory. One of these is the global descriptor table (GDT); this table provides a complete description of the global address space. In addition, there may be one or more local descriptor tables (LDTs), each describing the local address space of one or more tasks.

For each task in the system, a pair of descriptor tables-consisting of the GDT (shared by all tasks) and a particular LDT (private to the task or to a group of closely related tasks)-provides a complete description of that task's virtual address space. The protection mechanism ensures that a task is granted access only to its own virtual address space. In the simplest of system configurations, tasks can reside entirely within the GDT without the use of local descriptor tables. This will simplify system software by only requiring maintenance of one table (the GDT) at the expense of no isolation between tasks.

A descriptor table is simply a memory array of 8-byte entries called descriptors. A descriptor table may contain from 1 to 8192 entries. The first entry of the GDT (INDEX=0) is not used by the processor.

Because the first entry of the GDT is not used by the processor, a selector that has an index of zero and a table indicator of zero (i.e., a selector that points to the first entry of the GDT), can be used as a null selector.


#### Basic GDT in GAS Assembly

```
gdt_start:

gdt_null: #null descriptor
   .quad 0x0 #8 bytes

gdt_code: #the code segment descriptor
   # base = 0x00
   # limit or size = 0xfffff
   # 1st flags : present - 1, privilege - 00, descriptor type - 1 : 1001
   # type flags : code - 1, conforming - 0, readable - 1, accessed - 0 : 1010
   # 2nd flag : granularity - 1, 32bit default - 1, 64-bit seg - 0, AVL - 0 : 1100
   
   .word 0xffff #limit
   .word 0x0000 #base
   .byte 0x00   #base
   .byte 0b10011010 #1st flags, type flags
   .byte 0b11001111 #2nd flags, limit
   .byte 0x0 #base


gdt_data:
   # type flags : code - 0, expand down - 0, writable - 1, accessed - 0 -> 0010

   .word 0xffff
   .word 0x0
   .byte 0x0 
   .byte 0b10010010
   .byte 0b11001111
   .byte 0x0

gdt_end: # use to calculate size of GDT


gdt_descriptor:
   .word gdt_end - gdt_start - 1 #size of GDT
   .long gdt_start #start address of our GDT

```

## Loading GDT into GDTR

As we are using GRUB, GRUB already built a minimal GDT for us. In order to overwrite it, we need to again, load `gdt_descriptor` with `lgdt` instruction, and update all segment registers with new entries.

To load `gdtr` with a new `gdt_descriptor`, we have to follow the same approach, that was initially followed(by GRUB) while switching to Protected Mode from Real Mode.

The Procedure was:

Setting the PE bit of the MSW in CR0, results the 80386 to begin executing in protected mode. The current privilege level (CPL) starts at zero. The segment registers continue to point to the same linear addresses as in real address mode (in real address mode, linear addresses are the same physical addresses)

Immediately after setting the PE flag, the initialization code must flush the processor's instruction prefetch queue by executing a JMP instruction. The 80386 fetches and decodes instructions and addresses before they are used; however, after a change into protected mode, the prefetched instruction information (which pertains to real-address mode) is no longer valid. A JMP forces the processor to discard the invalid information.


```
WHY ISSUING A JMP?
''''''''''''''''''

After cr0 has been updated, the CPU is in 32-bit mode, but there is a issue with modern processor, that is, they use a technique called pipelining, that allows them to process different stages of an instructions execution in parallel. Now there is a risk that CPU may process some stages of an instruction's execution in wrong mode. So what we need to do, immediately after instructing the CPU to switch node, is to force CPU to finish any jobs in its pipeline, so that we can be confident that all future instructions will be executed in correct mode.

Now pipelining works very well when the CPU knows about the next few instructions that will be coming over the horizon, since it can pre-fetch them, but it doesn't like instructions such as <b>jmp</b> or <b>call</b>, because until those instructions have been executed fully the CPU can have no idea about the instructions that will follow them, especially if we use a <b>far jump</b> or <b>call</b>, which means, jump to another segment.

So immediately after instructing the CPU to switch mode, we can issue a far jump, which will force the CPU to flush the pipeline i.e, complete all of instructions currently in different stages of the pipeline.
```

However PE bit is already set, all we need to do now is:

- load `gdt_descriptor` with `lgdt`
- issue a far jump
- update all segment registers with new segment values 


```
load_gdt:
   lgdt gdt_descriptor

   mov $0x10, %ax
   mov %ax, %ds  ; 0x10 is the offset in the GDT to our data segment
   mov %ax, %ss
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   jmp $0x08, $.flush ; 0x08 is the offset to our code segment: Far jump!

.flush:
   ret

```

The CS register doesn't have to be touched, because our jump fills it with proper segment values







## References

https://wiki.osdev.org/GDT_Tutorial

https://pdos.csail.mit.edu/6.828/2018/readings/i386.pdf - INTEL 80386 PROGRAMMER'S REFERENCE MANUAL 1986

https://bitsavers.trailing-edge.com/components/intel/80286/210498-005_80286_and_80287_Programmers_Reference_Manual_1987.pdf  - INTEL 80286 AND 80287 PROGRAMMER'S REFERENCE MANUAL 1987

https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html  -  Intel® 64 and IA-32 Architectures Software Developer Manuals

