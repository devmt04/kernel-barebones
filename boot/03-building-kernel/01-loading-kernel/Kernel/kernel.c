#include <stdint.h>
// #include <stdlib.h>

void write_char(char c, uint8_t color, volatile uint16_t *video_memory){
	uint16_t value = (color << 8) | c;
    *video_memory = value;
}

void print_msg(char *msg) {
    volatile uint16_t *video_memory = (volatile uint16_t *)0xb8000;
    while(*msg++) {
        write_char(*msg, 0x0F, video_memory); // 0x0F is typically white on black
        video_memory++;
    }
}

void clear_screen(){
	/*
	it's advisable to use a volatile qualifier when working with memory-mapped I/O, which ensures that the compiler doesn't optimize away reads or writes to this memory location
	*/
	volatile uint16_t *video_memory = (volatile uint16_t *)0XB8000;
	int buffer_size = 80 * 25 * 2; // 80 columns 25 rows and 2 byte each in VGA text mode
	for(int i = 0; i < buffer_size; ++i) {
        *(uint8_t *)(video_memory + i) = 0;
    }
}

void main(){
	/*
while the base address for VGA text mode memory is usually 0xB8000, each
character cell occupies two consecutive bytes. Therefore, the attributes for
a character in a specific cell are stored in the next consecutive address,
which would be 0xB8001.

Given this layout, when you access VGA_MEMORY at 0xB8000, you're accessing the
memory for the first character cell, and the attributes for that cell would
indeed be stored at 0xB8001. Therefore, using uint16_t instead uint8_t to access VGA text
mode memory allows you to work with each character cell efficiently, as each
element in the array represents one character cell, comprising both the
character code and its attributes.
	*/

	 clear_screen();
	 //write_char('y', 0XAF, (volatile uint16_t *)ADDR);
	 print_msg("Hello");

	// while(*str++){
	// }
}