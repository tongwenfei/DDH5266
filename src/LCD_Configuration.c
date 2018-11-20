
/* Includes -------------------------------------------------------------------*/
//#include"includes.h"

/*******************************************************************************
* Function Name   : SYS_CLK_Configuration
* Description     : 系统时钟配置(不包括模块时钟)
* Inputs          : None
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/
//注意：启动时，主时钟源默认为HSI  RC时钟的8分频，即f(HSI/8)
void SYS_CLK_Configuration(void) 
{
  //使能系统时钟切换
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
* Description     : LCD配置如下:
                    - 时钟源 = LSE (32.768 KHz)       
                    - 电压源 = Internal
                    - 预分频 = 2
                    - 分频 = 18 (16 + 2)  
                    - 控制模式 = 1/4 Duty, 1/3 Bias
                    - LCD 频率 = (时钟源/(预分频* 分频数))* Duty
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

  //选择seg8-seg15作为LCD段码输入端(stm8l152K6 32pin 无SEG0-SEG7)
  //特别注意：STM8L152K6只有4个COM，且固定在PA4\5\6和PD1端口 
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

