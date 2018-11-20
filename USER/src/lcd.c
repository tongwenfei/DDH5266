
#include "lcd.h"
#include "key.h"
#include "SystemPara.h"
//数组声明
Menu_Type lcd_menu=loop_menu;
Channel_Type mesure_channel=leak_current;
Setting_Type setting_channel=setting_1111;
Channel_Type threshold_channel=leak_current;
Bit_Type     threshold_bit=bit1;


unsigned int tmp,n1, n2, n3, n4;  //变量用来定义LCD的段位显示
//码值表                           /* 0     1        2       3      4   */
__CONST uint16_t  NumberMap[10] = {0x77, 0x14, 0x5b, 0x5d, 0x3c,
                                   /*  5      6       7       8      9   */
                                   0x6d, 0x6f, 0x54, 0x7f, 0x7d};
//数值  AFBGCED  码值    
//0     1110111    77
//1     00101000  0220
//2     11100110  3212
//3     11101010  3222

/*******************************************************************************
****函数名称:
****函数功能:LCD初始化
****版本:V1.0
****日期:17-12-23
****入口参数:
****出口参数:
****说明:
********************************************************************************/
void LCD_ReadRAM(LCD_RAMRegister_TypeDef LCD_RAMRegister, uint8_t *LCD_Data)
{
  /* Check function parameters */
  assert_param(IS_LCD_RAM_REGISTER(LCD_RAMRegister));

  /* Copy data bytes to RAM register */
 *LCD_Data= LCD->RAM[LCD_RAMRegister];
}

void Lcd_Init()
{
  GPIO_Init(GPIOF, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow); //GPIO初始化推挽输出
  BACKLIGHT_ON();

  /*Enables or disables the specified peripheral clock.*/
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD,ENABLE);   //启用或禁用指定的外围时钟
  /*Configures the RTC clock (RTCCLK).*/
  CLK_RTCClockConfig(CLK_RTCCLKSource_HSI,CLK_RTCCLKDiv_64); //必须配置RTC时钟源,HSI选为RTC时钟
  /*lcd初始化，*/
  LCD_Init(LCD_Prescaler_8,LCD_Divider_16,LCD_Duty_1_4,LCD_Bias_1_3,LCD_VoltageSource_Internal );//LCD_VoltageSource_External,LCD_VoltageSource_Internal
  /*lcd端口的配置Configures the LCD Port Mask*/
  LCD_PortMaskConfig(LCD_PortMaskRegister_0,0Xff);    // seg 0-7为     seg口
  LCD_PortMaskConfig(LCD_PortMaskRegister_1,0X3f);    // seg 8,9 12-14为seg口
  /*Configures the LCD Contrast.*/
  LCD_ContrastConfig(LCD_Contrast_Level_5);           //配置LCD的对比度0---7
  /*Configures the LCD dead time.*/
  LCD_DeadTimeConfig(LCD_DeadTime_1);                 // 配置LCD液晶的死区时间0--7
  /*Configures the LCD pulses on duration.*/
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);   // 配置LCD脉冲持续时间0--7   
  /*Enables or disables the LCD Controller*/
  LCD_Cmd(ENABLE);                                    // 启用LCD控制器。
}

/*******************************************************************************
**函数名称:
**函数功能:LCD清屏
**版本:V1.0
**日期:17-12-23
**入口参数:Value-需要转换的数值
**出口参数:
**说明:
********************************************************************************/
void Lcd_Clear(void)
{
  /*Writes a word in the specific LCD RAM.*/
  LCD_WriteRAM(LCD_RAMRegister_0,0x00);
  LCD_WriteRAM(LCD_RAMRegister_1,0x00);
  LCD_WriteRAM(LCD_RAMRegister_3,0x00);
  LCD_WriteRAM(LCD_RAMRegister_4,0x00);
  LCD_WriteRAM(LCD_RAMRegister_7,0x00);
  LCD_WriteRAM(LCD_RAMRegister_8,0x00);
  LCD_WriteRAM(LCD_RAMRegister_10,0x00);
  LCD_WriteRAM(LCD_RAMRegister_11,0x00);
  LCD_WriteRAM(LCD_RAMRegister_12,0x00);
}

