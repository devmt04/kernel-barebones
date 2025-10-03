#include "VGA/vga.h"

void vector32_handler_p(void){
   vga_print_msg("INT 32 OCCURED", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_DARK_GREY), 10, 5);
}
