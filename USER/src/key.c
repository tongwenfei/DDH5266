/*************����ģ�鹦��****************
**ģ�鹦�ܣ�������ʼ������
**�� д �ߣ�Pulang
**��д���ڣ�2018��1��25��
Ӳ������: 
         key1  -- PG2
         key2  -- PG3
         key3  -- PF7
**�� �� �ţ�V1.0
********************************************/

#include "key.h"
unsigned short  Key1LastLevel=0;   //�����ϴε�ƽֵ
unsigned short  Key2LastLevel=0;   //�����ϴε�ƽֵ
unsigned short  Key3LastLevel=0;   //�����ϴε�ƽֵ
unsigned int backlight_off_count=0;
unsigned int key_value=0;


/******************************************
**�������ܣ�������ʼ��
**����˵����
**�� �� ֵ��
*******************************************/
void  key_init(void)
{
//  GPIO_DeInit( GPIOG);                                     //GPIOx��Χ�Ĵ�����ʼ��Ϊ���ǵ�Ĭ������ֵ��
//  GPIO_DeInit( GPIOF); 
  /*KEY1*/
  GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);        //GPIO��ʼ����������
  Key1LastLevel = (GPIOG->IDR&(0x1<<2)); //PG2�ĵ�ƽ
  /*KEY2*/  
  GPIO_Init(GPIOG, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);       //GPIO��ʼ����������
  Key2LastLevel = (GPIOG->IDR&(0x1<<3));
  /*KEY3*/  
  GPIO_Init(GPIOF, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);        //GPIO��ʼ����������
  Key3LastLevel = (GPIOF->IDR &(1<<0));
}

/*
��������: ����ɨ��
����ֵ:
       0000 0001  -- key1�½��� 
       0000 0002  -- key1������ 
       0000 0000  -- key1��Ч    

       0000 0010  -- key2�½���  
       0000 0020  -- key2������  
       0000 0000  -- key2��Ч    

       0000 0100  -- key3�½���  
       0000 0200  -- key3������  
       0000 0000  -- key3��Ч     
*/
unsigned int key_scan(void)
{
  unsigned int   keyflag=0;    //�ȼٶ����а�����Ч	
  unsigned short CurLevel;
  //KEY1(PG2ɨ��)
  if( GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2) !=  Key1LastLevel )  //������,����
  {
     CurLevel = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2);
     delay_ms(1);  //��ʱȥ��
     if( (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2) )==CurLevel  && CurLevel  !=  Key1LastLevel    )     //�ٴ��ж�
     {
        Key1LastLevel = CurLevel;               //���¸����ϴαȶ�ֵ     
        backlight_off_count=0;
        if( CurLevel )
        { keyflag  |= (2<<0); }                 //2��ʾ������       
        else if( !CurLevel ) 
        { keyflag  |= (1<<0); }                 //1��ʾ�½���
     }
  }
  //KEY2(PG3ɨ��)
  if( GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3) !=  Key2LastLevel )  //������,����
  {
     CurLevel = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3);
     delay_ms(1);  //��ʱȥ��
     if( (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3) )==CurLevel  && CurLevel  !=  Key2LastLevel    )     //�ٴ��ж�
     {
        Key2LastLevel = CurLevel;              //���¸����ϴαȶ�ֵ     
        backlight_off_count=0;
        if( CurLevel )
        { keyflag  |= (2<<4); }              //2��ʾ������       
         else if( !CurLevel ) 
        { keyflag  |= (1<<4); }              //1��ʾ�½���
     }
  }
    //KEY3(PF7ɨ��)
  if( GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7) !=  Key3LastLevel )  //������,����
  {
     CurLevel = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7);
     delay_ms(1);  //��ʱȥ��
     if( (GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7) )==CurLevel  && CurLevel  !=  Key3LastLevel    )     //�ٴ��ж�
     {
        Key3LastLevel = CurLevel;              //���¸����ϴαȶ�ֵ     
        backlight_off_count=0;
        if( CurLevel )
        { keyflag  |= (2<<8); }              //2��ʾ������       
         else if( !CurLevel ) 
        { keyflag  |= (1<<8); }              //1��ʾ�½���
     }
  }
  return  keyflag;					        
}
void  Sample_Crtl_init(void)
{
  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);        //GPIO��ʼ����������
  GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);       //GPIO��ʼ����������
  GPIO_ResetBits(GPIOC, GPIO_Pin_0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}


