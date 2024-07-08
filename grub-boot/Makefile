.DEFAULT_GOAL := outs/kernel.iso

outs/kernel.iso : kernel/outs/kernel.bin
	mkdir -p iso/boot/grub
	cp kernel/outs/kernel.bin iso/boot/ 
	cp grub.cfg iso/boot/grub
	grub-mkrescue -o $@ iso

kernel/outs/kernel.bin : kernel/outs/kernel.o kernel/outs/kernel_entry.o
	ld -m elf_i386 -T linker.ld -o $@ $^

kernel/outs/kernel.o kernel/outs/kernel_entry.o : kernel/kernel.c kernel/kernel_entry.s 
	as --32 -o kernel/outs/kernel_entry.o kernel/kernel_entry.s 
	gcc -m32 -ffreestanding -c kernel/kernel.c -o kernel/outs/kernel.o

run : outs/kernel.iso
	qemu-system-i386 -cdrom $^

clean : 
	rm -f kernel/outs/kernel.o kernel/outs/kernel_entry.o kernel/outs/kernel.bin outs/kernel.iso
	rm -r iso outs
	mkdir outs

.PHONY : run clean