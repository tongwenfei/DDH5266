/*************����ģ�鹦��****************
**ģ�鹦�ܣ�������ʼ������
**�� д �ߣ�Pulang
**��д���ڣ�2018��1��25��
**�� �� �ţ�V1.0
********************************************/

#ifndef _KEY_H
#define _KEY_H

//ͷ�ļ�����
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "delay.h"

//�궨��
#define    KEY1DOWN     0X00000001  //-- key1�½��� 
#define    KEY1UP       0X00000002  //-- key1������ 
#define    KEY1INVALID  0X00000000  //-- key1��Ч    

#define    KEY2DOWN     0X00000010  //-- key2�½���  
#define    KEY2UP       0X00000020  //-- key2������  
#define    KEY2INVALID  0X00000000  //-- key2��Ч    

#define    KEY3DOWN     0X00000100  //-- key3�½���  
#define    KEY3UP       0X00000200  //-- key3������  
#define    KEY3INVALID  0X00000000  //-- key3��Ч "

extern unsigned int backlight_off_count;
extern unsigned int key_value;

//��������
void  key_init(void);
unsigned int key_scan(void);
void  Sample_Crtl_init(void);

#endif