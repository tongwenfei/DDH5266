/*************��ʱ��ģ�鹦��****************
**ģ�鹦�ܣ�
**�� д �ߣ�
**��д���ڣ�
**�� �� �ţ�
********************************************/
#ifndef _DELAY_H
#define _DELAY_H

//ͷ�ļ�����
#include "stm8l15x.h"
#include "stm8l15x_tim4.h"

//��������
void Tim4_Init (void);
void Tim5_Init(void);
void Tim2_Init(void);
void Tim4_ms(u32 nTime);
void Tim4_Upd(void);
void delay_init(u8 clk); //��ʱ������ʼ��
void delay_us(u16 nus);  //us����ʱ����,���65536us.
void delay_ms(u32 nms);  //ms����ʱ����
#endif