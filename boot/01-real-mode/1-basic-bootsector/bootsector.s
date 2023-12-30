/*
as bootsector.s -o bootsector.o
ld --oformat binary bootsector.o -o bootsector.bin

*/

.code16

label: 
	/*As computer will run that code first which it encountered in bootsector
	and this code leads to a infinite loop, so when the system boots up
	with this boot sector, system will just go in a infinite loop*/
	jmp label

.fill 510-(. - label),1,0  #this will fill the in-between space with 0s each of 1-byte size, here . represent the current position
#upto here our 510 bytes of boot sector got covered

.word 0xaa55 #boot signature(of 2 bytes). Boot signature must lies at 510 and 511 byte of bootsector

#now here the 1st sector of disk got consumed completely
