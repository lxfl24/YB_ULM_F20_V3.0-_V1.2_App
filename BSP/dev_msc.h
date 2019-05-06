#ifndef __DEV_MSC_H
#define __DEV_MSC_H

#include  "system.h"
#include "em_msc.h"


#define FAIL    0
#define SUCCESS 1

#define   DATABASEADDRESS         0x0fe00000                           /* 定义所要写入字的起始地址 (用户数据区域) */  

#define   Fireword_Update_Flag    0x123456AB

void Init_Msc(void);


Bool MSC_Write_Buffer(uint32_t *buffer,u16 len);



void MSC_Read_Buffer(uint32_t *buffer,u16 len);


u32 MSC_Read_Word(u32 Addr);

Bool Update_flash_Param(u8 * fData,u16 len);


#endif 
