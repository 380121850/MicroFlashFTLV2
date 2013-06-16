#ifndef  __IIC_Module_H
#define  __IIC_Module_H

//#define  SoftWareIIC_Disable   1

#include ".\HardWareIIC_Module\HardWareIIC_Module.H"
#include ".\SoftWareIIC_Module\SoftWareIIC_Module.H"

/******************************************************************************
  函数(模块)名称:void IIC_Init(void)
  功能:	
  输入参数:         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
#ifdef  SoftWareIIC_Disable
#define IIC_Init  SoftWareIIC_Init
#else
#define IIC_Init  HardWareIIC_Init
#endif

/******************************************************************************
  函数(模块)名称:void IIC_Send_Byte(unsigned char Send_Data)
  功能:	SPI总线发送一个字节数据
  输入参数:发送的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
#ifdef  SoftWareIIC_Disable
#define IIC_SingleWrite  SoftWareIIC_Single_Write
#else
#define IIC_SingleWrite  HardWareIIC_SingleWrite
#endif

/******************************************************************************
  函数(模块)名称:void IIC_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节发送函数
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
#ifdef  SoftWareIIC_Disable
#define IIC_SingleRead  SoftWareIIC_Single_Read
#else
#define IIC_SingleRead  HardWareIIC_SingleRead
#endif

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Recei_Byte(void)
  功能:	SPI总线接收一个字节数据
  输入参数:  无        	              		
  输出参数:  接收到的数据       	 		   		 
  其它说明: 
*******************************************************************************/ 
#ifdef  SoftWareIIC_Disable
#define IIC_MultiRead  SoftWareIIC_Multi_read
#else
#define IIC_MultiRead  HardWareIIC_MultiRead
#endif


/******************************************************************************
  函数(模块)名称:void IIC_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节接收函数
  输入参数:存放的地址指针，以及需要接收的数据长度         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
#ifdef  SoftWareIIC_Disable
#define IIC_MultiWrite  SoftWareIIC_Page_Write
#else
#define IIC_MultiWrite  HardWareIIC_MultiWrite
#endif

#endif