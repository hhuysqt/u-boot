/*
 * FSMC驱动nand flash
 */

#include <stm32f10x.h>
#include <stm32f10x_fsmc.h>
#include <stm32f10x_gpio.h>
#include <nand.h>

#define FSMC_NAND_REG_DATA ((volatile u8*)0x70000000)
#define FSMC_NAND_REG_CMD  ((volatile u8*)0x70010000)
#define FSMC_NAND_REG_ADDR ((volatile u8*)0x70020000)


static void stm32f103_fsmc_send_cmd(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	if(cmd != NAND_CMD_NONE)
	{
		if(ctrl & NAND_CLE){
			//printf("nand cmd 0x%x\n", cmd);
			*FSMC_NAND_REG_CMD = (u8)cmd;
		}
		else if(ctrl & NAND_ALE){
			//printf("nand addr 0x%x\n", cmd);
			*FSMC_NAND_REG_ADDR = (u8)cmd;
		}
	}
}

static int stm32f103_fsmc_read_rb(struct mtd_info *mtd)
{
	return GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
}

int board_nand_init(struct nand_chip *nand)
{
	/*
	 * 103的fsmc之前已经初始化了
	 */
	// 设置读写寄存器的地址
	nand->IO_ADDR_R = nand->IO_ADDR_W = FSMC_NAND_REG_DATA;
	// 设置发送命令的函数
	nand->cmd_ctrl = stm32f103_fsmc_send_cmd;
	nand->dev_ready = stm32f103_fsmc_read_rb;
	// 先用软件ECC
	nand->ecc.mode = NAND_ECC_SOFT;
	nand->options = 0;

	return 0;
}

