
/* Includes -------------------------------------------------------------------*/
//#include"includes.h"

/*******************************************************************************
* Function Name   : SYS_CLK_Configuration
* Description     : ϵͳʱ������(������ģ��ʱ��)
* Inputs          : None
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/
//ע�⣺����ʱ����ʱ��ԴĬ��ΪHSI  RCʱ�ӵ�8��Ƶ����f(HSI/8)
void SYS_CLK_Configuration(void) 
{
  //ʹ��ϵͳʱ���л�
  CLK_SYSCLKSourceSwitchCmd(ENABLE); 
  
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI); //38KHZ 16M
  
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);//8M
  
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)
         {  

         }

  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

}


/*******************************************************************************
* Function Name   : LCD_Configuration
* Description     : LCD��������:
                    - ʱ��Դ = LSE (32.768 KHz)       
                    - ��ѹԴ = Internal
                    - Ԥ��Ƶ = 2
                    - ��Ƶ = 18 (16 + 2)  
                    - ����ģʽ = 1/4 Duty, 1/3 Bias
                    - LCD Ƶ�� = (ʱ��Դ/(Ԥ��Ƶ* ��Ƶ��))* Duty
                      Flcd = Fclk/(2squ(3:0)*(16+DIV))
                      Fframe = Flcd*duty = 57HZ
* Inputs          : None
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/
void LCD_Configuration(void) 
{


  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_4,
           LCD_Bias_1_3, LCD_VoltageSource_Internal);

  //ѡ��seg8-seg15��ΪLCD���������(stm8l152K6 32pin ��SEG0-SEG7)
  //�ر�ע�⣺STM8L152K6ֻ��4��COM���ҹ̶���PA4\5\6��PD1�˿� 
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0x00); 
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF); 
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x00); 
  LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0x00);
  LCD_PortMaskConfig(LCD_PortMaskRegister_4, 0x00);
  LCD_PortMaskConfig(LCD_PortMaskRegister_5, 0x00);

  LCD_ContrastConfig(LCD_Contrast_3V3);
  LCD_DeadTimeConfig(LCD_DeadTime_0); 
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

  LCD_Cmd(ENABLE); 
  
}

