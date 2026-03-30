#include <REGX52.H>

void Timer0Init(void)		//定时器初始化，1毫秒
{			
	TMOD &= 0xF0;		
	TMOD |= 0x01;		
	TL0 = 0x66;     //设置一个初值。记满申请中断	
	TH0 = 0xFC;		
	TF0 = 0;		
	TR0 = 1;	
	ET0=1;
	EA=1;
	PT0=0;
}








/*
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x66;		         //设置定时初值
	TH0 = 0xFC;						 //设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		
	}
	
}
*/