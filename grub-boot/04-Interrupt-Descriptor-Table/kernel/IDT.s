.section .bss
   .align 8              
   IDT:
       .space 8*256        

.section .data
   IDTR:
       .word IDT_END - IDT_START -1    # limit (size of IDT - 1)
       .long IDT_START                 # base (address of IDT)

.section .text
   .extern dummy_handler
   .extern vector32_handler

   .macro IDT_TRAP_ENTRY vector_num, offset
      _idt_entry_\vector_num:
         movl $\offset, %eax
         movw %ax, IDT+(8*\vector_num)+0
         movw $0x08, IDT+(8*\vector_num)+2
         movb $0, IDT+(8*\vector_num)+4
         movb $0, IDT+(8*\vector_num)+5
         movb $0b10001111, IDT+(8*\vector_num)+6
         shr $16, %eax
         movw %ax, IDT+(8*\vector_num)+7

           # bit 0 to bit 15 : OFFSET (lower 16 bits of handler address)
           # bit 16 to bit 31 : SEGMENT SELECTOR
           # bit 32 to bit 36 : RESERVED
           # bit 37 to bit 39 : 000
           # bit 40 to bit 44 : 11110
           # bit 45 to bit 46 : DPL
           # bit 47 : P
           # bit 48 to bit 63 : OFFSET (higher 16 bits of handler address for 32-bit entry)
   .endm

   .macro IDT_INTERRUPT_ENTRY vector_num, offset
      _idt_entry_\vector_num:
         movl $\offset, %eax
         movw %ax, IDT+(8*\vector_num)+0
         movw $0x08, IDT+(8*\vector_num)+2
         movb $0, IDT+(8*\vector_num)+4
         movb $0, IDT+(8*\vector_num)+5
         movb $0b10001110, IDT+(8*\vector_num)+6
         shr $16, %eax
         movw %ax, IDT+(8*\vector_num)+7

           # bit 0 to bit 15 : OFFSET (lower 16 bits of handler address)
           # bit 16 to bit 31 : SEGMENT SELECTOR
           # bit 32 to bit 36 : RESERVED
           # bit 37 to bit 39 : 000
           # bit 40 to bit 44 : 01110
           # bit 45 to bit 46 : DPL
           # bit 47 : P
           # bit 48 to bit 63 : OFFSET (higher 16 bits of handler address for 32-bit entry)
   .endm

   IDT_START:
      # Vector 0 : Divide Error Exception
         IDT_TRAP_ENTRY 0, dummy_handler
      # Vector 32 : first User Defined Interrupts
         IDT_INTERRUPT_ENTRY 32, vector32_handler
   IDT_END:

.global load_idt
load_idt:
   lidt IDTR
   ret
