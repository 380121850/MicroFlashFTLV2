//************************************************************************
//
//  向上提供统一接口API
//            
//************************************************************************
#include".\FlashManage\FlashManage.h"
#include "StorageManage.H"

#define StorageManage_Select  1

/******************************************************************************
  函数(模块)名称:void Storage_Module_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void Storage_Module_Init(void)
{
#if StorageManage_Select == 0
    ;
#elif StorageManage_Select == 1
    FlashManage_Init();
#endif
}


/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer)
  功能:	
  输入参数: 无         	              		
  输出参数: 无         	 		   		 
  其它说明: 缓冲区映射FLASH存储某个区，地址递增
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer)
{
#if StorageManage_Select == 0
    ;
#elif StorageManage_Select == 1
    return LogicFlashRead( ReadAddr, DataLent, DataBuffer);
#endif
    
}


/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer)
  功能:	
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer)
{
#if StorageManage_Select == 0
    ;
#elif StorageManage_Select == 1
    return LogicFlashWrite( WriteAddr, DataLent, DataBuffer);
#endif
   
}

