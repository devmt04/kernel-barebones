#ifndef IOPORT_H
#define IOPORT_H

#include <stdint.h>

uint32_t inportl(uint16_t port);
void outportl(uint16_t port,uint32_t data);
uint16_t inportw(uint16_t port);
void outportw(uint16_t port,uint16_t data);
uint8_t inportb(uint16_t port);
void outportb(uint16_t port,uint8_t data);
void io_wait(void);

#endif
