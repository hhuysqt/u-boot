/**
 * 包装官方库的RCC设置为以下的接口
 * clock_init
 * clock_get
 */

#include "stm32f10x.h"
#include "system_stm32f10x.h"

/*
 * Clocks enumeration
 */
enum clock {
	CLOCK_SYSCLK,		/* SYSCLK clock frequency expressed in Hz     */
	CLOCK_HCLK,		/* HCLK clock frequency expressed in Hz       */
	CLOCK_PCLK1,		/* PCLK1 clock frequency expressed in Hz      */
	CLOCK_PCLK2,		/* PCLK2 clock frequency expressed in Hz      */
	CLOCK_SYSTICK,		/* Systimer clock frequency expressed in Hz   */
	CLOCK_DIVM,		/* Input clock for PLL, PLLI2S, PLLSAI in Hz */
	CLOCK_END		/* for internal usage			      */
};
/*
 * 暂时先设成固定值
 */
static u32 clock_val[CLOCK_END] = {
	[CLOCK_SYSCLK] = 72000000,
	[CLOCK_HCLK]   = 72000000,
	[CLOCK_PCLK1]  = 36000000,
	[CLOCK_PCLK2]  = 72000000,
	[CLOCK_SYSTICK]=  9000000,
	[CLOCK_DIVM]   =  8000000
};

/*
 * Initialize the reference clocks.
 * 包装官方库，并获取当前的时钟值
 */
void clock_init(void)
{
	SystemInit();
}

/*
 * Return a clock value for the specified clock.
 * Note that we need this function in RAM because it will be used
 * during self-upgrade of U-boot into eNMV.
 * @param clck          id of the clock
 * @returns             frequency of the clock
 */
unsigned long  __attribute__((section(".ramcode")))
	       __attribute__ ((long_call))
	       clock_get(enum clock clck)
{
	return clock_val[clck];
}
