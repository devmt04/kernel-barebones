/*
Now as we know that CPU will follow every instruction it got first, so how to make CPU know from where to start in our Kernel code? As diffrent compiler will arrange code diffrently, so it a necessary step toward making our code more reboust. So we will just write some code in assembly which will call an external funtion, that is our main funtion(entry funtion) os our kernel code.

Now in order to link them togeter we will use a Linker, ld, and link the object file of both, kernel.o and kernel_entry.o to create a single binary file kernel.bin

    $ ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
*/


.section .multiboot
.align 4
.long 0x1BADB002            # Magic number
.long 0x00                  # Flags
.long -(0x1BADB002 + 0x00)  # Checksum

.section .text

.code32
.extern main

call main   #invoke main() from here. Here we enter into kernel code
jmp .       #incase if we return from kernel, we will hang system
