#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "AT24C02.h"

#define AT24C02_I2C			I2C2
#define AT24C02_I2C_CLK		RCC_APB1Periph_I2C2
#define AT24C02_GPIO_CLK	RCC_APB2Periph_GPIOB
#define AT24C02_SCL_PIN		GPIO_Pin_10
#define AT24C02_SDA_PIN		GPIO_Pin_11
#define AT24C02_GPIO_PORT	GPIOB

void AT24C02_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	I2C_InitTypeDef		I2C_InitStructure;
	
	RCC_APB2PeriphClockCmd(AT24C02_GPIO_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(AT24C02_I2C_CLK,ENABLE);
	
	//开漏复用
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=AT24C02_SCL_PIN|AT24C02_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AT24C02_GPIO_PORT,&GPIO_InitStructure);
	
	I2C_DeInit(AT24C02_I2C);
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed=100000;
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;	//时钟占空比
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1=0x00;
	I2C_Init(AT24C02_I2C,&I2C_InitStructure);

	I2C_Cmd(AT24C02_I2C,ENABLE);
}

void AT24C02_WriteByte(uint8_t addr,uint8_t data)			//硬件实现I2C通信
{
	while(I2C_GetFlagStatus(AT24C02_I2C,I2C_FLAG_BUSY));	//如果busy就等待
	
	I2C_GenerateSTART(AT24C02_I2C,ENABLE);
	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_MODE_SELECT));	//主机模式选择
	
	I2C_Send7bitAddress(AT24C02_I2C,AT24C02_ADDR,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//EV8字节正在发送
	
	I2C_SendData(AT24C02_I2C,addr);
	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(AT24C02_I2C,data);
	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(AT24C02_I2C,ENABLE);
	Delay_ms(5);
}

//uint8_t AT24C02_ReadByte(uint8_t addr)
//{
//	uint8_t data=0;
//	
//	while(I2C_GetFlagStatus(AT24C02_I2C,I2C_FLAG_BUSY));
//	
//	I2C_GenerateSTART(AT24C02_I2C,ENABLE);
//	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_MODE_SELECT));
//	
//	I2C_Send7bitAddress(AT24C02_I2C,AT24C02_ADDR,I2C_Direction_Transmitter);
//	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//	
//	I2C_SendData(AT24C02_I2C,addr);
//	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//	
//	//复合
//	I2C_GenerateSTART(AT24C02_I2C,ENABLE);
//	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_MODE_SELECT));
//	
//	
//	I2C_Send7bitAddress(AT24C02_I2C,AT24C02_ADDR,I2C_Direction_Receiver);
//	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//	
//	I2C_AcknowledgeConfig(AT24C02_I2C,DISABLE);
//	I2C_GenerateSTOP(AT24C02_I2C,ENABLE);
//	
//	while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_BYTE_RECEIVED));
//	data=I2C_ReceiveData(AT24C02_I2C);
//	
//	I2C_AcknowledgeConfig(AT24C02_I2C,ENABLE);
//	return data;
//	
//}
uint8_t AT24C02_ReadByte(uint8_t addr)
{
    uint8_t data=0;
    
    while(I2C_GetFlagStatus(AT24C02_I2C,I2C_FLAG_BUSY));
    
    I2C_GenerateSTART(AT24C02_I2C,ENABLE);
    while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(AT24C02_I2C,AT24C02_ADDR,I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    I2C_SendData(AT24C02_I2C,addr);
    while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    // 重新启动
    I2C_GenerateSTART(AT24C02_I2C,ENABLE);
    while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(AT24C02_I2C,AT24C02_ADDR,I2C_Direction_Receiver);
    while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    // ==== 正确顺序 ====
    I2C_AcknowledgeConfig(AT24C02_I2C,DISABLE);   // 关闭应答
    
    while(!I2C_CheckEvent(AT24C02_I2C,I2C_EVENT_MASTER_BYTE_RECEIVED));  // 等待收到
    data = I2C_ReceiveData(AT24C02_I2C);          // 取数据
    
    I2C_GenerateSTOP(AT24C02_I2C,ENABLE);         // 最后发停止位
    I2C_AcknowledgeConfig(AT24C02_I2C,ENABLE);    // 恢复ACK
    
    return data;
}
