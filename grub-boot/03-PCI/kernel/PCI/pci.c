#include "pci.h"

uint32_t pci_read_config(uint32_t bus, uint32_t device, uint32_t function, uint32_t register_num){
	uint32_t address = ((uint32_t)1 << 31) |
						((bus & 0xFF) << 16) |
						((device & 0x1F) << 11) |
						((function & 0x07) << 8) |
						((register_num & 0xFC));
	outl(address, CONFIG_ADDRESS);
	return inl(CONFIG_DATA);
}

uint16_t pci_get_vendor_id(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x00);
	return (uint16_t)(data & 0x0000FFFF);
}

uint16_t pci_get_device_id(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x00);
	return (uint16_t)((data & 0xFFFF0000) >> 16);
}

uint16_t pci_get_command(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x04);
	return (uint8_t)(data & 0x0000FFFF);
}

uint16_t pci_get_status(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x04);
	return (uint8_t)(	(data & 0xFFFF0000) >> 16);
}

uint8_t pci_get_revision_id(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x08);
	return (uint8_t)(data & 0x000000FF);
}

uint8_t pci_get_class_interface(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x08);
	return (uint8_t)((data & 0x0000FF00) >> 8);
}

uint8_t pci_get_subclass_code(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x08);
	return (uint8_t)((data & 0x00FF0000) >> 16);
}

uint8_t pci_get_baseclass_code(uint32_t bus, uint32_t device, uint32_t function){
	uint32_t data = pci_read_config(bus, device, function, 0x08);
	return (uint8_t)((data & 0xFF000000) >> 24);
}
