#ifndef __AT24C02_H
#define __AT24C02_H

#define AT24C02_ADDR		0xA0

void AT24C02_Init(void);
void AT24C02_WriteByte(uint8_t addr,uint8_t data);
uint8_t AT24C02_ReadByte(uint8_t addr); 

#endif
