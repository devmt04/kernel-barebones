#include "serial_output.h"

void serial_init(void) {
    outb(0x00, SERIAL_PORT_COM1 + 1);    // Disable all interrupts
    outb(0x80, SERIAL_PORT_COM1 + 3);    // Enable DLAB (set baud rate divisor)
    outb(0x03, SERIAL_PORT_COM1 + 0);    // Set divisor to 3 (lo byte) 38400 baud
    outb(0x00, SERIAL_PORT_COM1 + 1);    //                  (hi byte)
    outb(0x03, SERIAL_PORT_COM1 + 3);    // 8 bits, no parity, one stop bit
    outb(0xC7, SERIAL_PORT_COM1 + 2);    // Enable FIFO, clear them, with 14-byte threshold
    outb(0x0B, SERIAL_PORT_COM1 + 4);    // IRQs enabled, RTS/DSR set
}

int serial_received(void) {
    return inb(SERIAL_PORT_COM1 + 5) & 1;
}

char serial_read(void) {
    while (serial_received() == 0);
    return inb(SERIAL_PORT_COM1);
}

int is_transmit_empty(void) {
    return inb(SERIAL_PORT_COM1 + 5) & 0x20;
}

void serial_write(char c) {
    while (is_transmit_empty() == 0);
    outb(c, SERIAL_PORT_COM1);
}

void serial_write_string(const char *str) {
    while (*str) {
        serial_write(*str++);
    }
}

void serial_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[256];
    char *buf_ptr = buffer;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int num = va_arg(args, int);
                    if (num < 0) {
                        *buf_ptr++ = '-';
                        num = -num;
                    }
                    char num_buf[10];
                    int i = 0;
                    do {
                        num_buf[i++] = '0' + (num % 10);
                        num /= 10;
                    } while (num);
                    while (i > 0) {
                        *buf_ptr++ = num_buf[--i];
                    }
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    char num_buf[8];
                    int i = 0;
                    do {
                        int digit = num % 16;
                        num_buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + (digit - 10));
                        num /= 16;
                    } while (num);
                    while (i > 0) {
                        *buf_ptr++ = num_buf[--i];
                    }
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char *);
                    while (*str) {
                        *buf_ptr++ = *str++;
                    }
                    break;
                }
                case '%': {
                    *buf_ptr++ = '%';
                    break;
                }
                default:
                    *buf_ptr++ = '%';
                    *buf_ptr++ = *format;
                    break;
            }
        } else {
            *buf_ptr++ = *format;
        }
        format++;
    }

    *buf_ptr = '\0';
    serial_write_string(buffer);

    va_end(args);
}