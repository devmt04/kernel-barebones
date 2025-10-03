.section .text
   .extern vector32_handler_p

   .global dummy_handler
   .global vector32_handler

   dummy_handler:
      iret

   vector32_handler:
      pusha
      call vector32_handler_p
      popa
      iret

   # some exceptions pushes error code too, for those we have to handle it
