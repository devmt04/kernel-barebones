#include "VGA/vga.h"

void main(){
	 vga_clear_screen();
	 vga_print_msg("Welcome to this simple Kernel", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 0,0);
}