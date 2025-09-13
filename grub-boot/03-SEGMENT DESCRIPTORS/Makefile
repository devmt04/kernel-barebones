SRC_DIR := kernel
OBJ_DIR := build/obj
BIN_DIR := build/bin

C_SRC := $(shell find $(SRC_DIR) -name '*.c')
C_DIR := $(shell find -name '*.c' -exec dirname {} \; | sed 's|^\./||')
O_SRC := $(foreach src, $(C_SRC), $(OBJ_DIR)/$(subst /,_,$(src:$(SRC_DIR)/%.c=%.o)))
# O_SRC := $(foreach dir, $(C_DIR), $(foreach src, $(C_SRC), $(patsubst $(dir)/%, $(OBJ_DIR)/%, $(OBJ_DIR)/$(subst /,_,$(src:$(SRC_DIR)/%.c=%.o))))) 
# C_DIR := $(shell find -name '*.c' -exec dirname {} \; | sort -u | sed 's|^\./||')
O_DIR := $(foreach dir, $(C_DIR), $(OBJ_DIR)/$(dir)) 


OUTPUT := release/iso/kernel.iso

CC := gcc
LD := ld
AS := as

all : prep $(OUTPUT) 
prep : 
	mkdir -p release/iso
	mkdir -p build/iso/boot/grub
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

$(OUTPUT) : $(BIN_DIR)/kernel.bin
	cp $(BIN_DIR)/kernel.bin build/iso/boot/ 
	cp grub.cfg build/iso/boot/grub
	grub-mkrescue -o $@ build/iso

$(BIN_DIR)/kernel.bin : $(OBJ_DIR)/kernel_entry.o  $(O_SRC) 
	ld -m elf_i386 -T linker.ld -o $@ $^

$(OBJ_DIR)/kernel_entry.o : kernel/kernel_entry.s
	as --32 -o $@ $^

define generate_rule
$(1) : 
	gcc -c $(patsubst $(OBJ_DIR)/%, $(SRC_DIR)/%, $(subst _,/,$(1:.o=.c))) -o $(1) -m32 -ffreestanding -nostdlib
endef

$(foreach src, $(O_SRC), $(eval $(call generate_rule, $(src))))

run : all
	qemu-system-i386 -cdrom $(OUTPUT) -m 512


debug :
	qemu-system-i386 -cdrom $(OUTPUT) -m 512 -s -S 

dbserver :
	gdb $(BIN_DIR)/kernel.bin -ex "target remote localhost:1234"

clean : 
	rm -r build release

.PHONY : run clean test prep