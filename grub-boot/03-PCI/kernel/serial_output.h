#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdarg.h>

#define SERIAL_PORT_COM1 0x3F8

static inline void outb(uint8_t value, uint16_t port) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_write_string(const char *str);
void serial_write(char c);
int is_transmit_empty(void);
char serial_read(void);
int serial_received(void);
void serial_init(void);
void serial_printf(const char *format, ...);

#endif