/*******************************************************************************
*��Ŀ���ƣ�����¶�Դ
*Ӳ�����ӣ�LED4--PG4
           LED3--PG6
           LED5--PG5
*******************************************************************************/
#include "LED.h"

/*�������ܣ�LED��ʼ��
 *����˵������
 *�� �� ֵ����
 *��    �ߣ�
**/
void led_init(void)
{
  GPIO_DeInit( GPIOG);                                     //GPIOx��Χ�Ĵ�����ʼ��Ϊ���ǵ�Ĭ������ֵ��
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow); //GPIO��ʼ���������
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //GPIO��ʼ���������
  ALARM_OFF();
  FAULT_OFF();
}
void check_current_init(void)
{                                   //GPIOx��Χ�Ĵ�����ʼ��Ϊ���ǵ�Ĭ������ֵ��
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast); //GPIO��ʼ���������
   GPIO_ResetBits(GPIOC,GPIO_Pin_1); 
}