/*******************************************************************************
**函数名称:
**函数功能:数据转换函数
**版本:V1.0
**日期:17-12-23
**入口参数:Value-需要转换的数值
**出口参数:
**说明:
********************************************************************************/
void DataConvertor(u16 Value) 
{  
  tmp=Value;    
  n4=tmp/1000;
  tmp=tmp%1000;
  n3=tmp/100;
  tmp=tmp%100;	
  n2=tmp/10;
  tmp=tmp%10;		
  n1=tmp; 
 }

/*******************************************************************************
**函数名称:
**函数功能:LCD显示任意字符
**版本:V1.0
**日期:17-12-23
**入口参数:Value-需要转换的数值
**出口参数:
**说明:
********************************************************************************/
void LcdDisplay(u16 Value)
{
  /*Screen clearing function*/
  Lcd_Clear();
  /*data conversion*/
  DataConvertor(Value);
  /*Writes a word in the specific LCD RAM.*/
  LCD_WriteRAM(LCD_RAMRegister_0,((NumberMap[n4]&0x60)>>3)|((NumberMap[n3]&0x60)>>1)|((NumberMap[n2]&0x60)<<1));
  LCD_WriteRAM(LCD_RAMRegister_1,(NumberMap[n1]&0x60)>>5 | 0x20 |0x10);
  LCD_WriteRAM(LCD_RAMRegister_3,(NumberMap[n4]&0x18)<<3);
  LCD_WriteRAM(LCD_RAMRegister_4,((NumberMap[n3]&0x18)>>3)|((NumberMap[n2]&0x18)>>1)|((NumberMap[n1]&0x18)<<1));
  LCD_WriteRAM(LCD_RAMRegister_7,((NumberMap[n4]&0x06)<<1)|((NumberMap[n3]&0x06)<<3)|((NumberMap[n2]&0x06)<<5));
  LCD_WriteRAM(LCD_RAMRegister_8,(NumberMap[n1]&0x06)>>1);
  LCD_WriteRAM(LCD_RAMRegister_10,(NumberMap[n4]&0x01)<<6);
  LCD_WriteRAM(LCD_RAMRegister_11,((NumberMap[n3]&0x01))|((NumberMap[n2]&0x01)<<2)|((NumberMap[n1]&0x01)<<4));
}
void Lcd_Disp_Value_Blink(u16 Value,Bit_Type data_bit)
{
	uint8_t lcd_ram[8]={0};
	static uint8_t blink_count=0;
	/*Screen clearing function*/
	Lcd_Clear();
	/*data conversion*/
	DataConvertor(Value);
	if(blink_count>=5)
	{
		switch(data_bit)
		{
			case bit1:
			{
				lcd_ram[0]=((NumberMap[n3]&0x60)>>1)|((NumberMap[n2]&0x60)<<1);
				lcd_ram[1]=(NumberMap[n1]&0x60)>>5;
				lcd_ram[2]=0;
				lcd_ram[3]=((NumberMap[n3]&0x18)>>3)|((NumberMap[n2]&0x18)>>1)|((NumberMap[n1]&0x18)<<1);
				lcd_ram[4]=((NumberMap[n3]&0x06)<<3)|((NumberMap[n2]&0x06)<<5);
				lcd_ram[5]=(NumberMap[n1]&0x06)>>1;
				lcd_ram[6]=0;
				lcd_ram[7]=((NumberMap[n3]&0x01))|((NumberMap[n2]&0x01)<<2)|((NumberMap[n1]&0x01)<<4);
			}break;
			case bit2:
			{
				lcd_ram[0]=((NumberMap[n4]&0x60)>>3)|((NumberMap[n2]&0x60)<<1);
				lcd_ram[1]=(NumberMap[n1]&0x60)>>5;
				lcd_ram[2]=(NumberMap[n4]&0x18)<<3;
				lcd_ram[3]=((NumberMap[n2]&0x18)>>1)|((NumberMap[n1]&0x18)<<1);
				lcd_ram[4]=((NumberMap[n4]&0x06)<<1)|((NumberMap[n2]&0x06)<<5);
				lcd_ram[5]=(NumberMap[n1]&0x06)>>1;
				lcd_ram[6]=(NumberMap[n4]&0x01)<<6;
				lcd_ram[7]=((NumberMap[n2]&0x01)<<2)|((NumberMap[n1]&0x01)<<4);
			}break;
			case bit3:
			{
				lcd_ram[0]=((NumberMap[n4]&0x60)>>3)|((NumberMap[n3]&0x60)>>1);
				lcd_ram[1]=(NumberMap[n1]&0x60)>>5;
				lcd_ram[2]=(NumberMap[n4]&0x18)<<3;
				lcd_ram[3]=((NumberMap[n3]&0x18)>>3)|((NumberMap[n1]&0x18)<<1);
				lcd_ram[4]=((NumberMap[n4]&0x06)<<1)|((NumberMap[n3]&0x06)<<3);
				lcd_ram[5]=(NumberMap[n1]&0x06)>>1;
				lcd_ram[6]=(NumberMap[n4]&0x01)<<6;
				lcd_ram[7]=((NumberMap[n3]&0x01))|((NumberMap[n1]&0x01)<<4);
			}break;
			case bit4:
			{
				lcd_ram[0]=((NumberMap[n4]&0x60)>>3)|((NumberMap[n3]&0x60)>>1)|((NumberMap[n2]&0x60)<<1);
				lcd_ram[1]=0;
				lcd_ram[2]=(NumberMap[n4]&0x18)<<3;
				lcd_ram[3]=((NumberMap[n3]&0x18)>>3)|((NumberMap[n2]&0x18)>>1);
				lcd_ram[4]=((NumberMap[n4]&0x06)<<1)|((NumberMap[n3]&0x06)<<3)|((NumberMap[n2]&0x06)<<5);
				lcd_ram[5]=0;
				lcd_ram[6]=(NumberMap[n4]&0x01)<<6;
				lcd_ram[7]=((NumberMap[n3]&0x01))|((NumberMap[n2]&0x01)<<2);
			}break;
		}
		LCD_WriteRAM(LCD_RAMRegister_0,lcd_ram[0]);
		LCD_WriteRAM(LCD_RAMRegister_1,lcd_ram[1]);
		LCD_WriteRAM(LCD_RAMRegister_3,lcd_ram[2]);
		LCD_WriteRAM(LCD_RAMRegister_4,lcd_ram[3]);
		LCD_WriteRAM(LCD_RAMRegister_7,lcd_ram[4]);
		LCD_WriteRAM(LCD_RAMRegister_8,lcd_ram[5]);
		LCD_WriteRAM(LCD_RAMRegister_10,lcd_ram[6]);
		LCD_WriteRAM(LCD_RAMRegister_11,lcd_ram[7]);
	}
	else 
	{
		/*Writes a word in the specific LCD RAM.*/
		LCD_WriteRAM(LCD_RAMRegister_0,((NumberMap[n4]&0x60)>>3)|((NumberMap[n3]&0x60)>>1)|((NumberMap[n2]&0x60)<<1));
		LCD_WriteRAM(LCD_RAMRegister_1,(NumberMap[n1]&0x60)>>5 );
		LCD_WriteRAM(LCD_RAMRegister_3,(NumberMap[n4]&0x18)<<3);
		LCD_WriteRAM(LCD_RAMRegister_4,((NumberMap[n3]&0x18)>>3)|((NumberMap[n2]&0x18)>>1)|((NumberMap[n1]&0x18)<<1));
		LCD_WriteRAM(LCD_RAMRegister_7,((NumberMap[n4]&0x06)<<1)|((NumberMap[n3]&0x06)<<3)|((NumberMap[n2]&0x06)<<5));
		LCD_WriteRAM(LCD_RAMRegister_8,(NumberMap[n1]&0x06)>>1);
		LCD_WriteRAM(LCD_RAMRegister_10,(NumberMap[n4]&0x01)<<6);
		LCD_WriteRAM(LCD_RAMRegister_11,((NumberMap[n3]&0x01))|((NumberMap[n2]&0x01)<<2)|((NumberMap[n1]&0x01)<<4));
	}
	if(blink_count<10)blink_count++;
	else blink_count=0;
	
}

