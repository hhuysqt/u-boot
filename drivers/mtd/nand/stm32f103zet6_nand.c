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

static void stm32f103_fsmc_enable_nand_ecc(struct mtd_info *mtd, int mode)
{
	FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
}

static int stm32f103_fsmc_nand_ecc_calc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	int ecc;
	while(FSMC_GetFlagStatus(FSMC_Bank2_NAND,FSMC_FLAG_FEMPT)==Bit_RESET);
	ecc = FSMC_GetECC(FSMC_Bank2_NAND);
	FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
	ecc_code[0] = ecc;
	ecc_code[1] = ecc >> 8;
	ecc_code[2] = ecc >> 16;
	printf("ECC data: 0x%x\n", ecc);
	return 0;
}

extern int nand_correct_data(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc);
static int stm32f103_fsmc_nand_ecc_correct(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{
	FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);
	nand_correct_data(mtd, dat, read_ecc, calc_ecc);
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
	// 硬件ECC
	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.hwctl = stm32f103_fsmc_enable_nand_ecc;
	nand->ecc.calculate = stm32f103_fsmc_nand_ecc_calc;
	nand->ecc.correct = stm32f103_fsmc_nand_ecc_correct;
	nand->ecc.size = 512;
	nand->ecc.bytes = 3;
	// 指定位宽：8位
	nand->options = 0;

	return 0;
}

