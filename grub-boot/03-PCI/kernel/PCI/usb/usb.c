#include "PCI/pci.h"
#include "serial_output.h"

uint32_t get_usb_base(void){
	uint32_t USBBASE = 0;
	for(uint32_t bus = 0; bus<255; bus++){
		for(uint32_t device=0;device<32;device++){	
			for(uint32_t function=0; function<8; function++){
				if(pci_get_baseclass_code(bus, device, function) == 0x0C){	
					serial_printf("found base class\n");
					if(pci_get_subclass_code(bus, device, function) == 0x03){
						serial_printf("found sub class\n");
						if(pci_get_class_interface(bus, device, function) == 0x00){
							serial_printf("found interface\n");
							USBBASE = pci_read_config(bus, device, function, 0x20);
						}
					}
				}
			}
		}
	}
	return USBBASE;
}
