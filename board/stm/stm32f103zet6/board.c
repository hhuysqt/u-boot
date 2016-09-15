/*
 * 改写板子的初始化函数
 */

/*
 * Board specific code for the STmicro STM32F429 Discovery board
 */

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_fsmc.h"
#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Init FSMC GPIOs based
 */
static int fmc_fsmc_setup_gpio(void)
{
	{

		/* USART GPIO */

		GPIO_InitTypeDef io_usart;

		/* Enable clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,
				ENABLE);

		io_usart.GPIO_Speed = GPIO_Speed_50MHz;

		/* TXD */
		io_usart.GPIO_Pin = GPIO_Pin_9;
		io_usart.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &io_usart);

		/* RXD */
		io_usart.GPIO_Pin = GPIO_Pin_10;
		io_usart.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &io_usart);
	}
	{
		/* FSMC GPIO */
		GPIO_InitTypeDef io_fsmc;

		/* Enable clock */
		RCC_APB2PeriphClockCmd(
				RCC_APB2Periph_GPIOD | 
				RCC_APB2Periph_GPIOE | 
				RCC_APB2Periph_GPIOF | 
				RCC_APB2Periph_GPIOG, 
				ENABLE);

		io_fsmc.GPIO_Speed = GPIO_Speed_50MHz;
		io_fsmc.GPIO_Mode = GPIO_Mode_AF_PP;

		/* DATA lines */
		io_fsmc.GPIO_Pin = 
			GPIO_Pin_0 |    // D2
			GPIO_Pin_1 |    // D3
			GPIO_Pin_8 |    // D13
			GPIO_Pin_9 |    // D14
			GPIO_Pin_10 |   // D15
			GPIO_Pin_14 |   // D0
			GPIO_Pin_15;    // D1
		GPIO_Init(GPIOD, &io_fsmc);
		io_fsmc.GPIO_Pin = 
			GPIO_Pin_7 |    // D4
			GPIO_Pin_8 |    // D5
			GPIO_Pin_9 |    // D6
			GPIO_Pin_10 |   // D7
			GPIO_Pin_11 |   // D8
			GPIO_Pin_12 |   // D9
			GPIO_Pin_13 |   // D10
			GPIO_Pin_14 |   // D11
			GPIO_Pin_15;    // D12
		GPIO_Init(GPIOE, &io_fsmc);

		/* ADDRESS lines */
		io_fsmc.GPIO_Pin =
			GPIO_Pin_11 |   // A16 
			GPIO_Pin_12 |   // A17
			GPIO_Pin_13;    // A18
		GPIO_Init(GPIOD, &io_fsmc);
		io_fsmc.GPIO_Pin =
			GPIO_Pin_3 |    // A19
			GPIO_Pin_4 |    // A20
			GPIO_Pin_5;     // A21
		GPIO_Init(GPIOE, &io_fsmc);
		io_fsmc.GPIO_Pin =
			GPIO_Pin_0 |    // A0
			GPIO_Pin_1 |    // A1
			GPIO_Pin_2 |    // A2
			GPIO_Pin_3 |    // A3
			GPIO_Pin_4 |    // A4
			GPIO_Pin_5 |    // A5
			GPIO_Pin_12 |   // A6
			GPIO_Pin_13 |   // A7
			GPIO_Pin_14 |   // A8
			GPIO_Pin_15;    // A9
		GPIO_Init(GPIOF, &io_fsmc);
		io_fsmc.GPIO_Pin = 
			GPIO_Pin_0 |    // A10
			GPIO_Pin_1 |    // A11
			GPIO_Pin_2 |    // A12
			GPIO_Pin_3 |    // A13
			GPIO_Pin_4 |    // A14
			GPIO_Pin_5;     // A15
		GPIO_Init(GPIOG, &io_fsmc);

		/* RD, WR, CS signals */
		io_fsmc.GPIO_Pin =
			GPIO_Pin_4 |    // NOE
			GPIO_Pin_5 |    // NWE
			GPIO_Pin_7;     // NCE2 --> NAND flash
		GPIO_Init(GPIOD, &io_fsmc);
		io_fsmc.GPIO_Pin =
			GPIO_Pin_9 |    // NE2  --> NOR flash
			GPIO_Pin_10 |   // NE3  --> pSRAM
			GPIO_Pin_12;    // NE4  --> LCD
		GPIO_Init(GPIOG, &io_fsmc);
		io_fsmc.GPIO_Pin =
			GPIO_Pin_0 |    // NUB  --> pSRAM
			GPIO_Pin_1;     // NLB  --> pSRAM
		GPIO_Init(GPIOE, &io_fsmc);
		io_fsmc.GPIO_Pin =
			GPIO_Pin_6;     // NWAIT
		io_fsmc.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOD, &io_fsmc);
	}
	return 0;
}

/*
 * Early hardware init.
 */
int board_init(void)
{
	int rv;

	rv = fmc_fsmc_setup_gpio();
	if (rv)
		return rv;

	return 0;
}

/*
 * Dump pertinent info to the console.
 */
