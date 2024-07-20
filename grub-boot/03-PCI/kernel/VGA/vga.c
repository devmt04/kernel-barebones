#include "vga.h"

// Function to create VGA color attribute
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

// Function to write a sigle char at certain position
void vga_write_char(const char c, uint8_t color, uint8_t x, uint8_t y){
    uint16_t *vga_buffer = (uint16_t*) VGA_ADDRESS;
    vga_buffer[y * VGA_WIDTH + x] = c | (color << 8);
}

// Function to write a whole string at certain position
void vga_print_msg(const char *msg, uint8_t color, uint8_t x, uint8_t y){
    for (size_t i = 0; msg[i] != '\0'; i++) {
        vga_write_char(msg[i], color, x, y);
        x++;
        if (x >= VGA_WIDTH) {
            x = 0;
            y++;
            if (y >= VGA_HEIGHT) {
                y = 0; // Or handle scrolling or overflow as needed
            }
        }
    }
}

// Function to clear whole VGA Framebuffer
void vga_clear_screen(void){
    uint8_t color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK);
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            vga_write_char(' ', color, x, y);
        }
    }
}
