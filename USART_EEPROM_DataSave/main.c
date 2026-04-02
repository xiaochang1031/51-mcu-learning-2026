#include "stm32f10x.h"
#include "Serial.h"      // 你的串口
#include "AT24C02.h"     // 你的EEPROM
#include "Delay.h"
#include <stdlib.h>      // atoi 字符串转数字

int main(void)
{
    
    AT24C02_Init();      // EEPROM初始化
    Serial_Init();       // 串口初始化

    // ================================
    // 【关键】上电就读取，发给电脑
    // 断电重启后自动显示数据
    // ================================
    uint8_t last = AT24C02_ReadByte(0x00);
    Serial_Printf("上电成功！上次保存的数据：%d\r\n", last);

    while(1)
    {
        // 如果收到电脑发来 @xxx\r\n
        if(Serial_RxFlag == 1)
        {
            // 转成数字
            uint8_t data = atoi(Serial_RxPacket);

            // 写入EEPROM
            AT24C02_WriteByte(0x00, data);

            // 回复电脑
            Serial_Printf("已保存到EEPROM：%d\r\n", data);

            // 清空标志
            Serial_RxFlag = 0;
        }
		
    }
}
