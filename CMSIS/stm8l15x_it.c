/**
  ******************************************************************************
  * @file    EXTI/EXTI_IOControl/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "key.h"
#include "lcd.h"
#include "SystemPara.h"
#include "adc.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_adc.h"
#include "stdio.h"
/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_IOControl
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
uint32_t count=0,refresh_count=0,sample_count=0;
bool adc_sample_tc=FALSE;
static bool is_need_reload_para=FALSE;
static uint8_t alert_sec=0;
#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  if(DMA_GetITStatus(DMA1_IT_TC0))
  {
  	ADC_Cmd(ADC1, DISABLE);
        ADC_DMACmd(ADC1, DISABLE);
  	DMA_ClearITPendingBit(DMA1_IT_TC0);
	adc_sample_tc=TRUE;
	   
  }
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Joystick UP button is pressed */
  PressedButton = BUTTON_UP;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin0);
#endif /* USE_STM8L1528_EVAL */
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL
  /* Check if Joystick left button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_LEFT) == RESET)
  {
    PressedButton = BUTTON_LEFT;
  }

  /* Check if Key button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
    PressedButton = BUTTON_KEY;
  }

  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);

#elif defined USE_STM8L1528_EVAL

  /* Check if Joystick down button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_DOWN) == RESET)
  {
    PressedButton = BUTTON_DOWN;
  }

  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL

  /* Joystick SEL button is pressed */
  PressedButton = BUTTON_SEL;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);

#elif defined USE_STM8L1528_EVAL

  /* Joystick LEFT button is pressed */
  PressedButton = BUTTON_LEFT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Joystick RIGHT button is pressed */
  PressedButton = BUTTON_RIGHT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin3);
#endif /* USE_STM8L1528_EVAL */
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL
  /* Joystick DOWN button is pressed */
  PressedButton = BUTTON_DOWN;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);

#elif defined USE_STM8L1528_EVAL

  /* Joystick SEL button is pressed */
  PressedButton = BUTTON_SEL;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL
  /* Joystick RIGHT button is pressed */
  PressedButton = BUTTON_RIGHT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin5);
#endif /* USE_STM8L1526_EVAL */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Key button is pressed */
  PressedButton = BUTTON_KEY;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
