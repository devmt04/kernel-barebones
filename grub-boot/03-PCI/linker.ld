ENTRY(_start)
SECTIONS
{
	. = 0x100000;
	.multiboot : {
		*(.multiboot)
	}

	.text : {
		*(.text)
	}
   	.data : {
    		*(.data)
    	}
   	.bss : {
   	     	*(.bss)
   	}
}
