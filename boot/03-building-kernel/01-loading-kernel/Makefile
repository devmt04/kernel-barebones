ASM = as
LD = ld
CC = gcc

bootsector.bin : bootsector.o
	$(LD) -g -m elf_i386 -Ttext 0x7c00 --oformat binary -o $@ $^

bootsector.o : bootsector.s
	$(ASM) -g --32 $^ -o $@

GDT/gdt.o : GDT/gdt.s 
	$(ASM) -g --32 $^ -o $@

disk/disk.o : disk/disk.s 
	$(ASM) -g --32 $^ -o $@

Kernel/kernel_entry.o : Kernel/kernel_entry.s
	$(ASM) --32 -g $^ -o $@

Kernel/kernel.o : Kernel/kernel.c
	$(CC) -m32 -g -ffreestanding -c $^ -o $@ -lc -fno-pie

Kernel/kernel.bin :  Kernel/kernel_entry.o Kernel/kernel.o
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 --oformat binary $^

os-image : bootsector.bin Kernel/kernel.bin
	cat $^ > os-image

run : os-image
	qemu-system-x86_64 $<

Kernel/kernel.elf: Kernel/kernel_entry.o Kernel/kernel.o
	$(LD) -g -m elf_i386 -o $@ -Ttext 0x1000 $^ 

bootsector.elf: bootsector.o GDT/gdt.o disk/disk.o
	$(LD) -g -m elf_i386 -Ttext 0x7c00 -o $@ $^

debug: os-image bootsector.elf
	qemu-system-x86_64 -s -S -fda os-image 

dbserver :
	gdb -ex "target remote localhost:1234" -ex "symbol-file bootsector.elf"

clean:
	rm -f bootsector.bin bootsector.elf bootsector.o GDT/gdt.o disk/disk.o Kernel/kernel.bin Kernel/kernel.elf os-image Kernel/kernel.o Kernel/kernel_entry.o