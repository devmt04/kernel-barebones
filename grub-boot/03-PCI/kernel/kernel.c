#include <stdint.h>
#include "serial_output.h"

#include "VGA/vga.h"
#include "PS_2/keyboard/ps2_keyboard.h"

void main(void){
	 vga_clear_screen();
	 ps2_init_keyboard_controller();
     vga_print_msg("Welcome to this simple Kernel", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 0, 0);
}