#ifndef _LED_H_
#define _LED_H_

#include "stm8l15x_gpio.h"

//#define  ALARM_ON()   PG_ODR_bit.ODR4 = 0     //备注实际接  49脚  PG4
//#define  ALARM_OFF()  PG_ODR_bit.ODR4 = 1     
//#define  FAULT_ON()   PG_ODR_bit.ODR6 = 0     //备注实际接  51脚  PG6
//#define  FAULT_OFF()  PG_ODR_bit.ODR6 = 1 

#define  ALARM_OFF() GPIO_SetBits(GPIOG,GPIO_Pin_4) 
#define  FAULT_OFF() GPIO_SetBits(GPIOG,GPIO_Pin_6) 
#define  ALARM_ON()  GPIO_ResetBits(GPIOG,GPIO_Pin_4) 
#define  FAULT_ON()  GPIO_ResetBits(GPIOG,GPIO_Pin_6)

void led_init(void);
void check_current_init(void);
#endif