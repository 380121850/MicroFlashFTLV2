#ifndef  __Storage_Module_H
#define  __Storage_Module_H


#define SectorSize     0XFFF  //扇区大小
#define RAMBufferMax   SectorSize
#define SPIFlashMod    0X00
#define ParalFlashMod  0X01
#define ReadMOD        0X00
#define WriteMod       0X01


struct StorageStruct
{
    unsigned char Status;
    unsigned char StorageMod;
    unsigned long CurrenAddr;
    unsigned int  DataLent;
    unsigned char RAMBuffer[RAMBufferMax+1];
};
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
#define  RAMBufferBusy_BIT           BIT0


/******************************************************************************
  函数(模块)名称:void Storage_Module_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void Storage_Module_Init(void);

/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  功能:	
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned int ReadLent, unsigned char StorageMod);

/******************************************************************************
  函数(模块)名称:void Storage_Module_Init(void)
  功能:	FLASH缓冲区更新
  输入参数:需要操作的FLASH地址，数据缓冲区地址，数据长度，读写选择以及存储选择         	              		
  输出参数: 无         	 		   		 
  其它说明:分存储类型：SPI或是并行的FLASH，并分读写两大类，再 
*******************************************************************************/
void FlashRAM_UPdate(unsigned long ProceAddr,unsigned char *Buffer,
                              unsigned long DataLent,unsigned char PrecessMod,
                              unsigned char StorageMod);

/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  功能:	
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WritedAddr,unsigned char *WritedBuffer,
                                  unsigned int WriteLent, unsigned char StorageMod);
#endif