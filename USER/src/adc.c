/*************ADC模块功能****************
**模块功能：定时器延时函数
**编 写 者：Pulang
**编写日期：2018年1月25日
**版 本 号：V1.0
********************************************/
#include "adc.h"

//全局变量
u16 Buff[10] = {0};
uint16_t Buffer[10][8]  = {0};
#define ADC1_DR_ADDRESS        ((uint16_t)0x5344)
#define BUFFER_SIZE            ((uint8_t) 80)
#define BUFFER_ADDRESS         ((uint32_t)(&Buffer))
#define ADC_REF_VOLTAGE        0.0008056640625       //(3.30/4096.0)
#define LEAK_CURRENT_GAIN      			2000
#define RESIDUAL_CURRENT_GAIN		   	2000
uint16_t filter_value[8]={0};
float filter_voltage[8]={0.0};
const float temp_res[]={
975.8038,920.5962,868.8615,820.3603,774.871,732.1889,692.1238,654.4999,619.154,585.9346,
554.7016,525.3245,497.6821,471.6621,447.1599,424.0781,402.3264,381.8204,362.4818,344.2375,
327.0195,310.764,295.4121,280.9084,267.2014,254.2428,241.9877,230.394,219.4224,209.0361,
199.2007,189.8841,181.0559,172.6881,164.754,157.229,150.0898,143.3144,136.8825,130.7749,
124.9734,119.4612,114.2223,109.2417,104.5053,100,95.7132,91.6333,87.7492,84.0505,80.5274,
77.1707,73.9717,70.9222,68.0144,65.2411,62.5954,60.0707,57.661,55.3604,53.1635,51.0651,
49.0602,47.1443,45.313,43.5621,41.8878,40.2862,38.7539,37.2876,35.8842,34.5405,33.2538,
32.0214,30.8408,29.7096,28.6253,27.586,26.5895,25.6338,24.7171,23.8376,22.9937,22.1836,
21.4061,20.6594,19.9424,19.2537,18.592,17.9562,17.3452,16.7578,16.193,15.6499,15.1276,
14.6251,14.1417,13.6764,13.2286,12.7976,12.3825,11.9828,11.5978,11.227,10.8697,10.5254,
10.1935,9.8736,9.5652,9.2678,8.9809,8.7042,8.4373,8.1797,7.9312,7.6912,7.4596,7.236,7.0201,
6.8115,6.6101,6.4155,6.2274,6.0457,5.8701,5.7003,5.5362,5.3775,5.224,5.0755,4.9319,4.793,
4.6586,4.5285,4.4026,4.2807,4.1627,4.0484,3.9378,3.8306,3.7268,3.6263,3.5289,3.4345,3.343,
3.2543,3.1683,3.085,3.0042,2.9258,2.8498,2.7761,2.7045,2.6352,2.5678,2.5025,2.4391,2.3775,
2.3178,2.2598,2.2034,2.1487,2.0956,2.044,1.9939,1.9452,1.8978,1.8518,1.8071,1.7637,1.7215,
1.6804,1.6405,1.6017,1.564,1.5273,1.4915,1.4568,1.423,1.3901,1.3582,1.327,1.2967,1.2672,
1.2385,1.2106,1.1833,1.1568,1.131,1.1059,1.0814,1.0576,1.0343,1.0117,0.9896,0.9681,0.9472,
0.9268,0.9069,0.8875,0.8686,0.8501,0.8321,0.8146,0.7975,0.7808,0.7646,0.7487,0.7332,0.7181,
0.7034,0.689,0.6749,0.6612,0.6479,0.6348,0.6221,0.6096,0.5975,0.5856,0.574,0.5627,0.5517,
0.5409,0.5303,0.52,0.5099,0.5001,0.4905,0.4811,0.4719,0.463,0.4542,0.4456,0.4372,0.429,
0.421,0.4132,0.4055,0.398,0.3907,0.3836,0.3765,0.3697,0.363,0.3564,0.35,0.3437,0.3376,
0.3315,0.3257,0.3199,0.3142,0.3087,0.3033,0.298,0.2928,0.2878,0.2828,0.2779,0.2732,0.2685,
0.2639,0.2594,0.255,0.2507,0.2465,0.2424,0.2384,0.2344,0.2305};//-20~250 Celsius
/******************************************
**函数功能：ADC初始化
**参数说明：
*  管脚对应的通道 
*  PB5 -- ADC1_IN13        ADC-C0    温度采集
*  PB6 -- ADC1_IN12        ADC-C1    温度采集
*  PB7 -- ADC1_IN11        ADC-C2
*  PF0 -- ADC1_IN24        ADC-C3
*  PF1 -- ADC1_IN25        ADC-C4
*  PC4 -- ADC1_IN4         ADC-I3    电流采集
*  PD5 -- ADC1_IN9         ADC-I2    电流采集
*  PD4 -- ADC1_IN10        ADC-I1    电流采集
**返 回 值：
*******************************************/
void ADC_init(void)
{
  /*Initializes the GPIOx according to the specified parameters.*/
  GPIO_Init(GPIOF, GPIO_Pin_1, GPIO_Mode_In_FL_No_IT);     //GPIO初始化模拟输入
  /*Enables or disables the specified peripheral clock.*/
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  /*Deinitializes the ADC peripheral registers to their default reset */
  ADC_DeInit(ADC1);
   /* Initialise and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
   /* Enable ADC1 Channel 3 */
  ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);
  /*Configures the status of the Schmitt Trigger for the selected ADC */
  //ADC_SchmittTriggerConfig(ADC1,ADC_Channel_0,DISABLE);
  /* Start ADC1 Conversion using Software trigger*/
  ADC_SoftwareStartConv(ADC1);
}

