/*************按键模块功能****************
**模块功能：按键初始化函数
**编 写 者：Pulang
**编写日期：2018年1月25日
硬件连接: 
         key1  -- PG2
         key2  -- PG3
         key3  -- PF7
**版 本 号：V1.0
********************************************/

#include "key.h"
unsigned short  Key1LastLevel=0;   //按键上次电平值
unsigned short  Key2LastLevel=0;   //按键上次电平值
unsigned short  Key3LastLevel=0;   //按键上次电平值
unsigned int backlight_off_count=0;
unsigned int key_value=0;


/******************************************
**函数功能：按键初始化
**参数说明：
**返 回 值：
*******************************************/
void  key_init(void)
{
//  GPIO_DeInit( GPIOG);                                     //GPIOx外围寄存器初始化为它们的默认重置值。
//  GPIO_DeInit( GPIOF); 
  /*KEY1*/
  GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);        //GPIO初始化推挽输入
  Key1LastLevel = (GPIOG->IDR&(0x1<<2)); //PG2的电平
  /*KEY2*/  
  GPIO_Init(GPIOG, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);       //GPIO初始化推挽输入
  Key2LastLevel = (GPIOG->IDR&(0x1<<3));
  /*KEY3*/  
  GPIO_Init(GPIOF, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);        //GPIO初始化推挽输入
  Key3LastLevel = (GPIOF->IDR &(1<<0));
}

/*
函数功能: 按键扫描
返回值:
       0000 0001  -- key1下降沿 
       0000 0002  -- key1上升沿 
       0000 0000  -- key1无效    

       0000 0010  -- key2下降沿  
       0000 0020  -- key2上升沿  
       0000 0000  -- key2无效    

       0000 0100  -- key3下降沿  
       0000 0200  -- key3上升沿  
       0000 0000  -- key3无效     
*/
unsigned int key_scan(void)
{
  unsigned int   keyflag=0;    //先假定所有按键无效	
  unsigned short CurLevel;
  //KEY1(PG2扫描)
  if( GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2) !=  Key1LastLevel )  //屏蔽码,掩码
  {
     CurLevel = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2);
     delay_ms(1);  //延时去抖
     if( (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2) )==CurLevel  && CurLevel  !=  Key1LastLevel    )     //再次判断
     {
        Key1LastLevel = CurLevel;               //重新更新上次比对值     
        backlight_off_count=0;
        if( CurLevel )
        { keyflag  |= (2<<0); }                 //2表示上升沿       
        else if( !CurLevel ) 
        { keyflag  |= (1<<0); }                 //1表示下降沿
     }
  }
  //KEY2(PG3扫描)
  if( GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3) !=  Key2LastLevel )  //屏蔽码,掩码
  {
     CurLevel = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3);
     delay_ms(1);  //延时去抖
     if( (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3) )==CurLevel  && CurLevel  !=  Key2LastLevel    )     //再次判断
     {
        Key2LastLevel = CurLevel;              //重新更新上次比对值     
        backlight_off_count=0;
        if( CurLevel )
        { keyflag  |= (2<<4); }              //2表示上升沿       
         else if( !CurLevel ) 
        { keyflag  |= (1<<4); }              //1表示下降沿
     }
  }
    //KEY3(PF7扫描)
  if( GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7) !=  Key3LastLevel )  //屏蔽码,掩码
  {
     CurLevel = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7);
     delay_ms(1);  //延时去抖
     if( (GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7) )==CurLevel  && CurLevel  !=  Key3LastLevel    )     //再次判断
     {
        Key3LastLevel = CurLevel;              //重新更新上次比对值     
        backlight_off_count=0;
        if( CurLevel )
        { keyflag  |= (2<<8); }              //2表示上升沿       
         else if( !CurLevel ) 
        { keyflag  |= (1<<8); }              //1表示下降沿
     }
  }
  return  keyflag;					        
}
void  Sample_Crtl_init(void)
{
  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);        //GPIO初始化推挽输入
  GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);       //GPIO初始化推挽输入
  GPIO_ResetBits(GPIOC, GPIO_Pin_0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}


