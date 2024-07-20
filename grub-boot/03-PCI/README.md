# The PCI BUS:

The PCI (Peripheral Component Interconnect) bus was defined to establish a high performance and low cost local bus that would remain through several generations of products.

PCI is a hardware bus used to connect peripheral devices such as network cards, sound cards, and graphics cards to the motherboard. It provides a standardized way for these devices to communicate with the CPU and memory, ensuring interoperability and ease of integration.

It is highly recommended to learn about the PCI protocol via the PCI Specification from <a href="https://pcisig.com/" target="_blank"><u>PCI-SIG</u></a>. The document is not publicly available; to get a copy, you typically need to be an employee of a member company listed by PCI-SIG. However, you can find the necessary implementation information online easily.

You can read necessary implementation information from <a href="https://lekensteyn.nl/files/docs/PCI_SPEV_V3_0.pdf" target="_blank"><u>https://lekensteyn.nl/files/docs/PCI_SPEV_V3_0.pdf</u></a>

Other useful resoures : <a hred="https://wiki.osdev.org/PCI"><u>https://wiki.osdev.org/PCI</u></a>

The area of our intrest, for now, in this document is to learn about <b>protocol</b> and <b>configurational specification</b>.

## Some Key Point about PCI. Go through them, they are extracted from above PCI 3.0 Specification document:

- PCI Components add-in card interface is processor independent.

- PCI defines three physical address space: The Memory Address Space, The I/O Address Space, and The Conigurational Address Space.

- Every PCI Target (except host bus bridge) are required to impliment `Base Address Register(s)` to request a range of addresses which can be used to provide access to internal registers or functions.

- A device(PCI Target) may map it's internal register into both Memory Space or I/O space.

- Every device(Other than host bus bridge) must impliment Configurational Address Space. In the Configurational Address Space, each function is assigned a unique 256-byte space.

- <b>PCI Bridges</b> : Because of the electrical loading issue, the number of device that can be supported on a given bus segment is limited. To allow systems to be built beyond a single bus segment, PCI-to-PCI bridges are defined.

- To Identify a particular devices, connected on PCI Bus, we have to access it's Configurational Space.

- Two DWORD I/O Locations are used to generate configurational transactions for PC-AT compatible system. The first DWORD location is `0xCF8`(a read/write register) that is named `CONFIG_ADDRESS`. The second DWORD address is `0xCFC`(a read/write register) that is named `CONFIG_DATA`.

- The `CONFIG_ADDRESS` register is 32 bits with format: (you can refer to Chapter 3, Figure 3-2 from the PCI 3.0 Specification Document)
	

	bit 0-2   : Must be 0
	bit 2-7   : Register Number
	bit 8-10  : Function Number
	bit 11-15 : Device Number 
	bit 16-23 : Bus Number
	bit 24-30 : (Reserved)
	bit 31 	  : Enable Bit (1 = 'enabled', 0 = 'disabled')


- To access a particulat PCI-Target device, we must provide the above entries in `CONFIG_ADDRESS` register. In return, the results will be stored under `CONFIG_DATA` register. (See about these entries in detail in PCI 3.0 Specification document)

- The Register Number, will be one of the Register Offset, in PCI Configurational Space (See Chapter 6, fig:6-1 from PCI 3.0 Specification Document)

- In Configurational Space(implimented by PCI targer devices), there are some entries to Identify a Device Uniquely(these entries are: VendorID, DeviceID, RevisionID). Entries Like Class Code is used to identify the type and work of Device. (You can find Class Codes at Appendix D, PCI 3.0 Specification Document)



## Compile the Project

```bash
make
```

This will create a .iso disk image in `release/iso` folder.

## Run the Project

```bash
make run
```

## Clean the Project Directory

```bash
make clean
```