/******************************************
**函数功能：ADC转换值读取
**参数说明：
**返 回 值：采集值
*******************************************/
u16 ADC_Data_Read(void)
{
  u16 i,j,temp,AD_Value = 0;
  for(i = 0; i < 10; i++)
  {
    /*Starts ADC conversion, by software trigger*/
    ADC_SoftwareStartConv(ADC1);
    /*Checks whether the specified ADC flag is set or not.*/
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == 0);  //等待转换结束
    /*Returns the last ADC converted data.*/ 
    AD_Value = ADC_GetConversionValue(ADC1);           //读取ADC1，通道25的转换结果
    /*Clears the ADC's pending flags.*/
    ADC_ClearFlag (ADC1,ADC_FLAG_EOC);                 //清除标志位
    Buff[i] = AD_Value;
  }
  for(i=0;i<9;i++)   //C语言冒泡排序发提高ADC采集精度
  {
    for(j=i+1;j<10;j++)
    {
      if(Buff[i] > Buff[j])
      {
        temp = Buff[i];
        Buff[i] = Buff[j];
        Buff[j] = temp;
      }
    }
  }
  /* 求平均值 */
  temp = 0;
  for(i = 1; i < 9; i++)  {  temp += Buff[i]; }
  temp=temp/8; 
  return  temp;
}

