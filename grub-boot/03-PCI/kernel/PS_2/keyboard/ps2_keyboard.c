#include "ps2_keyboard.h"

void ps2_init_keyboard_controller(void){
	outportb(0x64, 0xAD); // disable keyboard
	outportb(0x64, 0xAE); // enable keyboard
}

uint8_t ps2_read_keyboard_scan_code(void){
	while((inportb(0x64) & 0x01) == 0);
	// Polling status register bit 0 (first bit 0 = 1 when data is available)
	uint8_t scan_code = inportb(0x60);
	return scan_code;
}