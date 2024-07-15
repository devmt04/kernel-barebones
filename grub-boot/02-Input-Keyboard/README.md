# VGA Text Mode

Printing to the screen using VGA in kernel development involves directly interacting with the VGA hardware. The VGA text mode allows you to write characters and their attributes directly to a specific memory address.

VGA text mode uses memory-mapped I/O, with the starting address typically being <b>0xB8000</b> for the first video memory segment in color text mode.

The screen in text mode is represented as a 2D array of characters, where each character cell consists of two bytes:

The first byte is the ASCII character to be displayed.
The second byte is the attribute byte (foreground and background color).

Screen Dimensions: Standard VGA text mode is 80 columns by 25 rows.

## Writing to VGA Text Mode

```c
volatile uint16_t *video_memory = (volatile uint16_t *video_memory)0xB8000;
uint8_t color = 0x0F; // 0x0F is typically white on black
uint16_t value = 'H' | (color << 8); // This will put char 'H' at first byte(or 8 bit) of 2-byte(or 16-bit) video_memory address(which is here 0xB8000, i.e, starting address of VGA Framebuffer) and the other 1-bye color attribute value to next byte of video_memory address(which would be 0xB8001).
*video_memory = value;

// it's advisable to use a volatile qualifier when working with memory-mapped I/O, which ensures that the compiler doesn't optimize away reads or writes to this memory location
```

## Compile the Project

```bash
make
```

This will create a .iso disk image in outs folder.

## Run the Project

```bash
make run
```

## Clean the Project Directory

```bash
make clean
```