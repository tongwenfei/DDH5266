/**
  *****************************************************************************
  *                             ����������صĺ���
  *
  *                       (C) Copyright 2000-2020, ***
  *                            All Rights Reserved
  *****************************************************************************
  *
  * @File    : usart.c
  * @By      : �¹�
  * @Version : V1.0
  * @Date    : 2018 / 01 / 19
  *
  *****************************************************************************
  *                                   Update
  * @Version : V1.*
  * @By      : ***
  * @Date    : 20** / ** / **
  * @Brief   : ***
  *
  *****************************************************************************
**/


#include "usart.h"


/**
  *****************************************************************************
  * @Name   : �ض���out_char����
  *
  * @Brief  : none
  *
  * @Input  : c: �����ַ�
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
int putchar(int c)
{
  UART1->DR = (u8)c;  //��������
  while ((UART1->SR & UART1_SR_TC) == 0);  //�ȴ��������
  
  return c;
}

/**
  *****************************************************************************
  * @Name   : ���ڳ�ʼ��
  *
  * @Brief  : none
  *
  * @Input  : BaudRate: ������
  *           HSE 16MHzʵ����в������У�19200��38400��57600��76800��115200��128000��230400��256000��
  *
  * @Output : none
  *
  * @Return : none
  *****************************************************************************
**/
void UART_Init(u32 BaudRate)
{
  u32 clock_freq = 0;
  
  //�ܽų�ʼ��
  //GPIOD->DDR |= 1<<5 | 1<<6;  //���ģʽ
  //GPIOD->CR1 |= 1<<5 | 1<<6;  //�������
  //GPIOD->CR2 |= 1<<5 | 1<<6;  //10MHz
  
  //��λUART1
  (void)UART1->SR;
  (void)UART1->DR;
  
  UART1->BRR2 = UART1_BRR2_RESET_VALUE;
  UART1->BRR1 = UART1_BRR1_RESET_VALUE;
  
  UART1->CR1 = UART1_CR1_RESET_VALUE;
  UART1->CR2 = UART1_CR2_RESET_VALUE;
  UART1->CR3 = UART1_CR3_RESET_VALUE;
  UART1->CR4 = UART1_CR4_RESET_VALUE;
  UART1->CR5 = UART1_CR5_RESET_VALUE;
  
  UART1->GTR = UART1_GTR_RESET_VALUE;
  UART1->PSCR = UART1_PSCR_RESET_VALUE;
  
  //����UART1
  UART1->CR1 &= ~(UART1_CR1_M | UART1_CR1_PCEN | UART1_CR1_PS);  //8λ���ݣ�һ����ʼλ������żУ��
  UART1->CR3 &= ~UART1_CR3_STOP;  //1��ֹͣλ
  UART1->BRR1 &= ~UART1_BRR1_DIVM;
  UART1->BRR2 &= ~(UART1_BRR2_DIVM | UART1_BRR2_DIVF);
  //�õ�ʱ��Ƶ��
  clock_freq = Sys_GetCPUClockFrequency();
  //���㲨���ʼĴ���ֵ
  //��дBRR2��дBRR1
  UART1->BRR2 = (((clock_freq / BaudRate) >> 8) & 0xF0) | ((clock_freq / BaudRate) & 0x0F);
  UART1->BRR1 = ((clock_freq / BaudRate) >> 4) & 0xFF;
  
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;  //TXD and RXD
  
  UART1->CR3 &= ~(UART1_CR3_CKEN | UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL);  //�ر�SCLK����ͬ��ģʽ
  
  UART1->CR1 &= ~UART1_CR1_UARTD;  //ʹ��UART1
}
