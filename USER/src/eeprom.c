/*************内部EEPROM模块功能****************
**模块功能：内部FLASH使用
**编 写 者：Pulang
**编写日期：2018年1月25日
**版 本 号：V1.0
********************************************/
#include "eeprom.h"


/********************************************************
**函数功能：EEPROM初始化
**参数说明：
**返 回 值：
**编写日期：2018/3/2
**作    者：Pulang
********************************************************/
void EEPROM_Init(void)
{
    FLASH_DeInit();                
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    //FLASH_EraseByte(0x1000);
}

/********************************************************
**函数功能：EEPROM写1个字节函数
**参数说明：
**返 回 值：
**编写日期：2018/3/2
**作    者：Pulang
*********************************************************/
void EEPROM_Write_8(u32 addr, u8 data)
{
    //close all interrupt
    disableInterrupts(); 
    //Sets the fixed programming time
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard); 
    // EEPRM unlock
    FLASH_Unlock(FLASH_MemType_Data);
    //wait EEPROM unlock seccess
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
    //write data to EEPROM
    FLASH_ProgramByte(addr + EEPROM_Start_Add,data);
    //Waits for a data write complete.
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    //EEPROM lock
    FLASH_Lock(FLASH_MemType_Data);
    //open all interrupt        
    enableInterrupts(); 
}
/********************************************************
**函数功能：EEPROM读1个字节函数
**参数说明：
**返 回 值：
**编写日期：2018/3/2
**作    者：Pulang
*********************************************************/
u16 EEPROM_Read_8(u32 addr)
{
    u16 eeprom_data;
    //close all interrupt
    disableInterrupts(); 
    //Sets the fixed programming time
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard); 
    // EEPRM unlock
    FLASH_Unlock(FLASH_MemType_Data);
    //wait EEPROM unlock seccess
    eeprom_data = FLASH_ReadByte(EEPROM_Start_Add + addr);
    //Waits for a data read complete.
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    //EEPROM lock
    FLASH_Lock(FLASH_MemType_Data);
    //open all interrupt        
    enableInterrupts();  
    return eeprom_data;
}

/*******************************************************************************
**函数功能：EEPROM写4个字节函数
**参数说明：
**返 回 值：
**编写日期：2018/3/2
**作    者：Pulang
********************************************************************************/
void EEPROM_Write_32(u32 addr, u32 data)
{
  FLASH_Unlock(FLASH_MemType_Data);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
  
  FLASH_WaitForLastOperation(FLASH_MemType_Data);
  
  FLASH_ProgramByte(EEPROM_Start_Add+ addr,   (u8)(data>>24));
  FLASH_WaitForLastOperation(FLASH_MemType_Data);  
  
  FLASH_ProgramByte(EEPROM_Start_Add+ addr+1, (u8)(data>>16));
  FLASH_WaitForLastOperation(FLASH_MemType_Data);  
  
  FLASH_ProgramByte(EEPROM_Start_Add+ addr+2, (u8)(data>>8));
  FLASH_WaitForLastOperation(FLASH_MemType_Data);  
  
  FLASH_ProgramByte(EEPROM_Start_Add+ addr+3, (u8)(data>>0));
  FLASH_WaitForLastOperation(FLASH_MemType_Data); 
  
  FLASH_Lock(FLASH_MemType_Data);
  FLASH_WaitForLastOperation(FLASH_MemType_Data);
}
/********************************************************
**函数功能：EEPROM读n个字节函数
**参数说明：
**返 回 值：
**编写日期：2018/3/2
**作    者：Pulang
*********************************************************/
void EEPROM_Read_bytes(u32 addr,unsigned short len,unsigned char *Buff)
{
    //close all interrupt
    disableInterrupts(); 
    //Sets the fixed programming time
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard); 
    // EEPRM unlock
    FLASH_Unlock(FLASH_MemType_Data);
	//wait EEPROM unlock seccess
	for(unsigned short i=0;i<len;i++)
	{
		*Buff = FLASH_ReadByte(EEPROM_Start_Add + addr+i);
		 Buff++;
	}
    
    
    //Waits for a data read complete.
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    //EEPROM lock
    FLASH_Lock(FLASH_MemType_Data);
    //open all interrupt        
    enableInterrupts();  
    
}
/********************************************************
**函数功能：EEPROM写n个字节函数
**参数说明：
**返 回 值：
**编写日期：2018/3/2
**作    者：Pulang
*********************************************************/
void EEPROM_Write_bytes(u32 addr, unsigned short len,unsigned char *Buff)
{
    //close all interrupt
    disableInterrupts(); 
    //Sets the fixed programming time
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard); 
    // EEPRM unlock
    FLASH_Unlock(FLASH_MemType_Data);
    //wait EEPROM unlock seccess
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
    for(unsigned short i=0;i<len;i++)
    {
      //write data to EEPROM
      FLASH_ProgramByte(addr + EEPROM_Start_Add+i,*Buff);
      Buff++;
    }
    
    //Waits for a data write complete.
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    //EEPROM lock
    FLASH_Lock(FLASH_MemType_Data);
    //open all interrupt        
    enableInterrupts(); 
}
/********************************************************
**函数功能：EEPROM写n个字节函数
**参数说明：
**返 回 值：
**编写日期：2018/3/2
**作    者：Pulang
*********************************************************/
void EEPROM_Erase_bytes(u32 addr, unsigned short len)
{
    //close all interrupt
    disableInterrupts(); 
    //Sets the fixed programming time
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard); 
    // EEPRM unlock
    FLASH_Unlock(FLASH_MemType_Data);
    //wait EEPROM unlock seccess
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
    for(unsigned short i=0;i<len;i++)
    {
      //write data to EEPROM
      FLASH_EraseByte(addr + EEPROM_Start_Add+i);
      
    }
    
    //Waits for a data write complete.
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    //EEPROM lock
    FLASH_Lock(FLASH_MemType_Data);
    //open all interrupt        
    enableInterrupts(); 
}



