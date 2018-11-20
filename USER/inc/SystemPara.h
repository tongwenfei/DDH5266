#ifndef _SYSTEMPARA_H
#define _SYSTEMPARA_H

//头文件引入
#include "stm8l15x.h"
#include "stm8l15x_flash.h"
#define MAX_TEMP_CHANNEL 4
#define MAX_CURRENT_CHANNEL 3
#define MAX_INPUT_CHANNEL 2
#define MAX_ICCID_CHAR 24
#define MAX_MSISDN_CHAR 20

#define MAX_TEMP_DATA     140
#define MIN_TEMP_DATA     45
#define MAX_CURRENT_DATA     999
#define MIN_CURRENT_DATA     10
#define MAX_LEAK_CURRENT_DATA     999
#define MIN_LEAK_CURRENT_DATA     200
#define ALERT_DISP_TIME      8


#define EEPROM_Start_Add 0x1000
#define FACTORY_STORE_SIZE 4
#define SYSTEM_PARA_Start_Add (0+FACTORY_STORE_SIZE)

typedef struct
{
	bool enable;
	bool open;
}input_struct;
typedef struct
{
	bool enable;
	bool alarm;
}outpu_struct;
typedef struct
{
	uint16_t leak_current;
	uint8_t  temp[MAX_TEMP_CHANNEL];
	uint16_t  current[MAX_CURRENT_CHANNEL];
	input_struct input_value[MAX_INPUT_CHANNEL];
	outpu_struct output_value;
	char iccid[MAX_ICCID_CHAR];
	char msisdn[MAX_MSISDN_CHAR];
}system_para_struct;
extern system_para_struct system_para;
extern system_para_struct sample_value;

void restore_factory(void);
bool is_need_restore_factory(void);
void load_system_para(system_para_struct *para);
void save_system_para(system_para_struct para);


#endif

