/*************ADC模块功能****************
**模块功能：定时器延时函数
**编 写 者：Pulang
**编写日期：2018年1月25日
**版 本 号：V1.0
********************************************/
#ifndef _ADC_H
#define _ADC_H

//头文件引入
#include "stm8l15x.h"
#include "stm8l15x_adc.h"
#include "SystemPara.h"
#include "stdio.h"
#include "string.h"
extern uint16_t Buffer[10][8];
extern uint16_t filter_value[8];
extern float filter_voltage[8];
//函数声明
void ADC_init(void);
u16 ADC_Data_Read(void);
void ADC_DMAInit(void);
void filter_adc_value(uint16_t *filter_value,float*filter_voltage);
int ntc100k_res_to_temp(float resistance);
float vol_to_res(float vol);
#endif