/*******************************************************************************
**函数名称:
**函数功能:LCD显示主窗口
**版本:V1.0
**日期:17-12-23
**入口参数:n1,n2,n3,n4需要转换的数值显示段位
**出口参数:
**说明:
********************************************************************************/
void MAIN_KeyProcess(u8 n4,u8 n3,u8 n2,u8 n1)
{
  /*Screen clearing function*/
  Lcd_Clear();
  /*Writes a word in the specific LCD RAM.*/
  LCD_WriteRAM(LCD_RAMRegister_0,((NumberMap[n4]&0x60)>>3)|((NumberMap[n3]&0x60)>>1)|((NumberMap[n2]&0x60)<<1));
  LCD_WriteRAM(LCD_RAMRegister_1,(NumberMap[n1]&0x60)>>5 | 0x20 | 0x10 | 0x04);// 0x20 | 0x10 | 0x04通道，示值，m
  LCD_WriteRAM(LCD_RAMRegister_3,(NumberMap[n4]&0x18)<<3);
  LCD_WriteRAM(LCD_RAMRegister_4,((NumberMap[n3]&0x18)>>3)|((NumberMap[n2]&0x18)>>1)|((NumberMap[n1]&0x18)<<1) | 0x40); //0x40V
  LCD_WriteRAM(LCD_RAMRegister_7,((NumberMap[n4]&0x06)<<1)|((NumberMap[n3]&0x06)<<3)|((NumberMap[n2]&0x06)<<5));
  LCD_WriteRAM(LCD_RAMRegister_8,(NumberMap[n1]&0x06)>>1);
  LCD_WriteRAM(LCD_RAMRegister_10,(NumberMap[n4]&0x01)<<6 );//0x20地址
  LCD_WriteRAM(LCD_RAMRegister_11,((NumberMap[n3]&0x01))|((NumberMap[n2]&0x01)<<2)|((NumberMap[n1]&0x01)<<4));
  LCD_WriteRAM(LCD_RAMRegister_12,0x02);//设置
}
void Lcd_Disp_Point(uint8_t pos)
{
	if((pos>0)&&(pos<4))
	{
		uint8_t lcd_ram[2]={0};
		LCD_ReadRAM(LCD_RAMRegister_10, &lcd_ram[0]);
		LCD_ReadRAM(LCD_RAMRegister_11, &lcd_ram[1]);
		lcd_ram[0]=(lcd_ram[0]&(0xff-0x80));
		lcd_ram[1]=(lcd_ram[1]&(0xff-0x0a));
		switch (pos)
		{
			case 1:lcd_ram[0]|=0x80;break;
			case 2:lcd_ram[1]|=0x02;break;
			case 3:lcd_ram[1]|=0x08;break;
		}
		LCD_WriteRAM(LCD_RAMRegister_10,lcd_ram[0]);//0x20地址
  		LCD_WriteRAM(LCD_RAMRegister_11,lcd_ram[1]);
	}
}
void Lcd_Disp_Unit(Uint_Type uint)
{
	uint8_t lcd_ram[3]={0};
	LCD_ReadRAM(LCD_RAMRegister_1, &lcd_ram[0]);
	LCD_ReadRAM(LCD_RAMRegister_4, &lcd_ram[1]);
	LCD_ReadRAM(LCD_RAMRegister_8, &lcd_ram[2]);
	lcd_ram[0]=(lcd_ram[0]&(0xff-0x0c));
	lcd_ram[1]=(lcd_ram[1]&(0xff-0xc0));
	lcd_ram[2]=(lcd_ram[2]&(0xff-0x08));
	switch (uint)
	{
		case uint_LEL:
		{
			lcd_ram[2]|=0x08;
		}break;
		case uint_V:
		{
			lcd_ram[0]|=0x08;
		}break;
		case uint_Celsius:
		{
			lcd_ram[1]|=0x80;
		}break;
		case uint_A:
		{
			lcd_ram[1]|=0x40;
		}break;
		case uint_mA:
		{
			lcd_ram[0]|=0x04;
			lcd_ram[1]|=0x40;
		}break;
	}
	LCD_WriteRAM(LCD_RAMRegister_1,lcd_ram[0]);//0x20地址
  	LCD_WriteRAM(LCD_RAMRegister_4,lcd_ram[1]);
  	LCD_WriteRAM(LCD_RAMRegister_8,lcd_ram[2]);//设置
	
}
void Lcd_Disp_Setting(void)
{
	uint8_t lcd_ram[2]={0};
	LCD_ReadRAM(LCD_RAMRegister_1,&lcd_ram[0]);
	LCD_ReadRAM(LCD_RAMRegister_12, &lcd_ram[1]);
	lcd_ram[0]=(lcd_ram[0]&(0xff-(0x20|0x10)));
	lcd_ram[1]=(lcd_ram[1]&(0xff-0x02));
	lcd_ram[1]|=0x02;
	LCD_WriteRAM(LCD_RAMRegister_1,lcd_ram[0]);//0x20地址
	LCD_WriteRAM(LCD_RAMRegister_12,lcd_ram[1]);//0x20地址
}
void Lcd_Disp_Threshold(void)
{
	uint8_t lcd_ram=0;
	LCD_ReadRAM(LCD_RAMRegister_3,&lcd_ram);
	lcd_ram=(lcd_ram&(0xff-0x20));
	lcd_ram|=0x20;
	LCD_WriteRAM(LCD_RAMRegister_3,lcd_ram);//0x20地址
}
void Lcd_Disp_Cal(void)
{
	uint8_t lcd_ram=0;
	LCD_ReadRAM(LCD_RAMRegister_7,&lcd_ram);
	lcd_ram=(lcd_ram&(0xff-0x02));
	lcd_ram|=0x02;
	LCD_WriteRAM(LCD_RAMRegister_7,lcd_ram);//0x20地址
}
void Lcd_Disp_CSQ(uint8_t Value)
{
  /*Screen clearing function*/
  Lcd_Clear();
  /*data conversion*/
  DataConvertor(Value);
  /*Writes a word in the specific LCD RAM.*/
  LCD_WriteRAM(LCD_RAMRegister_0,(NumberMap[n2]&0x60)<<1);
  LCD_WriteRAM(LCD_RAMRegister_1,(NumberMap[n1]&0x60)>>5);
  LCD_WriteRAM(LCD_RAMRegister_4,((NumberMap[n2]&0x18)>>1)|((NumberMap[n1]&0x18)<<1));
  LCD_WriteRAM(LCD_RAMRegister_7,(NumberMap[n2]&0x06)<<5);
  LCD_WriteRAM(LCD_RAMRegister_8,(NumberMap[n1]&0x06)>>1);
  LCD_WriteRAM(LCD_RAMRegister_11,((NumberMap[n2]&0x01)<<2)|((NumberMap[n1]&0x01)<<4));
}


