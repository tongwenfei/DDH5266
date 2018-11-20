#ifndef _LCD_h
#define _LCD_h

//头文件引入
#include "stm8l15x.h"
#include "stm8l15x_lcd.h"
#include "led.h"
#include "delay.h"
#define    BACKLIGHT_ON() GPIO_SetBits(GPIOF,GPIO_Pin_6) 
#define    BACKLIGHT_OFF()  GPIO_ResetBits(GPIOF,GPIO_Pin_6) 

typedef enum
{
 uint_LEL=0,
 uint_V,
 uint_Celsius,
 uint_A,
 uint_mA
}Uint_Type;
 typedef enum
{
 loop_menu=0,
 query_menu,
 setting_menu,
 signal_menu,
 threshold_menu,
 alert_menu,
}Menu_Type;
typedef enum
{
 leak_current=1,
 temp_ch1,
 temp_ch2,
 temp_ch3,
 temp_ch4,
 current_ch1,
 current_ch2,
 current_ch3
}Channel_Type;
typedef enum
{
 setting_1111=1,
 setting_2222
}Setting_Type;
typedef enum
{
 bit1=1,
 bit2,
 bit3,
 bit4
}Bit_Type;

extern Channel_Type mesure_channel;
extern Menu_Type lcd_menu;
extern Setting_Type setting_channel;
extern Channel_Type threshold_channel;
extern Bit_Type     threshold_bit;

//函数声明
void Lcd_Init();
void Lcd_Clear(void);
void LcdDisplay(u16 Value);
void MAIN_KeyProcess(u8 n4,u8 n3,u8 n2,u8 n1);//主窗口
void SELF_NUMBER_KeyProcess(void);//本机地址设置
void Displaysetting(void);
void CHENK_NUMBER_KeyProcess(void);
void AppCheck(void);
void Lcd_Disp_Unit(Uint_Type uint);
void Lcd_Disp_Point(uint8_t pos);
void Lcd_Disp_Loop(Channel_Type channel );
void Refresh_Lcd_Menu(void);


#endif

