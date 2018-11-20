/*************ADCģ�鹦��****************
**ģ�鹦�ܣ���ʱ����ʱ����
**�� д �ߣ�Pulang
**��д���ڣ�2018��1��25��
**�� �� �ţ�V1.0
********************************************/
#ifndef _ADC_H
#define _ADC_H

//ͷ�ļ�����
#include "stm8l15x.h"
#include "stm8l15x_adc.h"
#include "SystemPara.h"
#include "stdio.h"
#include "string.h"
extern uint16_t Buffer[10][8];
extern uint16_t filter_value[8];
extern float filter_voltage[8];
//��������
void ADC_init(void);
u16 ADC_Data_Read(void);
void ADC_DMAInit(void);
void filter_adc_value(uint16_t *filter_value,float*filter_voltage);
int ntc100k_res_to_temp(float resistance);
float vol_to_res(float vol);
#endif