/*******************************************************************************
**函数功能：EEPROM写函数
**参数说明：addr写入的地址
            data写入的数据
**返 回 值：无
**编写日期：2018/3/2
**作    者：Pulang
********************************************************************************/
/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define FLASH_OPERATION_STARTADDRESS  (uint32_t)0x1000   /* Flash Operation start address */
#define FLASH_WORD                    (uint32_t)0x01234567
#define FLASH_BYTE                    (uint8_t)0xAA

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TestStatus OperationStatus;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void EEPROM_Write_bit(u32 addr, u32 data)
{
  uint32_t add;
  uint8_t index, val;

  /*Define FLASH programming time*/
  FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);

  /* Unlock flash Data memory */
  FLASH_Unlock(FLASH_MemType_Data);

  /* Wait until Data EEPROM area unlocked flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
  {}
  /* Program word at addres 0x1000*/
  FLASH_ProgramWord(FLASH_OPERATION_STARTADDRESS, FLASH_WORD);

  /* Check program word action */
  val = FLASH_ReadByte(FLASH_OPERATION_STARTADDRESS);
  add = FLASH_OPERATION_STARTADDRESS;
  if (val != BYTE_3(FLASH_WORD))
  {
    /* Error */
    OperationStatus = FAILED;
    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
    while (1);
  }

  add += 1;
  val = FLASH_ReadByte(add);
  if (val != BYTE_2(FLASH_WORD))
  {
    /* Error */
    OperationStatus = FAILED;
    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
    while (1);
  }

  add += 1;
  val = FLASH_ReadByte(add);
  if (val != BYTE_1(FLASH_WORD))
  {
    /* Error */
    OperationStatus = FAILED;
    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
    while (1);
  }

  add += 1;
  val = FLASH_ReadByte(add);
  if (val != BYTE_0(FLASH_WORD))
  {
    /* Error */
    OperationStatus = FAILED;
    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
    while (1);
  }

  /* Program one word using program byte */
  add = FLASH_OPERATION_STARTADDRESS;
  for (index = 0; index < 4; index++)
  {
    FLASH_ProgramByte(add, FLASH_BYTE);
    add += 1;
  }

  /* Verify program action */
  add = FLASH_OPERATION_STARTADDRESS;
  for (index = 0; index < 4; index++)
  {
    val = FLASH_ReadByte(add);
    if (val != FLASH_BYTE)
    {
      /* Error */
      OperationStatus = FAILED;
      /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
      /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
      while (1);
    }
    add += 1;
  }
  /* Erase word using Erase byte */
  add = FLASH_OPERATION_STARTADDRESS;
  for (index = 0; index < 4; index++)
  {
    FLASH_EraseByte(add);
    add += 1;
  }

  /* Verify Erase action */
  add = FLASH_OPERATION_STARTADDRESS;
  for (index = 0; index < 4; index++)
  {
    val = FLASH_ReadByte(add);
    if (val != 0x00)
    {
      /* Error */
      OperationStatus = FAILED;
      /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
      /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
      while (1);
    }
    add += 1;
  }
  /* Pass */
  OperationStatus = PASSED;
  /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
  /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
  //while (1);
  
}




