# Working of Hard-Disk drivers

Mechanically, hard disk contain one or more stacked platters that spin under a read/write head, much like a old record player. The metallic coating of the platters give them the property that specific area of their surface can be magnetised or demagnetised by head, effectively allowing any state to be recorded permanently on them.

It is therefore important to be able to describe the exact place on the disk's surface where some state is to be read or written, and so <b>Cylinder-Head-Sector(CHS) addressing</b> is used, which effectively is a 3D cordinate system.

Sector: Usually of capacity 512 bytes.

For more info, you can read about Cylinder, Head, Sector strcture of a hard disk.


# Using BIOS to Read the Hard Disk

For Hard disk we will use BIOS Interrupt 0x13. To read Hard Disk we will 0x02 function of BIOS's 0x13 Interrupt. Similary to write to Hard Disk we will use 0x03 function if BiOS Interrupt 0x13.

### Interrupt 0x13, function 0x02 : Read Hard Disk
Reads one or more disk sectors into a buffer.

Input:	AH = 02H
		AL = Number of Sectors
		BX = Offset address of buffer
		CH = Track NUmber
		CL = Sector Number
		DH = Disk side number(0 or 1)
		DL = Disk drive number	
		ES = Buffer segment address

Output:	Carry Flag = 0 : Operation Completed (AH = 0)
		Carry Flag = 1 : Error (AH = Error Code)

The BIOS update some registers to let us know what happended: The CARRY FLAG(CF) of special flag register is set to signal a GENERAL FAULT and %al is set to the NUMBER OF SECTORS ACTUALLY READ, AS OPPOSED TO THE NUMBER REQUESTED.


THE FIRST HARD DISK IS ASSIGNED TO NUMBER 80H, THE SECOND IS ASSIGNED NUMBER 81H
dl:Drive Number

  0 : Floppy 1
  2 : Floppy 2

  0x80 : HDD 1
  0x81 : HDD 2



## CL : Selected Sector Number

CL = 1 corresponds to the first sector.
CL = 2 corresponds to the second sector.
CL = 3 corresponds to the third sector.

First sector is always our bootsector code, as the bootsector usually at the start of the storage device. However BIOS will load our bootsector code to Main Memory at specific memory address that is 0x7c00(for x86-based systems) and starts executing the code at that address.


## AL : Number of Sectors to be read

The base value for AL is 1, which means AL = 1 corresponds to reading one sector, AL = 2 means reading two sectors, and so on.

## Cylinder (CH):

The base value for the cylinder number is typically 0, indicating the first cylinder. Cylinder numbers are typically zero-based.

## Head (DH):

The base value for the head number is also 0, indicating the first head. Head numbers are usually zero-based as well.

## Buffer Segment(ES) and Buffer Offset(BX)

We also need to set the buffer segment address (ES) and buffer offset address (BX) to specify the location in memory where the data from the disk will be read into. These values depend on your specific program and the memory location you want to use for the data buffer.

### Buffer Segment Address (ES):
ES is used to specify the segment address where the data buffer is located.
You can set ES to any valid segment address within your program's memory space. The value should be loaded into ES before calling the BIOS interrupt 0x13 function.
A common choice is to set ES to a data segment or an area in memory reserved for data storage. For example, ES might be set to 0x1000 to indicate the segment address 0x1000.

Buffer Offset Address (BX):
BX is used to specify the offset address within the segment pointed to by ES where the data buffer starts.
You can set BX to any valid offset address within the segment specified by ES.
The specific value you use for BX depends on your program's memory layout and where you want to store the data buffer.
A common value is to set BX to 0x0000 to indicate the start of the segment pointed to by ES.


1. Buffer segment address:

    This refers to the starting address of the memory block where the data from the hard disk will be written.
    It typically uses the segment register ES to point to the specific segment in memory.


2. Buffer offset address:

    This refers to the specific location within the memory block where the data should be written.
    It represents the distance in bytes from the beginning of the buffer segment.