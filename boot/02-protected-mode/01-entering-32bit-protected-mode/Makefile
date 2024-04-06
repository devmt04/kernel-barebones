ASM = as
LD = ld

bootsector.bin : bootsector.s
	$(ASM) -g $< -o bootsector.o
	$(LD) --oformat binary bootsector.o -o $@ -Ttext 0x7c00

run : bootsector.bin
	qemu-system-x86_64 $<

clean:
	rm -f bootsector.bin