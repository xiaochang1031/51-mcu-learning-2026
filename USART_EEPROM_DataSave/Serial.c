#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>


char Serial_RxPacket[100];
uint8_t Serial_RxFlag;


void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;						//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;							//端口B12
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);								//初始
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;						//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;							//端口B12
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);								//初始
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None  ;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;				//Tx发送模式 Rx接收模式
	USART_InitStructure.USART_Parity=USART_Parity_No;			//不校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;    	//1位停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;	//8位字长
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);					//等待标志位置1
}	

void Serial_SendArray(uint8_t*Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char*String)
{
	uint8_t i;
	for(i=0;String [i]!=0;i++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}

int fputc(int ch,FILE*f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char*format,...)
{
	char String[100];
	va_list arg;			//定义一个参数列表变量
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);			//释放参数表
	Serial_SendString(String);
}

void USART1_IRQHandler(void)		//中断
{
	static uint8_t RxState=0;
	static uint8_t pRxPacket=0;

	if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==1)
	{
		uint8_t RxData=USART_ReceiveData(USART1);
		
		if(RxState==0)				//等待包头
		{
			if(RxData=='@'&&Serial_RxFlag==0)
			{
				RxState=1;
				pRxPacket=0;
			}
		}
		else if(RxState==1)
		{
			if(RxData=='\r')
			{
				RxState=2;
			}
			else
			{
				Serial_RxPacket[pRxPacket]=RxData;
				pRxPacket++;
			}
		}
		else if(RxState==2)
		{
			if(RxData=='\n')
			{
				RxState=0;
				Serial_RxPacket[pRxPacket]='\0';
				Serial_RxFlag=1;
			}
		}
		
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
