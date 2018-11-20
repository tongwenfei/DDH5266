#include "stm8l15x.h"
#include "stm8l15x_it.h"

#include "lcd.h"
#include "delay.h"
#include "LED.h"
#include "adc.h"
#include "Event.h"
#include "key.h"
#include "eeprom.h"
#include "uart.h"
#include "sys.h"
#include "SystemPara.h"
/*******************************************************************************
**�������ܣ�main
**����˵����
**�� �� ֵ��
**��д���ڣ�2018/3/2
**��    �ߣ�Pulang
********************************************************************************/
void main(void)
{
  CLK_Config();
  delay_init(16);
  EEPROM_Init();                    //�ڲ��洢��ʼ��
  if(is_need_restore_factory())
  {
    restore_factory();
  }
  else 
  {
    load_system_para(&system_para);
  }
  Uart1_Init(9600);
  Lcd_Init();                       //LCD��ʼ��
  led_init();                       //LED��ʼ��
  ADC_DMAInit();                       //ADC��ʼ��
  key_init();                       //KEY��ʼ��
  Sample_Crtl_init();
  Tim2_Init();
  Tim4_Init();
  Tim5_Init();
  //LcdDisplay(1234);
  while(1)
  { 
    /*ALARM_ON();
    delay_ms(500);
    ALARM_OFF();
    delay_ms(500);*/
    /*if(count==1000)
    {
    	if(lcd_menu==loop_menu)
    	{
    		count=0;
			Lcd_Disp_Loop(mesure_channel);
			if(mesure_channel<current_ch3)mesure_channel++;
			else mesure_channel=leak_current;
    		
    	}
    	
    }*/
        if(lcd_menu==loop_menu)
        {
          if(refresh_count>=100)
          {
              Refresh_Lcd_Menu();
              refresh_count=0;
          }
        }
        else if(lcd_menu==threshold_menu)
        {
            if(refresh_count>=100)
            {
              Refresh_Lcd_Menu();
              refresh_count=0;
            }
        }
        else
        {
            if(refresh_count>=5)
            {
              Refresh_Lcd_Menu();
              refresh_count=0;
            }
        }

    //Temperature1Process();
  }      
}


