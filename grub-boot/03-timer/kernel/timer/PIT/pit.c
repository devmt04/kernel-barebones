#include "pit.h"

// Write Operations
void pit_write_control_word(uint8_t data){
    outportb(PIT_CONTROL_WORD, data);
}

void pit_write_channel0(uint8_t data){
    outportb(PIT_CHANNEL_0, data);
}

void pit_write_channel1(uint8_t data){
    outportb(PIT_CHANNEL_1, data); 
}

void pit_write_channel2(uint8_t data){
    outportb(PIT_CHANNEL_2, data); 
}


// Read Operations
uint8_t pit_read_channel0(void){
    return inportb(PIT_CHANNEL_0);
}

uint8_t pit_read_channel1(void){
    return inportb(PIT_CHANNEL_1);
}

uint8_t pit_read_channel2(void){
    return inportb(PIT_CHANNEL_2);
}


/*
//Demo code
void pit_start_3_sec_timer(void){
    uint16_t hz = 20;
    uint16_t divisor = PIT_FREQ / hz;

    serial_printf("Starting 3 seconds of timer\n");

    pit_write_control_word(0b00110110); // 0b00110100 : Channel 0, lobyte/hibyte access mode, mode 3, 16-bit binary counting
    io_wait();
    pit_write_channel0((uint8_t)(divisor & 0xFF)); // write lobyte
    io_wait();
    pit_write_channel0((uint8_t)((divisor >> 8) & 0xFF)); // write hibyte
    io_wait();

    
    for (int i = 0; i < 60; i++) { // 60 * 0.05 seconds = 3 seconds
        pit_write_control_word(0b11100010);
        io_wait();
        uint8_t status = pit_read_channel0(); // Read status byte
        while ((status & 0x80)) { // Check OUT pin status
            status = pit_read_channel0(); // Read again
            serial_printf("!\n");
        }
        serial_printf(".\n");
        io_wait();

    }

    // pit_write_control_word(0b11100010); // issue read-back command : OUT status only, channel 0

    // while((pit_read_channel0() & 0b10000000) == 0){
    //      serial_printf(".\n");
    // };

    serial_printf("Timer Stopped after 3 seconds!");
}

*/
