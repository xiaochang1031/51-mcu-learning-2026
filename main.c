#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "Nixie.h"

sbit Buzzer=P2^5;

#define SPEED	500		//这里可以决定歌曲播放速度

#define P	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36

//这里是音调的表
unsigned int FreqTable[]={
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64524,
  64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
  65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};

//这个是歌曲《起风了》，其实我不会看谱，然后我叫豆包生成的谱也听不出来是什么歌，然后下面这个是我去小红书里面找然后叫豆包帮我改一下的
unsigned char code Music[]={						//flash只能读不能改
P,4,
M4,2, M4_,2, M5_,2, M6_,2, L6_,4, H1_,2, M6_,6, P,2,
M4,2, M4_,2, M5_,2, M6_,2, L5_,4, H1_,2, M6_,2, M5_,2, M6_,2, M4_,2, M5_,2, M4,2, M4_,2, M2_,2, P,4,

M4,2, M4_,2, M5_,2, M6_,2, L6_,4, H1_,2, M6_,6, P,2,
M4,2, M4_,2, M5_,2, M6_,2, L5_,4, H1_,2, M6_,2, M5_,2, M6_,2, M4_,2, M5_,2, M4,2, M4_,2, M2_,2, P,4,

L5_,6, L4_,2, L5_,6, L4_,2, L5_,4, L6_,4, M2_,4, L6_,4, L5_,6, L4_,2,
L5_,6, L4_,2, L5_,2, L6_,2, L5_,2, L4_,2, L1,4, P,4,

L5_,6, L4_,2, L5_,6, L4_,2, L5_,4, L6_,4, M2_,4, L6_,4, L5_,6, L6_,2, L5_,4, L4_,4, L5_,8, P,8,

L5_,6, L4_,2, L5_,6, L4_,2, L5_,4, L6_,4, M2_,4, L6_,4, L5_,6, L6_,2, L5_,4, L4_,4, L2,4, P,4,

L6_,2, L5_,2, L4_,2, L5_,2, L4_,4, P,4,
L6_,2, L5_,2, L4_,2, L5_,2, L4_,4, P,4,
L6_,2, L5_,2, L4_,2, L5_,2, L4_,8, P,8, P,4, L4_,4,

L5_,4, L6_,4, L4_,4, M2_,4, M1_,2, M2_,6, P,2, L4_,2, M4,4, M2_,2, M4,6, P,4,
M4,4, M2_,2, M4,6, L6_,4, M4_,2, M5_,2, M4_,2, M4,2, M2_,4, M1_,4,

M2_,4, M1_,2, M2_,4, M1_,2, M2_,2, M1_,2, M2_,4, M1_,2, L5_,4, M2_,4, L6_,8, P,8,

L4_,4, L5_,4, L6_,4, L4_,4, M2_,4, M1_,2, M2_,6, P,2, L4_,2, M4,4, M2_,2, M4,6, P,4,
M4,4, M2_,2, M4,6, L6_,4, M4_,2, M5_,2, M4_,2, M4,2, M2_,4, M1_,4,

M2_,4, M6_,2, M6_,6, M1_,4, M2_,4, M6_,2, M6_,4, M1_,4,
M2_,2, M2_,12, P,4, M4_,4, M5_,4, M6_,4, H2_,2, H1_,6, H2_,2, H1_,6, H2_,2, H1_,6, M5_,4, M6_,6,

H2_,2, H1_,6, H2_,2, H1_,6, H2_,2, H1_,4, M6_,8, M5_,4, M4_,2, M2_,4, M4_,4, M4_,2, M5_,4, M4_,2,
M2_,4, M4_,4, M6_,8, M6_,4, M5_,6, P,4, M4_,4, M5_,4, M6_,4, H2_,2, H1_,6, H2_,2, H1_,6, H2_,2, H1_,6,

P,2, M5_,2, M6_,4, H2_,2, H1_,6, H2_,2, H1_,6, H2_,2, H1_,6, M6_,6, M5_,4, M4_,2, M2_,4, M6_,4, M5_,4,
M4_,2, M2_,4, M2_,2, M4_,2, M4_,8, P,4, M2_,2, M6_,6, M5_,4, M4_,2, M2_,4, M6_,4, M5_,4, M4_,2, M2_,4,

M2_,2, M4_,6, M4_,8,

0xFF
};
unsigned char FreqSelect,MusicSelect;
unsigned char pwm_count = 0;
#define VOLUME  4 			//这里可以控制音量，数字越大音量越高，这里可以输入1到4
void main()
{
	Timer0Init();	
	while(1)
	{
		Nixie(1,(MusicSelect/16)%10);			//数码管显示进度
		
		if(Music[MusicSelect]!=0xFF)
		{
			FreqSelect=Music[MusicSelect];
			MusicSelect++;
			Delay(SPEED/4*Music[MusicSelect]);
			MusicSelect++;
			TR0=0;
			Delay(5);
			TR0=1;
		}
		else
		{
			TR0=0;
			while(1);
		}
	}
}
void Timer0_Routine() interrupt 1
{
	//重装定时器初值，保证音调不变
	if(FreqTable[FreqSelect])
	{
		TL0 = FreqTable[FreqSelect]%256;		
		TH0 = FreqTable[FreqSelect]/256;
	}

	//PWM计数器计数
	pwm_count++;
	if(pwm_count >= 4)		//记到4就清零  
		pwm_count = 0;

	if(pwm_count < VOLUME)		//VOLUME越大，满足条件次数越多，声音越大
	{
		Buzzer = !Buzzer;	//蜂鸣器电平翻转，发声 				 
	}
	else
	{
		Buzzer = 1;        //静音
	}
}