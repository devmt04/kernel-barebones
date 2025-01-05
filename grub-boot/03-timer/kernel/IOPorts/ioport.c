#include "ioport.h"

void outportb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ (
        "outb %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ (
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}

void outportw(uint16_t port, uint16_t data) {
    __asm__ __volatile__ (
        "outw %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}


uint16_t inportw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__ (
        "inw %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}


void outportl(uint16_t port, uint32_t data) {
    __asm__ __volatile__ (
        "outl %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}


uint32_t inportl(uint16_t port) {
    uint32_t ret;
    __asm__ __volatile__ (
        "inl %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}


void io_wait(void) {
    // Perform a dummy read from an unused I/O port
    __asm__ __volatile__ ("inb %0, %%al" : : "dN" (0x80));
}
