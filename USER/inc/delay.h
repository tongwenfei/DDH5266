/*************定时器模块功能****************
**模块功能：
**编 写 者：
**编写日期：
**版 本 号：
********************************************/
#ifndef _DELAY_H
#define _DELAY_H

//头文件引入
#include "stm8l15x.h"
#include "stm8l15x_tim4.h"

//函数声明
void Tim4_Init (void);
void Tim5_Init(void);
void Tim2_Init(void);
void Tim4_ms(u32 nTime);
void Tim4_Upd(void);
void delay_init(u8 clk); //延时函数初始化
void delay_us(u16 nus);  //us级延时函数,最大65536us.
void delay_ms(u32 nms);  //ms级延时函数
#endif