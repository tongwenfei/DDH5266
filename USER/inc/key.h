/*************按键模块功能****************
**模块功能：按键初始化函数
**编 写 者：Pulang
**编写日期：2018年1月25日
**版 本 号：V1.0
********************************************/

#ifndef _KEY_H
#define _KEY_H

//头文件引入
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "delay.h"

//宏定义
#define    KEY1DOWN     0X00000001  //-- key1下降沿 
#define    KEY1UP       0X00000002  //-- key1上升沿 
#define    KEY1INVALID  0X00000000  //-- key1无效    

#define    KEY2DOWN     0X00000010  //-- key2下降沿  
#define    KEY2UP       0X00000020  //-- key2上升沿  
#define    KEY2INVALID  0X00000000  //-- key2无效    

#define    KEY3DOWN     0X00000100  //-- key3下降沿  
#define    KEY3UP       0X00000200  //-- key3上升沿  
#define    KEY3INVALID  0X00000000  //-- key3无效 "

extern unsigned int backlight_off_count;
extern unsigned int key_value;

//函数声明
void  key_init(void);
unsigned int key_scan(void);
void  Sample_Crtl_init(void);

#endif