#include <REGX52.H>

void UART_Init()
{
	SCON=0x50;     
	PCON |= 0x80;
	TMOD &= 0x0F;		
    TMOD |= 0x20;		
    TL1 = 0xFA;		
	TH1 = 0xFA;			
    ET1 = 0;	
	TR1 = 1;
    EA=1;          
    ES=1;          
}
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
/*
void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		
		RI=0;
	}
}
*/