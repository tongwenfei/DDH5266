/* Includes ------------------------------------------------------------------*/
//#include"includes.h"


/*
================================================================================
                                              []                 (  )   (  )   
        [] [] [] []                           [] []             (    ) (    )                         
        [] [] [] [] []                        [] [] []          (     |     )
        [] [] [] []                           [] [] [] []        (         )
        X1 X2 X3 X4 X5                        T1 T2 T3 T4          (     )   T5
                                                                      |
              1A                    2A                   3A                 
          -----------           ------------          ------------
          |         |           |          |          |          |   
        1F|         |1B       2F|          |2B      3F|          |2B    
          |    1G   |           |          |          |          |
          -----------           ------------          ------------
          |         |           |          |          |          |
        1E|         |1C       2E|          |2C      3E|          |3C
          |         |           |          |          |          |
          -----------           ------------          ------------
               1D                    2D                     3D
================================================================================
   PIN	1   2	3   4	5   6	7   8   9   10  11  12
   COM0 X3 1A  1B  2A   2B  3A  3B  T1 COM1
   COM1 X1 1F  1G  2F   2G  3F  3G  T2     COM2
   COM2 X2 1E  1C  2E   2C  3E  3C  T3          COM3
   COM3 X4 X5  1D       2D  T5  3D  T4              COM4
================================================================================
      COM0 COM1 COM2 COM3
SEG0 { A  , F  , E  , 1  }  
SEG1 { B  , G  , C  , D  }

如显示‘0 ’，则
 { 1 , 1 , 1 , 1 }
 { 1 , 0 , 1 , 1 }
-------------------
=  3   1   3   3  hex
=> '0' = 0x3133

*/

/*Local Variables -------------------------------------------------------------*/
__CONST uint16_t mask[4] = {0xF000, 0x0F00, 0x00F0, 0x000F};
__CONST uint8_t shift[4] = {12 , 8, 4 , 0};
uint8_t LCD_DisBuffer[4];
                                   /* 0     1        2       3      4   */
__CONST uint16_t  NumberMap[10] = {0x3133, 0x2021, 0x3213, 0x3223, 0x2321,
                                   /*  5      6       7       8      9   */
                                   0x1323, 0x1333, 0x3021, 0x3333, 0x3323};
//x5 = 1;NC = 1; T5 = 1;


uint8_t FHR100,FHR10,FHR1;
extern uint8_t FHR ;
uint16_t  FHRLevel;

/*******************************************************************************
* Function Name   : ConvertNum
* Description     : 查找输入参数对应的字符编码
* Inputs          : InputNum--显示字符
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/

void ConvertNum(uint8_t InputNum) 
{
 uint16_t Num, tmp = 0;
 uint8_t i;
 
 if((InputNum>='0')&&(InputNum<='9'))
  {
  Num=NumberMap[InputNum-'0'];
  }
 else
   {
     Num=0;
   }

 for(i = 0;i < 4; i++)
  {
    tmp = Num & mask[i];
    LCD_DisBuffer[i] = (uint8_t)(tmp >> (uint8_t)shift[i]);
  }
 
}


/*******************************************************************************
* Function Name   : LCD_WriteBat
* Description     : 显示电量函数
* Inputs          : batterylevel--电量格数
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/

void LCD_WriteBat(uint16_t BatteryLevel) 
{
   //LCD内存结构:bit7 | bit6 bit5 | bit4 bit3 | bit2 bit1 | bit0 
  
   uint16_t Battery, tmp = 0;
   uint8_t i;
   Battery = BatteryLevel;
   for (i = 0;i < 4; i++)
   {
    tmp = Battery & mask[i];
    LCD_DisBuffer[i] = (uint8_t)(tmp >> (uint8_t)shift[i]);
   } 
   
   /* Enable the write access on the LCD RAM first banck */
    LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
   
   //COM0
   LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)LCD_DisBuffer[0];
   //COM1
   LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)(LCD_DisBuffer[1] << 4);
   //COM2
   LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)LCD_DisBuffer[2];
   //COM3
   LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)(LCD_DisBuffer[3] << 4); 
}

