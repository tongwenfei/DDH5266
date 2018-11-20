/*************��ʱ��ģ�鹦��****************
**ģ�鹦�ܣ���ʱ����ʱ����
**�� д �ߣ�Pulang
**��д���ڣ�2018��1��25��
**�� �� �ţ�V1.0
********************************************/
#include "delay.h"

/******************************************
**�������ܣ���ʱ����ʼ��(�γ���ʱ����ms����)
**����˵����
**�� �� ֵ��
*******************************************/
void Tim4_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);//����ʱ���ź��͸���ʱ��4(Lϵ�е�Ƭ������)
  TIM4_TimeBaseInit(TIM4_Prescaler_128,125);   //��ʼ����ʱ��4
  TIM4_ARRPreloadConfig(ENABLE);              //ʹ���Զ���װ
  TIM4_ITConfig(TIM4_IT_Update , ENABLE);     //���ݸ����ж�
  TIM4_Cmd(ENABLE);//����ʱ��
}
void Tim5_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM5,ENABLE);//����ʱ���ź��͸���ʱ��4(Lϵ�е�Ƭ������)
  TIM5_TimeBaseInit(TIM5_Prescaler_128,TIM5_CounterMode_Up,125);   //��ʼ����ʱ��4
  TIM5_ARRPreloadConfig(ENABLE);              //ʹ���Զ���װ
  TIM5_ITConfig(TIM5_IT_Update , ENABLE);     //���ݸ����ж�
  TIM5_Cmd(ENABLE);//����ʱ��
}
void Tim2_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);//����ʱ���ź��͸���ʱ��4(Lϵ�е�Ƭ������)
  TIM2_TimeBaseInit(TIM2_Prescaler_128,TIM2_CounterMode_Up,12500);   //��ʼ����ʱ��4
  TIM2_ARRPreloadConfig(ENABLE);              //ʹ���Զ���װ
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);     //���ݸ����ж�
  TIM2_Cmd(ENABLE);//����ʱ��
}
/******************************************************************************
                               ����������
******************************************************************************/

volatile u8 fac_us=0; //us��ʱ������

/**
  *****************************************************************************
  * @Name   : ��ʱ������ʼ��
  *
  * @Brief  : Ϊȷ��׼ȷ��,�뱣֤ʱ��Ƶ�����Ϊ4�ı���,���8Mhz
  *
  * @Input  : clk: ʱ��Ƶ��(24/16/12/8��) 
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_init(u8 clk)
{
 if(clk>16)fac_us=(16-4)/4;//24Mhzʱ,stm8���19������Ϊ1us
 else if(clk>4)fac_us=(clk-4)/4; 
 else fac_us=1;
}

/**
  *****************************************************************************
  * @Name   : ��ʱnus
  *
  * @Brief  : ��ʱʱ��=(fac_us*4+4)*nus*(T)
  *           ����,TΪCPU����Ƶ��(Mhz)�ĵ���,��λΪus.
  *           ׼ȷ��:
  *           92%  @24Mhz
  *           98%  @16Mhz
  *           98%  @12Mhz
  *           86%  @8Mhz
  *
  * @Input  : nus: ��ʱ����
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_us(u16 nus)
{
__asm(
"PUSH A          \n"  //1T,ѹջ
"DELAY_XUS:      \n"   
"LD A,fac_us     \n"   //1T,fac_us���ص��ۼ���A
"DELAY_US_1:     \n"  
"NOP             \n"  //1T,nop��ʱ
"DEC A           \n"  //1T,A--
"JRNE DELAY_US_1 \n"   //������0,����ת(2T)��DELAY_US_1����ִ��,������0,����ת(1T).
"NOP             \n"  //1T,nop��ʱ
"DECW X          \n"  //1T,x--
"JRNE DELAY_XUS  \n"    //������0,����ת(2T)��DELAY_XUS����ִ��,������0,����ת(1T).
"POP A           \n"  //1T,��ջ
);
}

/**
  *****************************************************************************
  * @Name   : ��ʱnms
  *
  * @Brief  : Ϊ��֤׼ȷ��,nms��Ҫ����16640.
  *
  * @Input  : nms: ��ʱ����
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_ms(u32 nms)
{
 u8 t;
 if(nms>65)
 {
  t=nms/65;
  while(t--)delay_us(65000);
  nms=nms%65;
 }
 delay_us(nms*1000);
}
