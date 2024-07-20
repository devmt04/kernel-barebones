#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include <stdint.h>
#include "PS_2/ioport.h"

void ps2_init_keyboard_controller(void);
uint8_t ps2_read_keyboard_scan_code(void);

#endif