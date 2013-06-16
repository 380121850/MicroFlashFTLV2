#ifndef  __FlashStorage_H
#define  __FlashStorage_H

#include ".\SST25VF_Module\SST25VF010_Module.H"
#include ".\SST29VF_Module\SST29VF_Module.H"


//#define  FlashModule_ture   1

/******************************************************************************
  函数(模块)名称:void FlashModule_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_Init  SST29VF_Init
#else
#define FlashModule_Init  SST25VF010_Init
#endif


/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_ReadInfo(unsigned char *ReadBuffer)
  功能:	FLASH读芯片信息函数
  输入参数:读数据缓冲区指针        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 默认2字节，第一字节为芯片厂商标识，第二字节为芯片ID
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_ReadInfo  ReadID_SST29
#else
#define FlashModule_ReadInfo  ReadID_SST25
#endif


/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_ReadByte(unsigned long ReadAddr)
  功能:	FLASH单字节读函数
  输入参数:读地址        	              		
  输出参数:读数据         	 		   		 
  其它说明: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_ReadByte  ReadByte_SST29
#else
#define FlashModule_ReadByte  ReadByte_SST25
#endif


/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
  功能:	FLASH单字节写函数
  输入参数:写地址，写数据        	              		
  输出参数:         	 		   		 
  其它说明: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_WriteByte  ByteWrite_SST29
#else
#define FlashModule_WriteByte  ByteWrite_SST25
#endif



/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
  功能:	FLASH多字节读函数
  输入参数:读地址，数据缓冲区指针，数据长度        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大4095个字节
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_MultiByteRead  MultiByteRead_SST29
#else
#define FlashModule_MultiByteRead  MultiByteRead_SST25
#endif



/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
  功能:	FLASH多字节写函数
  输入参数:写地址，写数据缓冲区指针，数据长度        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大4095个字节
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_MultiByteWrite  MultiByteWrite_SST29
#else
#define FlashModule_MultiByteWrite  MultiByteWrite_SST25
#endif



/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_SectorErase(unsigned int SectorAddr)
  功能:	FLASH扇区擦除函数
  输入参数:扇区地址        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大65536块扇区
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_SectorErase  SectorErase_SST29
#else
#define FlashModule_SectorErase  SectorErase_SST25
#endif

/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_BlockErase(unsigned int BlockAddr)
  功能:	FLASH芯片块擦除函数
  输入参数:块地址        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_BlockErase 
#else
#define FlashModule_BlockErase  BlockErase_SST25
#endif


/******************************************************************************
  函数(模块)名称:unsigned char FlashModule_ChipErase(void)
  功能:	FLASH芯片擦除函数
  输入参数:空        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_ChipErase ChipErase_SST29 
#else
#define FlashModule_ChipErase  ChipErase_SST25
#endif

unsigned char FlashTest(unsigned long FlashMaxSize);

#endif