#ifndef PIT_H
#define PIT_H

#define PIT_CHANNEL_0 0x40
#define PIT_CHANNEL_1 0x41
#define PIT_CHANNEL_2 0x42
#define PIT_CONTROL_WORD 0x43
#define PIT_FREQ 1193182

#include "IOPorts/ioport.h"
#include "serial_output.h"
#include <stdint.h>

uint8_t pit_read_channel2(void);
uint8_t pit_read_channel1(void);
uint8_t pit_read_channel0(void);
void pit_write_channel2(uint8_t data);
void pit_write_channel1(uint8_t data);
void pit_write_channel0(uint8_t data);
void pit_write_control_word(uint8_t data);
void pit_start_3_sec_timer(void);
#endif
