/*************事件模块功能****************
**模块功能：漏电温度报警事件处理
**编 写 者：Pulang
**编写日期：2018年1月25日
**版 本 号：V1.0
********************************************/
#include "Temputerature.h"
#include "adc.h"
#include "Lcd.h"
#include "led.h"

//温度检测1处理
void Temperature1Process(void)
{
  u16 temVal;
  u16 ADtemp;
  ADtemp = ADC_Data_Read();   //ADC获取的值（模拟量）
  /********************************************************************
  分压电路25度对应电阻值100K   假如V2为25度模拟电压值
  *V1 = （100k*100k） / （100k+100k）
  *V2 ={ V1 / （27K+V1）}*3300 
  分压电路26度对应电阻值95.724K   假如V2为26度模拟电压值
  *V1 = （100k*95.724k） / （100k+95.724）
  *V2 ={ V1 / （27K+V1）}*3300
  ********************************************************************/
  for (temVal=0; temVal<150; temVal++)
  {
    if (TemputeratureTable[temVal] > ADtemp)
    {
      break;	
    }		
  }
  LcdDisplay(ADtemp);
  ADtemp = temVal;
}