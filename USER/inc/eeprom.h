/*************内部EEPROM模块功能****************
**模块功能：内部FLASH使用
**编 写 者：Pulang
**编写日期：2018年1月25日
**版 本 号：V1.0
********************************************/
#ifndef _EEPROM_H
#define _EEPROM_H

//头文件引入
#include "stm8l15x.h"
#include "stm8l15x_flash.h"

#define EEPROM_Start_Add 0x1000

//函数声明
void EEPROM_Init(void);
void EEPROM_Write_8(u32 addr, u8 data);
u16 EEPROM_Read_8(u32 addr);
void EEPROM_Write_32(u32 addr, u32 data);
void EEPROM_Write_bit(u32 addr, u32 data);
void EEPROM_Read_bytes(u32 addr,unsigned short len,unsigned char *Buff);
void EEPROM_Write_bytes(u32 addr, unsigned short len,unsigned char *Buff);
void EEPROM_Erase_bytes(u32 addr, unsigned short len);
#endif