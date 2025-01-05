SRC_DIR := kernel
OBJ_DIR := build/obj
BIN_DIR := build/bin

C_SRC := $(shell find $(SRC_DIR) -name '*.c')
C_DIR := $(shell find -name '*.c' -exec dirname {} \; | sed 's|^\./||')
O_SRC := $(foreach src, $(C_SRC), $(OBJ_DIR)/$(src:%.c=%.o))
O_DIR := $(foreach dir, $(C_DIR), $(OBJ_DIR)/$(dir)) 

OUTPUT := release/iso/kernel.iso

CC := gcc
LD := ld
AS := as

CCFLAGS := -Wall -Wextra -Werror -m32 -ffreestanding -nostdlib -std=c11 -Wshadow -Wstrict-prototypes -O2  -fno-strict-aliasing -fno-builtin -fno-stack-protector -I$(SRC_DIR)
LDFLAGS := -nostdlib --warn-common -nmagic -gc-sections -s

all : prep $(OUTPUT) 
prep : 
	mkdir -p release/iso
	mkdir -p build/iso/boot/grub
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(O_DIR)

$(OUTPUT) : $(BIN_DIR)/kernel.bin
	cp $(BIN_DIR)/kernel.bin build/iso/boot/ 
	cp grub.cfg build/iso/boot/grub
	grub2-mkrescue -o $@ build/iso

$(BIN_DIR)/kernel.bin : $(OBJ_DIR)/kernel_entry.o  $(O_SRC)
	ld $(LDFLAGS) -m elf_i386 -T linker.ld -o $@ $^

$(OBJ_DIR)/kernel_entry.o : kernel/kernel_entry.s
	as  --32 -o $@ $^ #-g

$(OBJ_DIR)/%.o : %.c
	gcc $(CCFLAGS) -c $^ -o $@ #-g


run : all
	qemu-system-i386 -cdrom $(OUTPUT) -m 512 -serial stdio
# file:log.txt 
debug :
	qemu-system-i386 -cdrom $(OUTPUT) -m 512 -s -S 

dbserver :
	gdb $(BIN_DIR)/kernel.bin -ex "target remote localhost:1234"

clean : 
	rm -r build release

.PHONY : run clean test prep

