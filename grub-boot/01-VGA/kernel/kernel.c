#include <stdint.h>
#include <stddef.h>

#define VGA_ADDRESS 0XB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Function to create VGA color attribute
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Function to write a sigle char at certain position
void write_char(const char c, uint8_t color, uint8_t x, uint8_t y){
    uint16_t *vga_buffer = (uint16_t*) VGA_ADDRESS;
    vga_buffer[y * VGA_WIDTH + x] = c | (color << 8);
}

// Function to write a whole string at certain position
void print_msg(const char *msg, uint8_t color, uint8_t x, uint8_t y) {
    for (size_t i = 0; msg[i] != '\0'; i++) {
        write_char(msg[i], color, x, y);
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
void clear_screen() {
    uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            write_char(' ', color, x, y);
        }
    }
}

// From here the main execution of kernel code begins. This function main() is being
// called from kernel_entry.s file
void main(){
	 clear_screen();
	 print_msg("Welcome to this simple Kernel", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 0,0);
}