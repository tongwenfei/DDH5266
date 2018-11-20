#include "SystemPara.h"
#include "eeprom.h"
#include "stdio.h"
#include "string.h"
system_para_struct system_para;
system_para_struct sample_value;
const unsigned char factory_state[4]={0xaa,0xbb,0xcc,0xdd};
void restore_factory(void)
{
	memset((void*)&system_para,0,sizeof(system_para_struct));
	EEPROM_Erase_bytes(0,FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS-FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
    EEPROM_Write_bytes(0,FACTORY_STORE_SIZE,(unsigned char*)factory_state);
	system_para.leak_current=600;
	for(uint8_t i=0;i<MAX_TEMP_CHANNEL;i++)
	{
		system_para.temp[i]=70;
	}
	for(uint8_t i=0;i<MAX_CURRENT_CHANNEL;i++)
	{
		system_para.current[i]=80;
	}
	for(uint8_t i=0;i<MAX_INPUT_CHANNEL;i++)
	{
		system_para.input_value[i].enable=FALSE;
		system_para.input_value[i].open=TRUE;
	}
	system_para.output_value.enable=FALSE;
	system_para.output_value.alarm=TRUE;
	EEPROM_Write_bytes(SYSTEM_PARA_Start_Add,sizeof(system_para_struct),(unsigned char*)&system_para);
}
bool is_need_restore_factory(void)
{
	uint8_t data[4]={0};
	EEPROM_Read_bytes(0,FACTORY_STORE_SIZE,data);
	if((data[0]==0xaa)&&(data[1]==0xbb)&&(data[2]==0xcc)&&(data[3]==0xdd))
	{
		return FALSE;
	}
	else return TRUE;
}
void load_system_para(system_para_struct *para)
{
	memset((void*)para,0,sizeof(system_para_struct));
	EEPROM_Read_bytes(SYSTEM_PARA_Start_Add,sizeof(system_para_struct),(unsigned char*)para);
}
void save_system_para(system_para_struct para)
{
	memset((void*)&para,0,sizeof(system_para_struct));
	EEPROM_Read_bytes(SYSTEM_PARA_Start_Add,sizeof(system_para_struct),(unsigned char*)&para);
}