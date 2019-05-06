#include "dev_msc.h"

  

void Init_Msc(void)
{
  
    /*
     *  解锁对MSC的访问和设置MSC对Flash操作的时间基准
     */
    MSC_Init();    
    
    /*
     *  由于内核运行时钟频率高于16MHz低于32MHz，因此需要将读等待周期配置为WS1
     */
    MSC->READCTRL |= MSC_READCTRL_MODE_WS1SCBTP;
  
}


Bool MSC_Write_Buffer(uint32_t *buffer,u16 len)
{
   /*
     *  Flash写或擦除操作中，不允许发生中断，否则会硬件故障或内核锁死
     */
  __disable_irq();
    
    /*
     *  擦除需要写入字的存储单元所在页 
     */
    MSC_ErasePage((uint32_t *)DATABASEADDRESS);

    /*
     *  向Flash写入数据并判断写操作是否成功
     */
    if (MSC_WriteWord((uint32_t *)DATABASEADDRESS, (uint32_t *)buffer, len*4) != 0) {
        return FAIL;                             /* 写入失败*/
    }
    __enable_irq();     
    
   return SUCCESS;
}

void MSC_Read_Buffer(uint32_t *buffer,u16 len)
{
   u16 i=0;
   uint32_t *Prt=(uint32_t *)DATABASEADDRESS;
   for(i=0;i<len;i++)
   	{
      buffer[i]=*Prt;
	  Prt++;
   	}
}


u32 MSC_Read_Word(u32 Addr)
{
    return *(u32 *)Addr;
}

Bool Update_flash_Param(u8 * fData,u16 len)
{
     uint32_t buffer[10]={0}; 
	 if(len<28) return 0;
	 buffer[0]=Fireword_Update_Flag;
	 buffer[1]=((fData[3]&0xff)<<24)|((fData[2]&0xff)<<16)|((fData[1]&0xff)<<8)|(fData[0]&0xff);
	 buffer[2]=((fData[7]&0xff)<<24)|((fData[6]&0xff)<<16)|((fData[5]&0xff)<<8)|(fData[4]&0xff);
	 buffer[3]=((fData[11]&0xff)<<24)|((fData[10]&0xff)<<16)|((fData[9]&0xff)<<8)|(fData[8]&0xff);
	 buffer[4]=((fData[15]&0xff)<<24)|((fData[14]&0xff)<<16)|((fData[13]&0xff)<<8)|(fData[12]&0xff);
	 buffer[5]=((fData[19]&0xff)<<24)|((fData[18]&0xff)<<16)|((fData[17]&0xff)<<8)|(fData[16]&0xff);
	 buffer[6]=((fData[23]&0xff)<<24)|((fData[22]&0xff)<<16)|((fData[21]&0xff)<<8)|(fData[20]&0xff);
	 buffer[7]=((fData[27]&0xff)<<24)|((fData[26]&0xff)<<16)|((fData[25]&0xff)<<8)|(fData[24]&0xff);
	 if(MSC_Write_Buffer(buffer,8)==0)
	 	{
	 	  return 0;
	 	}
	 return 1;
}