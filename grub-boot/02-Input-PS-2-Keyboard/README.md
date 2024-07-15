# Taking Input from keyboard

There are two well-known method, to interact with hardware like Keyboard, they are:

- Direct I/O Port Access in x86 Architecture (this is what we are gonna discuss here)

- Direct Hardware Access (this include initilising the hardware registers and set up communication with the keyboard controller, and a lot more brain-stroming steps.)

## I/O Address Space

The x86 architecture has a separate I/O address space (also known as I/O ports) distinct from the main memory address space. This space is used for communication between the CPU and I/O devices.

Devices like the PS/2 keyboard controller are assigned specific port addresses. For instance, port `0x60` is used to exchange data (scan codes) between the PS/2 keyboard and the CPU, while port `0x64` is used for status and command communication with the PS/2 keyboard controller itself.

The PS/2 keyboard controller, which is connected to the CPU via these ports, sends data (such as scan codes when keys are pressed) to the CPU via port 0x60. The CPU reads this data from the port 0x60 and processes it accordingly.

The CPU interacts with I/O ports through special instructions (`in` and `out` instructions in assembly language) that allow it to read from or write to specific port addresses.

These instructions are part of the CPU’s instruction set and enable direct communication with devices connected to the system via I/O ports.

Check these they will surely help you to understand this in more detail:<br><br>

<a target="_blank" href="https://opensecuritytraining.info/IntroBIOS_files/Day1_04_Advanced%20x86%20-%20BIOS%20and%20SMM%20Internals%20-%20IO.pdf"><u>1 - (Do not forget to check which registers are being used to store port address and data while using `in` and `out` instructions. `ax` is used to store data, which we need to pass or fetch from port address stored in `dx` register or use drectly as immeduate value)</u></a><br><br>

<a target="_blank" href="https://pdos.csail.mit.edu/6.828/2008/readings/i386/c08.htmf"><u>2</u></a><br><br>

<a target="_blank" href="http://www-ug.eecg.utoronto.ca/desl/manuals/ps2.pdf
"><u>3 - PS/2 Protocol Documentation, you can use this to see what data you can get/give to PS/2 devices.</u></a><br><br>

<a target="_blank" href="https://wiki.osdev.org/%228042%22_PS/2_Controller#Interrupts"><u>4 - OSDevWiki - you can see here wht bits you can pass to PS/2 Controller(Not PS/2 device).</u></a><br><br>

<a target="_blank" href="https://valhalla.altium.com/Learning-Guides/PS2-PS2_Controller.pdf"><u>5 - Another documentaion, seems to contain more detail about PS/2 Protocol.</u></a><br><br>


<b>NOTE: All this is only for PS/2 controlled Keyboard (not one that are connected to USB, as they follow diffrent protocol for communication). However, any keyboard will work fine if run under Qemu(and probably others) emulator.</b>

## Restart PS/2 Controller

```asm
_start:
	# Disable PS/2 port 1 (command 0xAD)
	movb $0xAD, %al
	movb $0x64, %dx
	outb %al, %dx

	# Enable PS/2 port 1 (command 0xAD)
	movb $0xAE, %al
	movb $0x64, %dx
	outb %al, %dx
```

## Read keyboard scan code

```assembly
read_loop:
	# Polling loop to check if data is available 
	inb $0x64, %al
    testb $0x01, %al  # testb - performs bitwise AND, checks if output buffer full (bit 0 set)
    jz read_loop      # If not, loop back to check again

    # Read scan code from PS/2 data port
    inb $0x60, %al # scan code stored in %al
```

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