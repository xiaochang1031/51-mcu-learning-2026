#include "stm32f10x.h"
#include "Serial.h"      
#include "AT24C02.h"    
#include "Delay.h"
#include <stdlib.h>      

int main(void)
{
    
    AT24C02_Init();      // EEPROM初始化
    Serial_Init();       // 串口初始化

    uint8_t last = AT24C02_ReadByte(0x00);						
    Serial_Printf("上电成功！上次保存的数据：%d\r\n", last);	//读出断电前的数据

    while(1)
    {
        // 如果收到电脑发来 @xxx\r\n
        if(Serial_RxFlag == 1)		//标志位为1
        {
            
            uint8_t data = atoi(Serial_RxPacket);	// 转成数字
            AT24C02_WriteByte(0x00, data);	// 写入EEPROM（AT24C02）
            Serial_Printf("已保存到EEPROM：%d\r\n", data);
            Serial_RxFlag = 0;		// 清空标志
        }	
    }
}
