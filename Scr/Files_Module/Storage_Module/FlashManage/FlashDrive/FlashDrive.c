//************************************************************************
//
//                     FlashDrive 
//            
//************************************************************************
//************************************************************************
//  1、提供
//                    
//            
//************************************************************************
#include ".\SST25VF_Module\SST25VF010_Module.H"
#include ".\SST29VF_Module\SST29VF_Module.H"
#include "FlashDrive.H"

#define FlashDrive_Select  1

/******************************************************************************
  函数(模块)名称:void FlashDrive_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void FlashDrive_Init(void)
{
#if FlashDrive_Select == 0
    SST29VF_Init();
#elif FlashDrive_Select == 1
    SST25VF010_Init();
#endif
}

/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer)
  功能:	FLASH读芯片信息函数
  输入参数:读数据缓冲区指针        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 默认2字节，第一字节为芯片厂商标识，第二字节为芯片ID
*******************************************************************************/
unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer)
{   
#if FlashDrive_Select == 0
    return ReadID_SST29(ReadBuffer);
#elif FlashDrive_Select == 1
    return ReadID_SST25(ReadBuffer);
#endif
}


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_ReadByte(unsigned long ReadAddr)
  功能:	FLASH单字节读函数
  输入参数:读地址        	              		
  输出参数:读数据         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char FlashDrive_ReadByte(unsigned long ReadAddr)
{ 
#if FlashDrive_Select == 0
    return ReadByte_SST29(ReadAddr);
#elif FlashDrive_Select == 1
    return ReadByte_SST25(ReadAddr);
#endif    
}


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
  功能:	FLASH单字节写函数
  输入参数:写地址，写数据        	              		
  输出参数:         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
{
#if FlashDrive_Select == 0
    return ByteWrite_SST29( WriteAddr,WriteByte);
#elif FlashDrive_Select == 1
    return ByteWrite_SST25( WriteAddr,WriteByte);
#endif
}



/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
  功能:	FLASH多字节读函数
  输入参数:读地址，数据缓冲区指针，数据长度        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大4095个字节
*******************************************************************************/
unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
{  
#if FlashDrive_Select == 0
    return MultiByteRead_SST29( ReadAddr, ReadBuffer, ReadLent);
#elif FlashDrive_Select == 1
    return MultiByteRead_SST25( ReadAddr, ReadBuffer, ReadLent);
#endif
}


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
  功能:	FLASH多字节写函数
  输入参数:写地址，写数据缓冲区指针，数据长度        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大4095个字节
*******************************************************************************/
unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
{   
#if FlashDrive_Select == 0
    return MultiByteWrite_SST29( WriteAddr, WriteBuffer, Lent);
#elif FlashDrive_Select == 1
    return MultiByteWrite_SST25( WriteAddr, WriteBuffer, Lent);
#endif
}



/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_SectorErase(unsigned int SectorAddr)
  功能:	FLASH扇区擦除函数
  输入参数:扇区地址        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大65536块扇区
*******************************************************************************/
unsigned char FlashDrive_SectorErase(unsigned int SectorAddr)
{
#if FlashDrive_Select == 0
    return SectorErase_SST29( SectorAddr);
#elif FlashDrive_Select == 1
    return SectorErase_SST25( SectorAddr);
#endif
}
  


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_BlockErase(unsigned int BlockAddr)
  功能:	FLASH芯片块擦除函数
  输入参数:块地址        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char FlashDrive_BlockErase(unsigned int BlockAddr)
{
#if FlashDrive_Select == 0
     return BlockErase_SST29( BlockAddr);
#elif FlashDrive_Select == 1
     return BlockErase_SST25( BlockAddr);
#endif
}


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_ChipErase(void)
  功能:	FLASH芯片擦除函数
  输入参数:空        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char FlashDrive_ChipErase(void)
{
#if FlashDrive_Select == 0
    return ChipErase_SST29();
#elif FlashDrive_Select == 1
    return ChipErase_SST25();
#endif
}