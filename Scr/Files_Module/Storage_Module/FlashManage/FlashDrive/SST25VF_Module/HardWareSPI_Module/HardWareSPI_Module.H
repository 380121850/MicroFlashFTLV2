#ifndef  __HardWareSPI_Module_H
#define  __HardWareSPI_Module_H


/******************************************************************************
  函数(模块)名称:void SPI_Init(void)
  功能:	
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void HardWareSPI_Init(void);

/******************************************************************************
  函数(模块)名称:void SPI_Send_Byte(unsigned char Send_Data)
  功能:	SPI单字节发送函数
  输入参数:需要发送的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
unsigned char  HardWareSPI_Send_Byte(unsigned char Send_Data);

/******************************************************************************
  函数(模块)名称:void SPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节发送函数
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void HardWareSPI_Send_Mass(unsigned char *Data_point,unsigned int Data_Lent);

/******************************************************************************
  函数(模块)名称:unsigned char SPI_Recei_Byte(void)
  功能:	SPI单字节接收函数
  输入参数:无         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
unsigned char HardWareSPI_Recei_Byte(void);


/******************************************************************************
  函数(模块)名称:void SPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节接收函数
  输入参数:存放的地址指针，以及需要接收的数据长度         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
void HardWareSPI_Recei_Mass(unsigned char *Data_point,unsigned int Data_Lent);

/******************************************************************************
  函数(模块)名称:void SPI_Send_Mass_Ext(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节发送函数，使用中断方式
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void HardWareSPI_Send_Mass_Ext(unsigned char *Data_point,unsigned int Data_Lent);

/******************************************************************************
  函数(模块)名称:void SPI_Recei_Mass_Ext(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节接收函数，使用中断方式
  输入参数:缓冲区指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void HardWareSPI_Recei_Mass_Ext(unsigned char *Data_point,unsigned int Data_Lent);

#endif