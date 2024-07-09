#include <stdint.h>

void write_char(char c, uint8_t color, volatile uint16_t *video_memory){
	uint16_t value = (color << 8) | c;
    *video_memory = value;
}

void print_msg(char *msg) {
    volatile uint16_t *video_memory = (volatile uint16_t *)0xb8000;
    while(*msg) {
        write_char(*msg, 0x0F, video_memory++);
        msg++;
    }
}

void clear_screen(){
	volatile uint16_t *video_memory = (volatile uint16_t *)0XB8000;
	int buffer_size = 80 * 25 * 2; // 80 columns 25 rows and 2 byte each in VGA text mode
	for(int i = 0; i < buffer_size; ++i) {
        *(uint8_t *)(video_memory + i) = 0;
    }
}

void main(){
	 clear_screen();
	 print_msg("Welcome to our OS");
}