/*******************************************************************************
**函数名称:
**函数功能:LCD显示设置菜单项
**版本:V1.0
**日期:17-12-23
**入口参数:
**出口参数:
**说明:
********************************************************************************/
void Displaysetting(void)
{
  /*Screen clearing function*/
  Lcd_Clear();
  /*Writes a word in the specific LCD RAM.*/
  LCD_WriteRAM(LCD_RAMRegister_0,0x04 | 0x08 | 0x10 |0x20 |0x80); //1A,1F,2F,2A,3A
  LCD_WriteRAM(LCD_RAMRegister_3, 0x40); //1G
  LCD_WriteRAM(LCD_RAMRegister_4, 0x01 |0x08); //1G,3B
  LCD_WriteRAM(LCD_RAMRegister_7,0x08 |0x10 | 0x80);//1C,2E,3C
  LCD_WriteRAM(LCD_RAMRegister_10, 0x40); //1D
  LCD_WriteRAM(LCD_RAMRegister_11, 0x01); //2D
}

/*******************************************************************************
**函数名称:
**函数功能:本机地址设置
**版本:V1.0
**日期:17-12-23
**入口参数:
**出口参数:
**说明:
********************************************************************************/
void SELF_NUMBER_KeyProcess(void)
{
  /*Screen clearing function*/
  Lcd_Clear();
  /*Writes a word in the specific LCD RAM.*/
  LCD_WriteRAM(LCD_RAMRegister_0,0x04 | 0x08); //1A,1F
  LCD_WriteRAM(LCD_RAMRegister_3,0x80 | 0x40); //1G,1B
  LCD_WriteRAM(LCD_RAMRegister_7,0x04 | 0x08 | 0x10 | 0x20 | 0x40 | 0x80); //1E,1C,2E,2C,3E,3C
  LCD_WriteRAM(LCD_RAMRegister_4,0x01 | 0x02 | 0x04 | 0x08);               //2G,2B,3B,3G
  LCD_WriteRAM(LCD_RAMRegister_11,0x01 | 0x04);                            //2D,3D
  LCD_WriteRAM(LCD_RAMRegister_12,0x02);                                   //设置
}

