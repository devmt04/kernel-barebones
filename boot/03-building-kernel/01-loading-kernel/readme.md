# IMPORTANT ANNOUNCEMENT !

###  Due to some unknown bugs in our bootloader, we are switching to GRUB, to boot our kernel, to keep continuing this journey. A new directory with name "grub-boot" is created where the further code and documentation will be uploaded! 


# Writing Kernel in C

So far, we have learnt a lot about how the computer really works, by communicating with it in the low-level assembly language, but we've also seen how it can be very slow to progress in such language. Another drawback of us continuing in assembly language is that it is closely tied to the specific CPU architecture, and so it would be harder for us to port our operating system to another CPU architecture (e.g, ARM, RISC, PowerPC).

From Now we will continuing writing our code in another languages, like c. Now we have to call our kernel code written in c, from our bootsector code. For that, we will load our kernel code in some specified memory address, and instruct our bootsector to start executing the instructions from that specified memory address.

For this we will compile our code with:

	$ gcc -ffreestanding -c kernel.c -o kernel.o
	$ ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary

-ffreestanding in gcc command will compile the c code as it is, means it won't search for any main function to start compilation from.

-Ttext 0x1000, will load kernel with an offset 0x1000, just like we load out bootsector with on offset of 0x7C00. We can see the binary/hex of our compiled c code with:

	$ objdump -d basic.o


# Steps

The steps involves are:

-> Write and compile the kernel code
-> Write and assemble the bootsector code
-> Create a kernel image that includes not only our bootsector but our compiled kernel code too
-> Load our kernel code into memory
-> Switch to 32-bit protected code
-> Begin executing our kernel code

Now to simplify the problem of which disk and from which sectors have to load the kernel code, the bootsector and kernel of an operating system can be grafted together into a kernel image, which can be written ti the initial sectors of boot disk, such that the bootsector code is always at the head of the kernel image.

Once we have compiled our kernel & bootsector code, we can create our kernel image with the following file concatenation command:

	cat boot_sector.bin kernel.bin > os-image