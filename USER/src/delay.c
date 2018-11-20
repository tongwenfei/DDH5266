/*************定时器模块功能****************
**模块功能：定时器延时函数
**编 写 者：Pulang
**编写日期：2018年1月25日
**版 本 号：V1.0
********************************************/
#include "delay.h"

/******************************************
**函数功能：定时器初始化(形成延时函数ms级别)
**参数说明：
**返 回 值：
*******************************************/
void Tim4_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);//将主时钟信号送给定时器4(L系列单片机必需)
  TIM4_TimeBaseInit(TIM4_Prescaler_128,125);   //初始化定时器4
  TIM4_ARRPreloadConfig(ENABLE);              //使能自动重装
  TIM4_ITConfig(TIM4_IT_Update , ENABLE);     //数据更新中断
  TIM4_Cmd(ENABLE);//开定时器
}
void Tim5_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM5,ENABLE);//将主时钟信号送给定时器4(L系列单片机必需)
  TIM5_TimeBaseInit(TIM5_Prescaler_128,TIM5_CounterMode_Up,125);   //初始化定时器4
  TIM5_ARRPreloadConfig(ENABLE);              //使能自动重装
  TIM5_ITConfig(TIM5_IT_Update , ENABLE);     //数据更新中断
  TIM5_Cmd(ENABLE);//开定时器
}
void Tim2_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);//将主时钟信号送给定时器4(L系列单片机必需)
  TIM2_TimeBaseInit(TIM2_Prescaler_128,TIM2_CounterMode_Up,12500);   //初始化定时器4
  TIM2_ARRPreloadConfig(ENABLE);              //使能自动重装
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);     //数据更新中断
  TIM2_Cmd(ENABLE);//开定时器
}
/******************************************************************************
                               定义计算变量
******************************************************************************/

volatile u8 fac_us=0; //us延时倍乘数

/**
  *****************************************************************************
  * @Name   : 延时函数初始化
  *
  * @Brief  : 为确保准确度,请保证时钟频率最好为4的倍数,最低8Mhz
  *
  * @Input  : clk: 时钟频率(24/16/12/8等) 
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_init(u8 clk)
{
 if(clk>16)fac_us=(16-4)/4;//24Mhz时,stm8大概19个周期为1us
 else if(clk>4)fac_us=(clk-4)/4; 
 else fac_us=1;
}

/**
  *****************************************************************************
  * @Name   : 延时nus
  *
  * @Brief  : 延时时间=(fac_us*4+4)*nus*(T)
  *           其中,T为CPU运行频率(Mhz)的倒数,单位为us.
  *           准确度:
  *           92%  @24Mhz
  *           98%  @16Mhz
  *           98%  @12Mhz
  *           86%  @8Mhz
  *
  * @Input  : nus: 延时参数
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void delay_us(u16 nus)
{
__asm(
"PUSH A          \n"  //1T,压栈
"DELAY_XUS:      \n"   
"LD A,fac_us     \n"   //1T,fac_us加载到累加器A
"DELAY_US_1:     \n"  
"NOP             \n"  //1T,nop延时
"DEC A           \n"  //1T,A--
"JRNE DELAY_US_1 \n"   //不等于0,则跳转(2T)到DELAY_US_1继续执行,若等于0,则不跳转(1T).
"NOP             \n"  //1T,nop延时
"DECW X          \n"  //1T,x--
"JRNE DELAY_XUS  \n"    //不等于0,则跳转(2T)到DELAY_XUS继续执行,若等于0,则不跳转(1T).
"POP A           \n"  //1T,出栈
);
}

/**
  *****************************************************************************
  * @Name   : 延时nms
  *
  * @Brief  : 为保证准确度,nms不要大于16640.
  *
  * @Input  : nms: 延时参数
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