/*******************************************************************************
**函数名称:
**函数功能:LCD显示检测菜单项
**版本:V1.0
**日期:17-12-23
**入口参数:
**出口参数:
**说明:
********************************************************************************/
void CHENK_NUMBER_KeyProcess(void)
{
  /*Screen clearing function*/
  Lcd_Clear();
  /*Writes a word in the specific LCD RAM.*/
  LCD_WriteRAM(LCD_RAMRegister_0,0x04 | 0x08 | 0x10 | 0x40 | 0x80); //1A,1F,2F,3F,3E
  LCD_WriteRAM(LCD_RAMRegister_7,0x04 | 0x10 | 0x20 | 0x40); //1E,2E,2C,3E
  LCD_WriteRAM(LCD_RAMRegister_10,0x40); //1D
  LCD_WriteRAM(LCD_RAMRegister_4,0x02 | 0x01 | 0x04);//2B,2G,3G
  LCD_WriteRAM(LCD_RAMRegister_11,0x04 | 0x10);   //3D,4D
  LCD_WriteRAM(LCD_RAMRegister_1,0x01 | 0x02);//4F,4A
  LCD_WriteRAM(LCD_RAMRegister_8,0x01); //4E
  LCD_WriteRAM(LCD_RAMRegister_12,0x02); //设置
}


