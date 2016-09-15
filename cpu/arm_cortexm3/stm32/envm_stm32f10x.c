/**
 * 封装flash读写
 * 	envm_init
 * 	envm_write
 */

#include "stm32f10x_flash.h"


/*
 * Initialize internal Flash interface
 */
void envm_init(void)
{
}


/*
 * Write a data buffer to internal Flash.
 * Note that we need for this function to reside in RAM since it
 * will be used to self-upgrade U-boot in internal Flash.
 */
u32 __attribute__((section(".ramcode")))
	     __attribute__ ((long_call))
envm_write(u32 offset, void * buf, u32 size)
{
	u32 eraseaddr = offset;
	u32 erasesize, programsize;
	u16* psrc = buf;

	if(offset < 0x08000000 || offset > 0x08040000)
	{
		printf("Address 0x%x is not in flash!!!\n", offset);
		return 0;
	}
	else if(size > 0x00020000)
	{
		printf("%d bytes is too large...\n", size);
		return 0;
	}
	else if(offset & 0x000007ff)
	{
		eraseaddr &= 0xfffff800;
		printf("Warning: addr 0x%x is not aligned to 2048, fixed to %x...\n", offset, eraseaddr);
	}
	printf("Write %d bytes to addr 0x%x\n", size, eraseaddr);

	// begin to program
	FLASH_Unlock();
	// erase sector
	for(erasesize = 0; erasesize < size; erasesize += 2048)
		FLASH_ErasePage(eraseaddr + erasesize);
	// program by half word
	for(
	  programsize = 0;
	  programsize < size;
	  programsize += sizeof(u16),
	  psrc++)
		FLASH_ProgramHalfWord(eraseaddr + programsize, *psrc);
	FLASH_Lock();
	printf("Finished\n");
	return size;
}
