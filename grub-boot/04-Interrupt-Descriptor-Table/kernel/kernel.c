#include <stdint.h>
#include <stddef.h>

#include "VGA/vga.h"

static inline void outportb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ (
        "outb %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

// Define inportb function to read a byte from an I/O port
static inline uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ (
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}


void init_keyboard_controller(){
	outportb(0x64, 0xAD); // disable keyboard
	outportb(0x64, 0xAE); // enable keyboard
}

uint8_t read_keyboard_scan_code(){
	while((inportb(0x64) & 0x01) == 0);
	// Polling status register bit 0 (first bit 0 = 1 when data is available)
	uint8_t scan_code = inportb(0x60);
	return scan_code;
}

void main(){
	 vga_clear_screen();
	 init_keyboard_controller();
     vga_print_msg("Welcome to this simple Kernel", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 0, 0);

	 while(1){
	 	uint8_t scan_code = read_keyboard_scan_code();
       	if(scan_code == (uint8_t)0x1C) // Enter Pressed
       		vga_print_msg("Enter Key Pressed", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY), 5, 2);
	 	if(scan_code == (uint8_t)0x9C) // Enter Released
       		vga_print_msg("Enter Key Released", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_DARK_GREY), 5, 4);
	    if(scan_code == (uint8_t)0x17){
            __asm__ __volatile__("int $0x20"); // 0x20 = 32
        }
     }
}
