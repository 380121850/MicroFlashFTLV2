#ifndef  __SPI_Module_H
#define  __SPI_Module_H

//#define  SoftWareSPI_Disable   1
#include ".\SoftWareSPI_Module\SoftWareSPI_Module.H"
#include ".\HardWareSPI_Module\HardWareSPI_Module.H"

/******************************************************************************
  函数(模块)名称:void SPI_Init(void)
  功能:	
  输入参数:         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
#ifdef  SoftWareSPI_Disable
#define SPI_Init  SoftWareSPI_Init
#else
#define SPI_Init  HardWareSPI_Init
#endif

/******************************************************************************
  函数(模块)名称:void SPI_Send_Byte(unsigned char Send_Data)
  功能:	SPI总线发送一个字节数据
  输入参数:发送的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
#ifdef  SoftWareSPI_Disable
#define SPI_Send_Byte  SoftWareSPI_Send_Byte
#else
#define SPI_Send_Byte  HardWareSPI_Send_Byte
#endif

/******************************************************************************
  函数(模块)名称:void SPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节发送函数
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
#ifdef  SoftWareSPI_Disable
#define SPI_Send_Mass  SoftWareSPI_Send_Mass
#else
#define SPI_Send_Mass  HardWareSPI_Send_Mass
#endif

/******************************************************************************
  函数(模块)名称:unsigned char SPI_Recei_Byte(void)
  功能:	SPI总线接收一个字节数据
  输入参数:  无        	              		
  输出参数:  接收到的数据       	 		   		 
  其它说明: 
*******************************************************************************/ 
#ifdef  SoftWareSPI_Disable
#define SPI_Recei_Byte  SoftWareSPI_Recei_Byte
#else
#define SPI_Recei_Byte  HardWareSPI_Recei_Byte
#endif


/******************************************************************************
  函数(模块)名称:void SPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节接收函数
  输入参数:存放的地址指针，以及需要接收的数据长度         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
#ifdef  SoftWareSPI_Disable
#define SPI_Recei_Mass  SoftWareSPI_Recei_Mass
#else
#define SPI_Recei_Mass  HardWareSPI_Recei_Mass
#endif

#endif