/**
 * 包装的串口读写函数
 *  serial_init
 *  serial_setbrg
 *  serial_getc
 *  serial_putc
 *  serial_puts
 *  serial_tstc
 */

#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

/*
 * U-Boot global data to get the baudrate from
 */
DECLARE_GLOBAL_DATA_PTR;

s32 serial_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	serial_setbrg();
	return 0;
}

void serial_setbrg(void)
{
	USART_InitTypeDef usart;
	USART_StructInit(&usart);
	usart.USART_BaudRate = 115200;
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);
}

s32 serial_getc(void)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != SET);
	return USART_ReceiveData(USART1);
}

void serial_putc(const char c)
{
	if (c == '\n')
		serial_putc('\r');

	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void serial_puts(const char *s)
{
	while (*s)
		serial_putc(*s++);
}

s32 serial_tstc(void)
{
	return (s32)USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
}

