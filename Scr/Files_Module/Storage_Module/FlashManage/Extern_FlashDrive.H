#ifndef  __Extern_FlashDrive_H
#define  __Extern_FlashDrive_H


/******************************************************************************
  函数(模块)名称:void FlashDrive_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
extern void FlashDrive_Init(void);


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer)
  功能:	FLASH读芯片信息函数
  输入参数:读数据缓冲区指针        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 默认2字节，第一字节为芯片厂商标识，第二字节为芯片ID
*******************************************************************************/
extern unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer);


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_ReadByte(unsigned long ReadAddr)
  功能:	FLASH单字节读函数
  输入参数:读地址        	              		
  输出参数:读数据         	 		   		 
  其它说明: 
*******************************************************************************/
extern unsigned char FlashDrive_ReadByte(unsigned long ReadAddr);


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
  功能:	FLASH单字节写函数
  输入参数:写地址，写数据        	              		
  输出参数:         	 		   		 
  其它说明: 
*******************************************************************************/
extern unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte);



/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
  功能:	FLASH多字节读函数
  输入参数:读地址，数据缓冲区指针，数据长度        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大4095个字节
*******************************************************************************/
extern unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent);


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
  功能:	FLASH多字节写函数
  输入参数:写地址，写数据缓冲区指针，数据长度        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大4095个字节
*******************************************************************************/
extern unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent);



/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_SectorErase(unsigned int SectorAddr)
  功能:	FLASH扇区擦除函数
  输入参数:扇区地址        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 最大65536块扇区
*******************************************************************************/
extern unsigned char FlashDrive_SectorErase(unsigned int SectorAddr);


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_BlockErase(unsigned int BlockAddr)
  功能:	FLASH芯片块擦除函数
  输入参数:块地址        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 
*******************************************************************************/
extern unsigned char FlashDrive_BlockErase(unsigned int BlockAddr);


/******************************************************************************
  函数(模块)名称:unsigned char FlashDrive_ChipErase(void)
  功能:	FLASH芯片擦除函数
  输入参数:空        	              		
  输出参数: 错误代码        	 		   		 
  其它说明: 
*******************************************************************************/
extern unsigned char FlashDrive_ChipErase(void);

#endif