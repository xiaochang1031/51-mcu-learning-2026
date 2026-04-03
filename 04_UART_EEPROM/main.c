#include <REGX52.H>
#include "UART.h"
#include "AT24C02.h"

#define EEPROM_ADDRESS 0x00		// 定义 EEPROM 存储地址

unsigned char RecData;		// 保存串口收到的数据

void main(void)
{
	UART_Init();				// 初始化串口
	
	RecData = AT24C02_ReadByte(EEPROM_ADDRESS);
	UART_SendByte(RecData);
	//读EEPROM，发给电脑
	while(1)
	{
		// 等待串口中断接收数据
	}
}

//中断
void UART_Routine() interrupt 4
{
	if(RI == 1)		
	{
		RecData = SBUF;				// 取出电脑发的数据
		
		AT24C02_WriteByte(EEPROM_ADDRESS, RecData);			// 写入 EEPROM
		
		
		UART_SendByte(RecData);		//确认收到
		
		RI = 0;	// 清除接收标志
	}
}