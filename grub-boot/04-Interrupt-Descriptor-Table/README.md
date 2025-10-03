# Interrupt Descriptor Table

## Exceptions and interrupts

**Exceptions** and **interrupts** are forced transfers of execution to a task or a procedure. The task or procedure is called a handler. Interrupts occur at random times during the execution of a program, in response to signals from hardware. Exceptions occur when instructions are executed which provoke exceptions. Interrupts are used to handle events external to the processor, such as requests to service peripheral devices. Exceptions handle conditions detected by the processor in the course of executing instructions, such as division by O.

There are two sources for interrupts and three sources for exceptions:

1. Interrupts
   
   - **External or Maskable Interrupts**, External interrupts are received through pins on the processor or through the local APIC, like those collected from devices (keyboard, timer, NIC, etc). Maskable hardware interrupts that can be delivered through the INTR pin include all IA-32 architecture defined interrupt vectors from 0 through 255; those that can be delivered through the local APIC include interrupt vectors 16 through 255. Maskable interrupts do not occur unless the interrupt-enable flag (IF) is set.
   When interrupts 0 through 15 are delivered through the local APIC, the APIC indicates the receipt of an illegal vector.

   - **Software-Generated or Nonmaskable Interrupts**, The INT n instruction permits interrupts to be generated from within software by supplying an interrupt vector number as an operand. For example, the INT 35 instruction forces an implicit call to the interrupt handler for interrupt 35. These are received on the NMI (Non-Maskable Interrupt) input of the processor. The processor does not provide a mechanism to prevent nonmaskable interrupts, i.e, Interrupts generated in software with the INT n instruction cannot be masked by the IF flag in the EFLAGS register.

2. Exceptions

   - **Processor-detected program-error exceptions**, The processor generates one or more exceptions when it detects program errors during the execution in an application program or the operating system or executive. Intel 64 and IA-32 architectures define a vector number for each processor-detectable exception. Exceptions are classified as faults, traps, and aborts

   - **Programmed or Software-Generated Exceptions**. The INTO, INT1, INT3, and BOUND instructions permit exceptions to be generated in software. These instructions allow checks for exception conditions to be performed at points in the instruction stream. For example, INT3 causes a breakpoint exception to be generated.

   - **Machine-check exceptions**, The P6 family and Pentium processors provide both internal and external machine-check mechanisms for checking the operation of the internal chip hardware and bus transactions. These mechanisms are implementation dependent. When a machine-check error is detected, the processor signals a machine-check exception and returns an error code.

## EXCEPTION CLASSIFICATIONS

- **Faults** :  A fault is an exception that can generally be corrected and that, once corrected, allows the program to be restarted with no loss of continuity. When a fault is reported, the processor restores the machine state to the state prior to the beginning of execution of the faulting instruction. The return address (saved contents of the CS and EIP registers) for the fault handler points to the faulting instruction, rather than to the instruction following the faulting instruction.

A fault is an unintentional exception detected before the instruction finishes executing. e.g, Page Fault (when required memory page isn’t in RAM), Invalid opcode (instruction doesn’t exist).

- **Traps** : A trap is an exception that is reported immediately following the execution of the trapping instruction. Traps allow execution of a program or task to be continued without loss of program continuity. The return address for the trap handler points to the instruction to be executed after the trapping instruction.

A trap is an exception that is intentional and occurs after the instruction completes.

- **Aborts** :  An abort is an exception that does not always report the precise location of the instruction causing the exception and does not allow a restart of the program or task that caused the exception. Aborts are used to report severe errors, such as hardware errors and inconsistent or illegal values in system tables.

An abort is a serious error that prevents the instruction (and sometimes the process) from continuing.


## IDT

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
      bit 16 to bit 31 : TSS SEGMENT SELECTOR
      bit 32 to bit 39 : RESERVED
      bit 40 to bit 44 : 10100
      bit 45 to bit 46 : DPL (A 2-bit value which defines the CPU Privilege Levels which are allowed to access this interrupt via the INT instruction. Hardware interrupts ignore this mechanism.)
      bit 47 : P (Present bit. Must be set (1) for the descriptor to be valid)
      bit 48 to bit 63 : RESERVED 

- INTERRUPT GATE:

      bit 0 to bit 15 : OFFSET (lower 16 bits of handler address)
      bit 16 to bit 31 : SEGMENT SELECTOR
      bit 32 to bit 36 : RESERVED
      bit 37 to bit 39 : 000
      bit 40 to bit 44 : 01110
      bit 45 to bit 46 : DPL
      bit 47 : P
      bit 48 to bit 63 : OFFSET (higher 16 bits of handler address for 32-bit entry)


- TRAP GATE:

      bit 0 to bit 15 : OFFSET (lower 16 bits of handler address)
      bit 16 to bit 31 : SEGMENT SELECTOR
      bit 32 to bit 36 : RESERVED
      bit 37 to bit 39 : 000
      bit 40 to bit 44 : 11110
      bit 45 to bit 46 : DPL
      bit 47 : P
      bit 48 to bit 63 : OFFSET (higher 16 bits of handler address for 32-bit entry)

#### Descriptor Privilege Level (DPL):

DPL is a 2-bit field in an x86/x86-64 IDT (Interrupt Descriptor Table) entry that specifies the minimum privilege level required to invoke that interrupt or trap via the INT n instruction from software.

Privilege levels (rings) in x86:

   Ring 0 (DPL = 0): Kernel / highest privilege
   Ring 1 (DPL = 1): Rarely used
   Ring 2 (DPL = 2): Rarely used
   Ring 3 (DPL = 3): User / lowest privilege

The CPU checks CPL (Current Privilege Level) vs. DPL whenever a software interrupt (INT n) is executed:

   CPL ≤ DPL: Allowed
   CPL > DPL: General Protection Fault (#GP) occurs

## EXCEPTION AND INTERRUPT VECTORS

To aid in handling exceptions and interrupts, each architecturally defined exception and each interrupt condition requiring special handling by the processor is assigned a unique identification number, called a **vector number**. The processor uses the vector number assigned to an exception or interrupt as an index into the interrupt descriptor table (IDT).

Vector numbers in the range 0 through 31 are reserved by the Intel 64 and IA-32 architectures for architecture-defined exceptions and interrupts. Not all of the vector numbers in this range have a currently defined function. The unassigned vector numbers in this range are reserved. Do not use the reserved vector numbers. (see table at https://wiki.jwo.cz/wiki/x86_protected_mode_interrupts)

Vector numbers in the range 32 to 255 are designated as user-defined interrupts and are not reserved by the Intel 64 and IA-32 architecture.


# References

- https://pdos.csail.mit.edu/6.828/2018/readings/i386/s09_04.htm
- https://wiki.osdev.org/Interrupt_Descriptor_Table
- https://pdos.csail.mit.edu/6.828/2018/readings/i386.pdf : INTEL 80386 PROGRAMMER'S REFERENCE MANUAL 1986
- https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html : Intel® 64 and IA-32 Architectures Software Developer Manuals



