#ifndef  __StorageManage_H
#define  __StorageManage_H


/******************************************************************************
  函数(模块)名称:void Storage_Module_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void Storage_Module_Init(void);

/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer)
  功能:	
  输入参数: 无         	              		
  输出参数: 无         	 		   		 
  其它说明: 缓冲区映射FLASH存储某个区，地址递增
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer)
  功能:	
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer);
#endif