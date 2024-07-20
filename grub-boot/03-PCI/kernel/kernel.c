#include <stdint.h>
#include "serial_output.h"

#include "VGA/vga.h"
#include "PS_2/keyboard/ps2_keyboard.h"
#include "PCI/usb/usb.h"

static inline uint8_t innb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
}

void outbb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ (
        "outb %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

void main(void){
	 vga_clear_screen();
	 ps2_init_keyboard_controller();
     vga_print_msg("Welcome to this simple Kernel", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 0, 0);

     uint16_t USBBASE = (uint16_t)((get_usb_base() & 0b00000000000000001111111111100000) >> 5);
     serial_printf("--->%x\n", USBBASE);

     if(USBBASE == 0){
      	vga_print_msg("0", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 2, 3);
     }else{
     	vga_print_msg("found", vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), 2, 3);
     }
	 
	 // uint16_t data = ((innb((USBBASE + 0x06)) & 0x00ff) << 8) | (innb((USBBASE + 0x07)) & 0x00ff) ;
     // serial_printf("%d   %x\n", data, data);
     
     while(1){
     	//uint16_t data = ((innb((USBBASE + 0x06)) & 0x00ff) << 8) | (innb((USBBASE + 0x07)) & 0x00ff) ;
     	uint8_t	data = innb((USBBASE + 0x06));
     	if(data!=0){
     		serial_printf("%d   %x\n", data, data);
     	}
     	outbb((USBBASE+0x00), 0x0);
     }
}