/*******************************************************************************
* Function Name   : LCD_WriteFHR
* Description     : 显示FHR函数
* Inputs          : Position--显示位置；Num--显示数字
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/
void LCD_WriteFHR(uint8_t Position ,uint8_t Num) 
{

ConvertNum(Num);

switch (Position)
  {

   case FHR_1:
     
     /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      //COM0
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(LCD_DisBuffer[0] << 1);
      //COM1
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)(LCD_DisBuffer[1] << 5);
      //COM2
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)(LCD_DisBuffer[2] << 1);
      //COM3
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)(LCD_DisBuffer[3] << 5);
      break;

   case FHR_2:
     
     /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
      //COM0
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(LCD_DisBuffer[0] << 3); 
      //COM1
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)(LCD_DisBuffer[1] << 7);
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)(LCD_DisBuffer[1] >> 1);
      //COM2
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)(LCD_DisBuffer[2] << 3);
      //COM3
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)(LCD_DisBuffer[3] << 7);
      LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)(LCD_DisBuffer[3] >> 1);
      break;

   case FHR_3:
     
     /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
      //COM0
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(LCD_DisBuffer[0] << 5);
      //COM1
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)(LCD_DisBuffer[1] << 1);
      //COM2
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)(LCD_DisBuffer[2] << 5);
      //COM3
      LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)(LCD_DisBuffer[3] << 1);
      break;

   default:
      break;
  }//end of switch (Position)

}

/*******************************************************************************
* Function Name   : LCD_WriteSignal
* Description     : 显示FHR信号强度函数
* Inputs          : signallevel--信号格数
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/
void LCD_WriteSignal(uint16_t SignalLevel) 
{
  
  uint16_t Signal,tmp = 0;
  uint8_t i;
  Signal = SignalLevel;
  for (i = 0;i < 4; i++)
    {
     tmp = Signal & mask[i];
     LCD_DisBuffer[i] = (uint8_t)(tmp >> (uint8_t)shift[i]);
    } 
  
  /* Enable the write access on the LCD RAM First banck */
   LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
  
   //COM0
   LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(LCD_DisBuffer[0] << 7);
   //COM1
   LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)(LCD_DisBuffer[1] << 3);
   //COM2
   LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)(LCD_DisBuffer[2] << 7);
   //COM3
   LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)(LCD_DisBuffer[3] << 3); 
    
}

/*******************************************************************************
* Function Name   : ShowFHR
* Description     : 显示FHR值大小
* Inputs          : ShowFHR--FHR_Val值
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/

void ShowFHR(uint8_t FHR_Val)  
{
 //送显示FHR大小
  FHR100 = (FHR_Val/100) + '0';
  LCD_WriteFHR(FHR_1 , FHR100);
  FHR10 = ((FHR_Val%100)/10) + '0';
  LCD_WriteFHR(FHR_2 , FHR10);
  FHR1 = (FHR_Val%10) + '0';
  LCD_WriteFHR(FHR_3 , FHR1);   
}

/*******************************************************************************
* Function Name   : ShowFHRLevel
* Description     : 显示FHR信号大小
* Calls           :
* Called By       :
* Inputs          : FHR_Level--信号格数
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/

void ShowFHRLevel(uint16_t FHR_Level)
{   
 if(FHR > 150)
   { 
    FHRLevel = SIGNALLEVEL_FULL ;
   }
   else if((FHR > 100) && (FHR <= 150)) 
       {
        FHRLevel = SIGNALLEVEL_3_4; 
       }
      else if((FHR >70) && (FHR <= 100))
           {
            FHRLevel = SIGNALLEVEL_1_2;
           }
         else if((FHR > 40) && (FHR <= 70)) 
              {
               FHRLevel = SIGNALLEVEL_1_4; 
              }
              else
               {
                FHRLevel = SIGNALLEVEL_OFF; 
               }
     
   //送显示信号强度
   LCD_WriteSignal(FHRLevel);   
}

/*******************************************************************************
* Function Name   : LCD_ClearChar
* Description     : 清除LCD缓冲中的字符
* Inputs          : Position 要清除的位置
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/
void LCD_ClearChar(uint8_t Position)
{
  switch (Position)
  {
    case BATTERYLEVEL:
      
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
      //COM0
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x7f ;//清除 bit7
      //COM1
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0x7f ;
      //COM2
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0x7f ;
      //COM3
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0x7f ;
      break;

    case FHR_1:
      
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
      //COM0
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x9f;//清除 bit6 bit5
      //COM1
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0x9f;
      //COM2
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0x9f;
      //COM3
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0x9f;
      break;

    case FHR_2:
      
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
      //COM0
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xe7;//清除 bit4 bit3
      //COM1
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xef;//清除bit4
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xf7;//清除bit3
      //COM2
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xe7;//清除 bit4 bit3
      //COM3
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xef;//清除bit4
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xf7;//清除bit3
      break;

    case FHR_3:
      
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xf9;//清除 bit2 bit1
      //COM1
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xfb;//清除bit2
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xfd;//清除bit1
      //COM2
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xf9;//清除 bit2 bit1
      //COM3
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xfb;//清除bit2
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xfd;//清除bit1
      
    case SIGNALLEVEL:
      
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
      //COM0
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xfe ;//清除 bit0
      //COM1
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xfe ;
      //COM2
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xfe ;
      //COM3
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xfe ;
      break;

    default:
      break;
  }// end of switch (Position)
  
}


/*******************************************************************************
* Function Name   : LCD_Clear
* Description     : 清除整个LCD显示
* Inputs          : None
* Outputs         : None
* Return          : None
* Notes           : None
*******************************************************************************/ 

void LCD_Clear(void)
{
  uint8_t counter = 0x00;
  
  /* Enable the write access on the LCD RAM First banck */
  LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      
  for (counter = 0x00;counter < 0x0e; counter++)
  {
    LCD->RAM[counter] =  0x00;
  }
}
