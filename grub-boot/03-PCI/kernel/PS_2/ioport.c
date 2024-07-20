#include "ioport.h"

void outportb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ (
        "outb %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

// Define inportb function to read a byte from an I/O port
uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ (
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}