int checkboard(void)
{
	printf("Board: STM32F103ZET6-hyq Rev %s\n", CONFIG_SYS_BOARD_REV_STR);

	return 0;
}

/*
 * Rewrote for sram
 */
int sram_init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	{
		/* Bank 1 for NOR flash, pSRAM and LCD */
		FSMC_NORSRAMTimingInitTypeDef timing;
		FSMC_NORSRAMInitTypeDef norsram;

		/* common settings */
		/* timing */
		timing.FSMC_BusTurnAroundDuration = 0;
		timing.FSMC_CLKDivision      = 0;
		timing.FSMC_DataLatency      = 0;
		/* fsmc mode */
		norsram.FSMC_MemoryDataWidth    = FSMC_MemoryDataWidth_16b;
		norsram.FSMC_DataAddressMux     = FSMC_DataAddressMux_Disable;
		norsram.FSMC_AsynchronousWait   = FSMC_AsynchronousWait_Disable;
		norsram.FSMC_WrapMode           = FSMC_WrapMode_Disable;
		norsram.FSMC_WaitSignal         = FSMC_WaitSignal_Disable;
		norsram.FSMC_WaitSignalActive   = FSMC_WaitSignalActive_BeforeWaitState;
		norsram.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
		norsram.FSMC_WriteOperation     = FSMC_WriteOperation_Enable;
		norsram.FSMC_WriteBurst         = FSMC_WriteBurst_Disable;
		norsram.FSMC_BurstAccessMode    = FSMC_BurstAccessMode_Disable;
		norsram.FSMC_ExtendedMode       = FSMC_ExtendedMode_Disable;
		{
			/* NOR flash */
			
			timing.FSMC_AccessMode = FSMC_AccessMode_B;
			timing.FSMC_AddressSetupTime = 2;
			timing.FSMC_AddressHoldTime  = 0;
			timing.FSMC_DataSetupTime    = 5;
			
			norsram.FSMC_Bank            = FSMC_Bank1_NORSRAM2;
			norsram.FSMC_MemoryType      = FSMC_MemoryType_NOR;
			norsram.FSMC_ReadWriteTimingStruct = &timing;
			norsram.FSMC_WriteTimingStruct = &timing;
			
			FSMC_NORSRAMInit(&norsram);
			FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);
		}
		{
			/* pSRAM */
			timing.FSMC_AccessMode = FSMC_AccessMode_A;
			timing.FSMC_AddressSetupTime = 1;
			timing.FSMC_AddressHoldTime  = 1;
			timing.FSMC_DataSetupTime    = 2;
			
			norsram.FSMC_Bank            = FSMC_Bank1_NORSRAM3;
			norsram.FSMC_MemoryType      = FSMC_MemoryType_SRAM;
			norsram.FSMC_ReadWriteTimingStruct = &timing;
			norsram.FSMC_WriteTimingStruct = &timing;
			
			FSMC_NORSRAMInit(&norsram);
			FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
		}
		{
			/* LCD */
			timing.FSMC_AccessMode = FSMC_AccessMode_A;
			timing.FSMC_AddressSetupTime = 1;
			timing.FSMC_AddressHoldTime  = 1;
			timing.FSMC_DataSetupTime    = 2;
			
			norsram.FSMC_Bank            = FSMC_Bank1_NORSRAM4;
			norsram.FSMC_MemoryType      = FSMC_MemoryType_SRAM;
			norsram.FSMC_ReadWriteTimingStruct = &timing;
			norsram.FSMC_WriteTimingStruct = &timing;
			
			FSMC_NORSRAMInit(&norsram);
			FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
		}
	}
	{
		/* NAND flash */
		FSMC_NAND_PCCARDTimingInitTypeDef timing;
		FSMC_NANDInitTypeDef nand;
		
		timing.FSMC_SetupTime     = 1;
		timing.FSMC_WaitSetupTime = 2;
		timing.FSMC_HoldSetupTime = 1;
		timing.FSMC_HiZSetupTime  = 1;
		
		nand.FSMC_Bank = FSMC_Bank2_NAND;
		nand.FSMC_Waitfeature = FSMC_Waitfeature_Disable;
		nand.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
		nand.FSMC_ECC = FSMC_ECC_Disable;
		nand.FSMC_ECCPageSize = FSMC_ECCPageSize_512Bytes;
		nand.FSMC_TCLRSetupTime = 0;
		nand.FSMC_TARSetupTime = 0;
		nand.FSMC_CommonSpaceTimingStruct = &timing;
		nand.FSMC_AttributeSpaceTimingStruct = &timing;
		
		FSMC_NANDInit(&nand);
		FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
	}

	/*
	 * Fill in global info with description of SRAM configuration
	 */
	gd->bd->bi_dram[0].start = CONFIG_SYS_RAM_BASE;
	gd->bd->bi_dram[0].size  = CONFIG_SYS_RAM_SIZE;

	return 0;
}

/*
 * Register ethernet driver
 */
int board_eth_init(bd_t *bis)
{
	return 0;
}

/*
 * dummy assert
 */
void assert_param(int tmp)
{
}

