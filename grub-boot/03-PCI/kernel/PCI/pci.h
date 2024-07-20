#ifndef PCI_H
#define PCI_H

#include <stdint.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

static inline void outl(uint32_t value, uint16_t port){
	__asm__ __volatile__(
		"outl %0, %1" 
		:
		:"a"(value), "Nd"(port)
		);
}

static inline uint32_t inl(uint16_t port){
	uint32_t ret;
	__asm__ __volatile__(
		"inl %1, %0"
		:"=a"(ret)
		:"Nd"(port)
		);
	return ret;
}

uint32_t pci_read_config(uint32_t bus, uint32_t device, uint32_t function, uint32_t register_num);

uint16_t pci_get_vendor_id(uint32_t bus, uint32_t device, uint32_t function);
uint16_t pci_get_device_id(uint32_t bus, uint32_t device, uint32_t function);
uint16_t pci_get_command(uint32_t bus, uint32_t device, uint32_t function);
uint16_t pci_get_status(uint32_t bus, uint32_t device, uint32_t function);
uint8_t pci_get_revision_id(uint32_t bus, uint32_t device, uint32_t function);
uint8_t pci_get_class_interface(uint32_t bus, uint32_t device, uint32_t function);
uint8_t pci_get_subclass_code(uint32_t bus, uint32_t device, uint32_t function);
uint8_t pci_get_baseclass_code(uint32_t bus, uint32_t device, uint32_t function);

#endif