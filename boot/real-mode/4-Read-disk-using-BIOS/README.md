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