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


## DESCRIPTOR TABLES

The descriptor table contents govern the interpretation of virtual addresses.

Within a **Protected Mode system**, there are ordinarily several descriptor tables resident in memory. One of these is the global descriptor table (GDT); this table provides a complete description of the global address space. In addition, there may be one or more local descriptor tables (LDTs), each describing the local address space of one or more tasks.

For each task in the system, a pair of descriptor tables-consisting of the GDT (shared by all tasks) and a particular LDT (private to the task or to a group of closely related tasks)-provides a complete description of that task's virtual address space. The protection mechanism ensures that a task is granted access only to its own virtual address space. In the simplest of system configurations, tasks can reside entirely within the GDT without the use of local descriptor tables. This will simplify system software by only requiring maintenance of one table (the GDT) at the expense of no isolation between tasks.

The descriptor tables consist of a sequence of 8-byte entries called descriptors. A descriptor table may contain from 1 to 8192 entries.



