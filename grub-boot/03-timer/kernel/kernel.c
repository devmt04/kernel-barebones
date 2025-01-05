#include <stdint.h>
#include "serial_output.h"

#include "VGA/vga.h"
#include "PS_2/keyboard/ps2_keyboard.h"
#include "timer/PIT/pit.h"

void main(void){
	 vga_clear_screen();
   vga_print_msg("Welcome to this simple Kernel", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 0, 0);
   pit_start_3_sec_timer();
}
