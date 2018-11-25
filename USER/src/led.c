/*******************************************************************************
*项目名称：检测温度源
*硬件连接：LED4--PG4
           LED3--PG6
           LED5--PG5
*******************************************************************************/
#include "LED.h"

/*函数功能：LED初始化
 *参数说明：无
 *返 回 值：无
 *作    者：
**/
void led_init(void)
{
  GPIO_DeInit( GPIOG);                                     //GPIOx外围寄存器初始化为它们的默认重置值。
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow); //GPIO初始化推挽输出
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //GPIO初始化推挽输出
  ALARM_OFF();
  FAULT_OFF();
}
void check_current_init(void)
{                                   //GPIOx外围寄存器初始化为它们的默认重置值。
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast); //GPIO初始化推挽输出
   GPIO_ResetBits(GPIOC,GPIO_Pin_1); 
}