#endif /* USE_STM8L1526_EVAL */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  #ifdef USE_STM8L1528_EVAL
  /* Joystick UP button is pressed */
  PressedButton = BUTTON_UP;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin7);
  #endif /* USE_STM8L1526_EVAL */
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  if(TIM2_GetITStatus(TIM2_IT_Update))
  {
    if(sample_count>=20)
    {
          if(adc_sample_tc)
            {
                  filter_adc_value(filter_value,filter_voltage);
                  adc_sample_tc=FALSE;
                  printf("/********************Start********************/\r\n");
                  for (uint8_t i = 0;  i < 8; i++)
                  {
                          if(i<=2)
                          {
                                  printf("ADC_CH[%d]=%d,电流通道%d,电压=%.3fV\r\n",i,filter_value[i],3-i,filter_voltage[i]);
                          }
                          else if(i<=4)
                          {
                                  
                                  float res=vol_to_res(filter_voltage[i]);
                                  int temp=ntc100k_res_to_temp(res);
                                  if((temp==-1000)||(temp==1000))
                                  {
                                          sample_value.temp[4-i]=255;
                                  }
                                  else if(temp>=0)
                                  {
                                          sample_value.temp[4-i]=temp;
                                  }
                                  else
                                  {
                                          sample_value.temp[4-i]=0;
                                  }
                                  printf("ADC_CH[%d]=%d,温度通道%d,电压=%.3fV,温度=%d\r\n",i,filter_value[i],5-i,filter_voltage[i],sample_value.temp[4-i]);
                          }
                          else if(i==5)
                          {
                                  printf("ADC_CH[%d]=%d,漏电流通道,电压=%.3fV\r\n",i,filter_value[i],filter_voltage[i]);
                          }
                          else if(i<=7)
                          {
                                  
                                  float res=vol_to_res(filter_voltage[i]);
                                  int temp=ntc100k_res_to_temp(res);
                                  if((temp==-1000)||(temp==1000))
                                  {
                                          sample_value.temp[i-4]=255;
                                  }
                                  else if(temp>=0)
                                  {
                                          sample_value.temp[i-4]=temp;
                                  }
                                  else
                                  {
                                          sample_value.temp[i-4]=0;
                                  }
                                  printf("ADC_CH[%d]=%d,温度通道%d,电压=%.3fV,温度=%d\r\n",i,filter_value[i],i-3,filter_voltage[i],sample_value.temp[i-4]);
                          }
                  }
                  
                  
                  printf("/********************End ********************/\r\n");
                  ADC_Cmd(ADC1, ENABLE);
                  ADC_DMACmd(ADC1, ENABLE);
                  ADC_SoftwareStartConv(ADC1);
            }
        sample_count=0;
      
    }
    else
    {
      sample_count++;
    }
    TIM2_ClearITPendingBit(TIM2_IT_Update);        //清除中断标志
  }
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler, 20)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler, 22)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  TIM4_ClearITPendingBit(TIM4_IT_Update);        //清除中断标志
  key_value=key_scan();

 switch (lcd_menu)
 {
 	case loop_menu:
 	{	
		switch (key_value)
		{
			case (1<<0):
			{
				lcd_menu=setting_menu;
			}break;
			case (1<<4):
			{
				lcd_menu=query_menu;
				if(mesure_channel>leak_current)mesure_channel--;
				else mesure_channel=current_ch3;
			}break;
			case (1<<8):
			{
				lcd_menu=query_menu;
				if(mesure_channel<current_ch3)mesure_channel++;
				else mesure_channel=leak_current;
			}break;
		}
		
 	}break;
	case query_menu:
	{
		switch (key_value)
		{
			case (1<<0):
			{
				lcd_menu=setting_menu;
			}break;
			case (1<<4):
			{
				lcd_menu=query_menu;
				if(mesure_channel>leak_current)mesure_channel--;
				else mesure_channel=current_ch3;
			}break;
			case (1<<8):
			{
				lcd_menu=query_menu;
				if(mesure_channel<current_ch3)mesure_channel++;
				else mesure_channel=leak_current;
			}break;
		}
		
	}break;
	case setting_menu:
	{
		switch (key_value)
		{
			case (1<<0):
			{
				lcd_menu=setting_menu;
				if(setting_channel==setting_1111)setting_channel=setting_2222;
				else setting_channel=setting_1111;
			}break;
			case (1<<4):
			{
				if(setting_channel==setting_1111)lcd_menu=signal_menu;
				else
				{
					threshold_bit=bit1;
					threshold_channel=leak_current;
					lcd_menu=threshold_menu;
				}
			}break;
			case (1<<8):
			{
				lcd_menu=loop_menu;
			}break;
		}	
	}break;
	case signal_menu:
	{
		switch (key_value)
		{
			
			case (1<<8):
			{
				lcd_menu=setting_menu;
			}break;
		}	
	}break;
	case threshold_menu:
	{
		switch (threshold_bit)
		{
			case bit1:
			{
				if(is_need_reload_para)
				{
					load_system_para(&system_para);
                    is_need_reload_para=FALSE;
				}
				switch (key_value)
				{
					
					case (1<<8):
					{
						lcd_menu=setting_menu;
					}break;
					case (1<<4):
					{
						threshold_bit=bit2;
					}break;
					case (1<<0):
					{
						if(threshold_channel<current_ch3)threshold_channel++;
						else threshold_channel=leak_current;
					}break;
				}	
			}break;
			case bit2:
			{
				switch (key_value)
				{
					
					case (1<<8):
					{
						threshold_bit=bit1; //返回保存数值，然后重新载入
					}break;
					case (1<<4):
					{
						threshold_bit=bit3;
					}break;
					case (1<<0):
					{
						
						if(threshold_channel==leak_current)
						{
							if((system_para.leak_current/100)<(MAX_LEAK_CURRENT_DATA/100))
							{
								system_para.leak_current+=100;
							}
							else
							{
								system_para.leak_current=system_para.leak_current%100;
							}
						}
						else if(threshold_channel<=temp_ch4)
						{
							if((system_para.temp[threshold_channel-temp_ch1]/100)<(MAX_TEMP_DATA/100))
							{
								system_para.temp[threshold_channel-temp_ch1]+=100;
							}
							else
							{
								system_para.temp[threshold_channel-temp_ch1]=system_para.temp[threshold_channel-temp_ch1]%100;
							}
						}
						else if(threshold_channel<=current_ch3)
						{
							if((system_para.current[threshold_channel-current_ch1]/100)<(MAX_CURRENT_DATA/100))
							{
								system_para.current[threshold_channel-current_ch1]+=100;
							}
							else
							{
								system_para.current[threshold_channel-current_ch1]=system_para.current[threshold_channel-current_ch1]%100;
							}
						}
					}break;
				}	
			}break;
			case bit3:
			{
				switch (key_value)
				{
					
					case (1<<8):
					{
						threshold_bit=bit1; //返回保存数值，然后重新载入
					}break;
					case (1<<4):
					{
						threshold_bit=bit4;
					}break;
					case (1<<0):
					{
						
						if(threshold_channel==leak_current)
						{
							if((system_para.leak_current/10)<(MAX_LEAK_CURRENT_DATA/10))
							{
								system_para.leak_current+=10;
							}
							else
							{
								system_para.leak_current=system_para.leak_current/100*100+system_para.leak_current%10;
							}
						}
						else if(threshold_channel<=temp_ch4)
						{
							if((system_para.temp[threshold_channel-temp_ch1]/10)<(MAX_TEMP_DATA/10))
							{
								system_para.temp[threshold_channel-temp_ch1]+=10;
							}
							else
							{
								system_para.temp[threshold_channel-temp_ch1]=system_para.temp[threshold_channel-temp_ch1]/100*100+system_para.temp[threshold_channel-temp_ch1]%10;
							}
						}
						else if(threshold_channel<=current_ch3)
						{
							if((system_para.current[threshold_channel-current_ch1]/10)<(MAX_CURRENT_DATA/10))
							{
								system_para.current[threshold_channel-current_ch1]+=10;
							}
							else
							{
								system_para.current[threshold_channel-current_ch1]=system_para.current[threshold_channel-current_ch1]/100*100+system_para.current[threshold_channel-current_ch1]%10;
							}
						}
					}break;
				}	
			}break;
			case bit4:
			{
				switch (key_value)
				{
					
					case (1<<8):
					{
						threshold_bit=bit1; //返回保存数值，然后重新载入
					}break;
					case (1<<4):
					{
						threshold_bit=bit2;
						system_para_struct save_para={0};
						load_system_para(&save_para);
						if(threshold_channel==leak_current)
						{
							if((system_para.leak_current==0)||((system_para.leak_current>=MIN_LEAK_CURRENT_DATA)&&(system_para.leak_current<=MAX_LEAK_CURRENT_DATA)))
							{
								save_para.leak_current=system_para.leak_current;
								save_system_para(save_para);
							}
							else
							{
								is_need_reload_para=TRUE;
							}
						}
						else if(threshold_channel<=temp_ch4)
						{
							if((system_para.temp[threshold_channel-temp_ch1]==0)||((system_para.temp[threshold_channel-temp_ch1]>=MIN_TEMP_DATA)&&(system_para.temp[threshold_channel-temp_ch1]<=MAX_TEMP_DATA)))
							{
								save_para.temp[threshold_channel-temp_ch1]=system_para.temp[threshold_channel-temp_ch1];
								save_system_para(save_para);
							}
							else
							{
								is_need_reload_para=TRUE;
							}
						}
						else if(threshold_channel<=current_ch3)
						{
							if((system_para.current[threshold_channel-current_ch1]==0)||((system_para.current[threshold_channel-current_ch1]>=MIN_CURRENT_DATA)&&(system_para.current[threshold_channel-current_ch1]<=MAX_CURRENT_DATA)))
							{
								save_para.current[threshold_channel-current_ch1]=system_para.current[threshold_channel-current_ch1];
								save_system_para(save_para);
							}
							else
							{
								is_need_reload_para=TRUE;
							}
						}
					}break;
					case (1<<0):
					{
						
						if(threshold_channel==leak_current)
						{
							if(system_para.leak_current<MAX_LEAK_CURRENT_DATA)
							{
								system_para.leak_current+=1;
							}
							else
							{
								system_para.leak_current-=system_para.leak_current%10;
							}
						}
						else if(threshold_channel<=temp_ch4)
						{
							if(system_para.temp[threshold_channel-temp_ch1]<MAX_TEMP_DATA)
							{
								system_para.temp[threshold_channel-temp_ch1]+=1;
							}
							else
							{
								system_para.temp[threshold_channel-temp_ch1]-=system_para.temp[threshold_channel-temp_ch1]%10;
							}
						}
						else if(threshold_channel<=current_ch3)
						{
							if(system_para.current[threshold_channel-current_ch1]<MAX_CURRENT_DATA/10)
							{
								system_para.current[threshold_channel-current_ch1]+=1;
							}
							else
							{
								system_para.current[threshold_channel-current_ch1]-=system_para.current[threshold_channel-current_ch1]%10;
							}
						}
					}break;
				}	
			}break;
			case alert_menu:
			{
			}break;
		}
		
	}
 }
 if(backlight_off_count<10000)
 {
 	backlight_off_count++;
    if(!GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_6))
    {
      BACKLIGHT_ON();
    }
 }
 else
 {
 	if(GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_6))
  	{
  		if(lcd_menu!=signal_menu)
		{
			lcd_menu=loop_menu;
		}
  		BACKLIGHT_OFF();
 	}
 }
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  if(TIM5_GetITStatus(TIM5_IT_Update))
  {
  	if(count<1000)
  	{
  		count++;
  	}
	else
	{
		if(lcd_menu==loop_menu)
                {
                        count=0;
                                if(mesure_channel<current_ch3)mesure_channel++;
                                else mesure_channel=leak_current;
                        
                }
		else if(lcd_menu==alert_menu)
		{
			count=0;
			if(alert_sec<ALERT_DISP_TIME)alert_sec++;
			else
			{
			
			}
		}
         }
	if(refresh_count<100)refresh_count++;
  	TIM5_ClearITPendingBit(TIM5_IT_Update);        //清除中断标志
        
  }
  
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
