# Interrupt Descriptor Table

## Overview

Interrupts are special cases of control transfer within a program. An interrupt occurs as a result of an event that is independent of the currently executing program.

The Interrupt Descriptor Table (IDT) is a binary data structure specific to the IA-32 and x86-64 architectures. It is the Protected Mode and Long Mode counterpart to the Real Mode **Interrupt Vector Table (IVT)** telling the CPU where the Interrupt Service Routines (ISR) are located (one per interrupt vector). It is similar to the GDT in structure.

The interrupt descriptor table (IDT) associates each interrupt or exception identifier with a descriptor for the instructions that service the associated event. Unlike the GDT and LDTs, the first entry of the IDT may contain a descriptor. Like the GDT and LDTs, the IDT is an array of 8-byte descriptors. To form an index into the IDT, the processor multiplies the interrupt or exception identifier by eight. Because there are only 256 identifiers, the IDT need not contain more than 256 descriptors. It can contain fewer than 256 entries; entries are required only for interrupt identifiers that are actually used.

The IDT may reside anywhere in physical memory. The processor locates the IDT by means of the IDT register (IDTR). The instructions LIDT and SIDT operate on the IDTR.

**LIDT (Load IDT register)** loads the IDT register with the linear base address and limit values contained in the memory operand. This instruction can be executed only when the CPL is zero. It is normally used by the initialization logic of an operating system when creating an IDT. An operating system may also use it to change from one IDT to another.

**SIDT (Store IDT register)** copies the base and limit value stored in IDTR to a memory location. This instruction can be executed at any privilege level. 

## INTERRUPT DESCRIPTOR TABLE

Many different events may cause an interrupt. To allow the reason for an interrupt to be easily identified, each interrupt source is given a number called the interrupt vector. Up to 256 different interrupt vectors (numbers) are possible. The IDT is in physical memory, pointed to by the contents of the on-chip IDT register that contains a 32-bit base and a 16-bit limit. The IDTR is normally loaded with the LIDT instruction by code that executes at privilege level 0 during system initialization. The IDT may be located anywhere in the physical address space. 

Structure of IDTR register:

   first 16 bits : IDT LIMIT
   rest 32 bits : IDT BASE

Each IDT entry is a 8-byte gate descriptor that contains a pointer to the handler. The IDT entries are called gates. 

The three types of gates permitted in the IDT are :

- **Interrupt Gates**
- **Task Gates** 
- **Trap Gates**

Interrupt and task gates process interrupts in the same task, while task gates cause a task switch. Any other descriptor type in the IDT will cause an exception if it is referenced by an interrupt.

The IDT need not contain all 256 entries. A 16-bit limit register allows less than the full number of entries. Unused entries may be signaled by placing a zero in the access rights byte. If an attempt is made to access an entry outside the table limit, or if the wrong descriptor type is found; a general protection fault occurs with an error code pushed on the stack identifying the invalid interrupt vector.

IDT Gate Descriptors structure:

- TASK GATE:

   bit 0 to bit 15 : RESERVED
   bit 15 to bit 31 : TSS SEGMENT SELECTOR
   bit 32 to bit 39 : RESERVED
   bit 40 to bit 44 : 10100
   bit 45 to bit 46 : DPL (A 2-bit value which defines the CPU Privilege Levels which are allowed to access this interrupt via the INT instruction. Hardware interrupts ignore this mechanism.)
   bit 47 : P (Present bit. Must be set (1) for the descriptor to be valid)
   bit 48 to bit 63 : RESERVED 

- INTERRUPT GATE:

   bit 0 to bit 15 : OFFSET
   bit 15 to bit 31 : SEGMENT SELECTOR
   bit 32 to bit 36 : RESERVED
   bit 37 to bit 39 : 000
   bit 40 to bit 44 : 01110
   bit 45 to bit 46 : DPL
   bit 47 : P
   bit 48 to bit 63 : OFFSET


- TRAP GATE:

   bit 0 to bit 15 : OFFSET
   bit 15 to bit 31 : SEGMENT SELECTOR
   bit 32 to bit 36 : RESERVED
   bit 37 to bit 39 : 000
   bit 40 to bit 44 : 11110
   bit 45 to bit 46 : DPL
   bit 47 : P
   bit 48 to bit 63 : OFFSET


- Interrupts 0 to 31 are pre-defined interrupts. (see table at https://wiki.jwo.cz/wiki/x86_protected_mode_interrupts)
- Interrupts 32 to 255 are User Defined Interrupts



# References

- https://pdos.csail.mit.edu/6.828/2018/readings/i386/s09_04.htm
- https://wiki.osdev.org/Interrupt_Descriptor_Table
- https://pdos.csail.mit.edu/6.828/2018/readings/i386.pdf : INTEL 80386 PROGRAMMER'S REFERENCE MANUAL 1986
- https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html : Intel® 64 and IA-32 Architectures Software Developer Manuals