void ADC_DMAInit(void)
{
     ADC_DeInit(ADC1);
     DMA_GlobalDeInit();
     CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
     CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);
     ADC_Init(ADC1,ADC_ConversionMode_Continuous,ADC_Resolution_12Bit,ADC_Prescaler_2);
     ADC_SamplingTimeConfig(ADC1,ADC_Group_SlowChannels,ADC_SamplingTime_384Cycles);
     ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
     ADC_Cmd(ADC1, ENABLE);   
	 ADC_ChannelCmd(ADC1,ADC_Channel_4,ENABLE); //current3
	 ADC_ChannelCmd(ADC1,ADC_Channel_9,ENABLE);//current2
     ADC_ChannelCmd(ADC1,ADC_Channel_10,ENABLE);//current1
     ADC_ChannelCmd(ADC1,ADC_Channel_11,ENABLE);//temp2
     ADC_ChannelCmd(ADC1,ADC_Channel_12,ENABLE);//temp1
     ADC_ChannelCmd(ADC1,ADC_Channel_13,ENABLE);//leak_current
     ADC_ChannelCmd(ADC1,ADC_Channel_24,ENABLE);//temp3
     ADC_ChannelCmd(ADC1,ADC_Channel_25,ENABLE);//temp4

     

     SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);
     DMA_Init(
               DMA1_Channel0,
               BUFFER_ADDRESS,
               ADC1_DR_ADDRESS,
               BUFFER_SIZE,
               DMA_DIR_PeripheralToMemory,
               DMA_Mode_Circular,
               DMA_MemoryIncMode_Inc,
               DMA_Priority_High,
               DMA_MemoryDataSize_HalfWord);
	DMA_Cmd(DMA1_Channel0, ENABLE);
	DMA_ITConfig(DMA1_Channel0,DMA_ITx_TC,ENABLE);
	DMA_GlobalCmd(ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
   //ADC_ExternalTrigConfig(ADC1, ADC_ExtEventSelection_Trigger2,ADC_ExtTRGSensitivity_Falling);
}
void filter_adc_value(uint16_t *filter_value,float*filter_voltage)

{
	uint32_t temp[8]={0};
	u16 i=0,j=0,k=0;
	for(k=0;k<8;k++)
	{
		for(i=0;i<9;i++)   //C语言冒泡排序发提高ADC采集精度
		{
			for(j=i+1;j<10;j++)
			{
			  if(Buffer[i][k] > Buffer[j][k])
			  {
			    temp[k] = Buffer[i][k];
			    Buffer[i][k] = Buffer[j][k];
			    Buffer[j][k] = temp[k];
			  }
			}
		}
	}
	
 	/* 求平均值 */
  	memset(temp,0,sizeof(temp));
	for(k=0;k<8;k++)
	{
		for(i = 1; i < 9; i++) 
		{  
			temp[k] += Buffer[i][k]; 
		}
		temp[k]=temp[k]/8; 
		filter_value[k]=temp[k];
		filter_voltage[k]=temp[k]*ADC_REF_VOLTAGE;
		/*if(k<=2)sample_value->current[2-k]=temp[k];
		else if(k<=4)sample_value->temp[4-k]=temp[k];
		else if(k==5)sample_value->leak_current=temp[k];
		else if(k<=7)sample_value->temp[k-3]=temp[k];*/
	}
  
}
int ntc100k_res_to_temp(float resistance)
{
  u16 low = 0;
  u16 mid = sizeof(temp_res) / sizeof(temp_res[0]) / 2;
  u16 high = sizeof(temp_res) / sizeof(temp_res[0]) - 1;
  
  if((resistance > temp_res[0]) || (resistance < temp_res[high]))
  {
    return -1000;      // 超限，不做特殊处理  返回0度
  }
  
  while(high >= low)
  {
    mid = (high + low) / 2;
    
    if(resistance <= temp_res[mid])
    {
      if(resistance > temp_res[mid + 1])
      {
	return (1 * mid + (resistance - temp_res[mid]) * 1 / ((temp_res[mid + 1]) - temp_res[mid]))-20;
	// return ( mid + (resistance - temp_res[mid]) / ((temp_res[mid + 1]) - temp_res[mid]));
      }
      else
      {
	low = mid +1;
      }
    }
    else if(resistance > temp_res[mid])
    {
      high = mid -1;
    }
  }
  
  return 1000; // 查找失败，不做特殊处理
}
float vol_to_res(float vol)
{
 	float a=0,b=0;
	a=2700.0*vol*1.0;
	b=330-127*vol*1.0;
	return a/b;
}
float vol_to_leak_current(float vol_mv)
{
	float current=0;
	current=vol_mv*LEAK_CURRENT_GAIN/499.0*(1+27.0/100.0);
	return current;
}
float vol_to_residual_current(float vol_mv)
{
	float current=0;
	current=vol_mv*RESIDUAL_CURRENT_GAIN/499.0*(1+27.0/100.0);
	return current;
}