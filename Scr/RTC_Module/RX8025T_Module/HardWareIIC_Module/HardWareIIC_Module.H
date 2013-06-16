#ifndef  __HardWareIIC_Module_H
#define  __HardWareIIC_Module_H


#define HardWaredefine_try_time     128   //当产生错误时，尝试次数。防止进入死循环

/******************************************************************************
  函数(模块)名称:void HardWareIIC_Init(unsigned char Slave_Addr)
  功能:	IIC初始化函数，可以设置从机地址
  输入参数: 从机地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 传入的从机地址，需要右移一位再放入地址寄存器
*******************************************************************************/
void HardWareIIC_Init(unsigned char Slave_Addr);

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  功能:	单次读/随机读
  输入参数: 要读的数据地址，输出数据保存的字节指针        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_SingleRead(unsigned char ReadAddr,unsigned char *ReadData);

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  功能:	连续读
  输入参数: 读起始地址，数据保存指针，读长度        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_MultiRead(unsigned char ReadAddr,unsigned char *ReadBuff,
                                unsigned int ReadLent);

/******************************************************************************
  函数(模块)名称:unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,
                                                       unsigned char WriteData)
  功能:	IIC总线单字节写
  输入参数: 要写的地址，要写的数据        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,unsigned char WriteData);

/******************************************************************************
  函数(模块)名称:unsigned char HardWareIIC_MultiWrite(unsigned char WriteAddr,
                                     unsigned char *WriteBuffer,
                                     unsigned char DataLent)
  功能:	多字节写函数
  输入参数: 要写的地址，要写的数据指针，要写的字节数        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_MultiWrite(unsigned char WriteAddr,
                                     unsigned char *WriteBuffer,
                                     unsigned char DataLent);

#endif