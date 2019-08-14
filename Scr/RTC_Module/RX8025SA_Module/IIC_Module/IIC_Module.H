#ifndef  __IIC_Module_H
#define  __IIC_Module_H


#define define_try_time     3   //当产生错误时，尝试次数。防止进入死循环

#define IIC_SCL_BIT   BIT2    //要求  1、SCL至少为输出IO口
#define IIC_SDA_BIT   BIT1    //      2、SDA为输出输入IO口
#define IIC_DIR_Port  P10DIR  //      3、若使用电源控制，则VCC至少为输出IO口
#define IIC_OUT_Port  P10OUT
#define IIC_IN_Port   P10IN

#define IIC_SCL_DIR_IN   IIC_DIR_Port &= ~IIC_SCL_BIT;
#define IIC_SCL_DIR_OUT  IIC_DIR_Port |= IIC_SCL_BIT;

#define IIC_SDA_DIR_IN   IIC_DIR_Port &= ~IIC_SDA_BIT;
#define IIC_SDA_DIR_OUT  IIC_DIR_Port |= IIC_SDA_BIT;

#define IIC_SCL_H     IIC_OUT_Port |= IIC_SCL_BIT;
#define IIC_SCL_L     IIC_OUT_Port &= ~IIC_SCL_BIT;

#define IIC_SDA_H     IIC_OUT_Port |= IIC_SDA_BIT;
#define IIC_SDA_L     IIC_OUT_Port &= ~IIC_SDA_BIT;

static unsigned char Slave_Addres;

/******************************************************************************
  函数(模块)名称:void IIC_Init_Port(void)
  功能:	IIC启动函数
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 在函数里面启动了供电
*******************************************************************************/
void IIC_Init(unsigned char Slave_Addr);

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  功能:	单次读/随机读
  输入参数: 要读的数据地址，输出数据保存的字节指针        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char IIC_Single_Read(unsigned char R_addr,unsigned char *R_word);

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  功能:	连续读
  输入参数: 读起始地址，数据保存指针，读长度        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char IIC_Multi_read(unsigned char R_addr,unsigned char *Save_addr,
                                unsigned int R_lent);

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Single_Write(unsigned char W_addr,unsigned char Date_byte)
  功能:	IIC总线单字节写
  输入参数: 要写的地址，要写的数据        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char IIC_Single_Write(unsigned char W_addr,unsigned char Date_byte);

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Page_Write(unsigned char W_addr,
                                              unsigned char *WriteData_Addr,
                                              unsigned char WriteData_Cnt)
  功能:	IIC页写
  输入参数: 要写的地址，要写的数据指针，要写的字节数        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 要写的字节数，不能大于当前写字节地址到当前页最大地址之差。
*******************************************************************************/
unsigned char IIC_Page_Write(unsigned char W_addr,unsigned char *WriteData_Addr,
                             unsigned char WriteData_Cnt);

#endif