/*******************************************************************************
**函数名称:
**函数功能:探测器自检状态LCD显示地址全部
**版本:V1.0
**日期:17-12-23
**入口参数:
**出口参数:
**说明:
********************************************************************************/
void AppCheck(void)
{
  static u8 i = 0;
  for(;;)
  {
    /*Writes a word in the specific LCD RAM.*/
    LCD_WriteRAM(LCD_RAMRegister_0,0xff);
    LCD_WriteRAM(LCD_RAMRegister_1,0xff);
    LCD_WriteRAM(LCD_RAMRegister_3,0xff);
    LCD_WriteRAM(LCD_RAMRegister_4,0xff);
    LCD_WriteRAM(LCD_RAMRegister_7,0xff);
    LCD_WriteRAM(LCD_RAMRegister_8,0xff);
    LCD_WriteRAM(LCD_RAMRegister_10,0xff);
    LCD_WriteRAM(LCD_RAMRegister_11,0xff);
    LCD_WriteRAM(LCD_RAMRegister_12,0xff);
    ALARM_ON(); //报警灯电亮
    FAULT_ON(); //故障灯点亮
    for(i = 0;i<50;i++)  {  delay_ms(254);  }
    ALARM_OFF();
    FAULT_OFF();
    Displaysetting();    //检测完成返回设置检测项
    break;              //跳出当前循环
  }
}
void Lcd_Disp_Loop(Channel_Type channel )
{
	uint16_t data=0;
	switch (channel)
	{
		case 1:data=channel*1000+sample_value.leak_current%1000;break;
		case 2:data=channel*1000+sample_value.temp[0]%1000;break;
		case 3:data=channel*1000+sample_value.temp[1]%1000;break;
		case 4:data=channel*1000+sample_value.temp[2]%1000;break;
		case 5:data=channel*1000+sample_value.temp[3]%1000;break;
		case 6:data=channel*1000+sample_value.current[0]%1000;break;
		case 7:data=channel*1000+sample_value.current[0]%1000;break;
		case 8:data=channel*1000+sample_value.current[0]%1000;break;
	}
	LcdDisplay(data);
	Lcd_Disp_Point(1);
	switch (channel)
	{
		case 1:Lcd_Disp_Unit(uint_mA);break;
		case 2:
		case 3:
		case 4:
		case 5:Lcd_Disp_Unit(uint_Celsius);break;
		case 6:
		case 7:
		case 8:Lcd_Disp_Unit(uint_A);break;
	}

}
void Lcd_Disp_Setting_Menu(Setting_Type channel)
{
	switch (channel)
	{
		case setting_1111:
		{
			LcdDisplay(1111);
			Lcd_Disp_Threshold();
		}break;
		case setting_2222:
		{
			LcdDisplay(2222);
			Lcd_Disp_Cal();
		}break;
	}
	Lcd_Disp_Setting();
}
void Lcd_Disp_Signal_Menu(uint8_t csq)
{
	if(csq<32)Lcd_Disp_CSQ(csq);
	else Lcd_Disp_CSQ(00);
	Lcd_Disp_Cal();
	Lcd_Disp_Setting();
}
void Lcd_Disp_Threshold_Setting(Channel_Type channel,Bit_Type data_bit)
{
	

		uint16_t data=0;
		switch (channel)
		{
			case 1:data=channel*1000+system_para.leak_current%1000;break;
			case 2:data=channel*1000+system_para.temp[0]%1000;break;
			case 3:data=channel*1000+system_para.temp[1]%1000;break;
			case 4:data=channel*1000+system_para.temp[2]%1000;break;
			case 5:data=channel*1000+system_para.temp[3]%1000;break;
			case 6:data=channel*1000+system_para.current[0]%1000;break;
			case 7:data=channel*1000+system_para.current[0]%1000;break;
			case 8:data=channel*1000+system_para.current[0]%1000;break;
		}
		Lcd_Disp_Value_Blink(data,data_bit);
		Lcd_Disp_Point(1);
		switch (channel)
		{
			case 1:Lcd_Disp_Unit(uint_mA);break;
			case 2:
			case 3:
			case 4:
			case 5:Lcd_Disp_Unit(uint_Celsius);break;
			case 6:
			case 7:
			case 8:Lcd_Disp_Unit(uint_A);break;
		}

}
void Lcd_Disp_threshold_Menu(Channel_Type channel,Bit_Type data_bit)
{
	

	Lcd_Disp_Threshold_Setting(channel,data_bit);
	Lcd_Disp_Threshold();
	Lcd_Disp_Setting();
}

void Refresh_Lcd_Menu(void)
{
	switch (lcd_menu)
	{
		case loop_menu:
		{
			Lcd_Disp_Loop(mesure_channel);
		}break;
		case query_menu:
		{
			Lcd_Disp_Loop(mesure_channel);
		}break;
		case setting_menu:
		{
			Lcd_Disp_Setting_Menu(setting_channel);
		}break;
		case signal_menu:
		{
			Lcd_Disp_Signal_Menu(00);
		}break;
		case threshold_menu:
		{
			Lcd_Disp_threshold_Menu(threshold_channel,threshold_bit);
		}break;
	}
}

