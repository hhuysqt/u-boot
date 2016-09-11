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
	s32 ret = 0;
	return ret;
}
