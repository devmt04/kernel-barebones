.code16
disk_load:
  pusha
  pushw %dx
  

  movb $0x0e, %ah # BIOS teletype output
  movb $'L', %al  # Print 'L' to indicate loading
  int $0x10
  
  movb $0x02, %ah   #Calling funtion
  
  movb %dh, %al     #al = number of Sector to be read
  movb $0x02, %cl   #cl = sector numberSector 01(0x01) is our boot sector, 0x02 is our 1st availabe sector
  movb $0x00, %ch   #ch = cylinder number
  movb $0x00, %dh   #dh = Head number
  int $0x13         #Calling interupt

  jc disk_error     #jump if error
  
  popw %dx
  popa
  ret

disk_error:
	movb $0x0e, %ah #call bios function to print msg
	movb $'e', %al
	int $0x10	#call bios interuppt to rpint msg

	jmp diskloop

diskloop:
	jmp .

