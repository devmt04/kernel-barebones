#ifndef IOPORT_H
#define IOPORT_H

#include <stdint.h>

void outportb(uint16_t port, uint8_t data);
uint8_t inportb(uint16_t port);

#endif