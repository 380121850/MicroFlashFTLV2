#ifndef  __SoftWareSPI_Module_H
#define  __SoftWareSPI_Module_H

//通讯接口 SI,SO,SCK
#define SoftWareSPI_SI_BIT     BIT1    //要求  1、CE至少为输出IO口
#define SoftWareSPI_SO_BIT     BIT2    //      2、HOLD为输出IO口
#define SoftWareSPI_SCK_BIT    BIT3    //      2、WP为输出IO口

//接口寄存器
#define SoftWareSPI_DIR_Port  P3DIR    //      3、若使用电源控制，则VCC至少为输出IO口
#define SoftWareSPI_OUT_Port  P3OUT
#define SoftWareSPI_IN_Port   P3IN

#define SoftWareSPI_SI_OUT   SoftWareSPI_DIR_Port |=SoftWareSPI_SI_BIT;
#define SoftWareSPI_SI_IN    SoftWareSPI_DIR_Port &=~SoftWareSPI_SI_BIT;
#define SoftWareSPI_SI_H     SoftWareSPI_OUT_Port |= SoftWareSPI_SI_BIT;
#define SoftWareSPI_SI_L     SoftWareSPI_OUT_Port &=~SoftWareSPI_SI_BIT;

#define SoftWareSPI_SO_OUT   SoftWareSPI_DIR_Port |=SoftWareSPI_SO_BIT;
#define SoftWareSPI_SO_IN    SoftWareSPI_DIR_Port &=~SoftWareSPI_SO_BIT;
#define SoftWareSPI_SO_Read  (SoftWareSPI_IN_Port & SoftWareSPI_SO_BIT)
#define SoftWareSPI_SO_H     SoftWareSPI_OUT_Port |= SoftWareSPI_SO_BIT;
#define SoftWareSPI_SO_L     SoftWareSPI_OUT_Port &=~SoftWareSPI_SO_BIT;

#define SoftWareSPI_SCK_OUT   SoftWareSPI_DIR_Port |=SoftWareSPI_SCK_BIT;
#define SoftWareSPI_SCK_IN    SoftWareSPI_DIR_Port &=~SoftWareSPI_SCK_BIT;
#define SoftWareSPI_SCK_H     SoftWareSPI_OUT_Port |= SoftWareSPI_SCK_BIT;
#define SoftWareSPI_SCK_L     SoftWareSPI_OUT_Port &=~SoftWareSPI_SCK_BIT;


/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Init(void)
  功能:	SPI总线发送一个字节数据
  输入参数:发送的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
void SoftWareSPI_Init(void);

/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Send_Byte(unsigned char Send_Data)
  功能:	SPI总线发送一个字节数据
  输入参数:发送的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
void SoftWareSPI_Send_Byte(unsigned char Send_Data);

/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节发送函数
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned int Data_Lent);

/******************************************************************************
  函数(模块)名称:unsigned char SPI_Recei_Byte(void)
  功能:	SPI总线接收一个字节数据
  输入参数:  无        	              		
  输出参数:  接收到的数据       	 		   		 
  其它说明: 
*******************************************************************************/ 
unsigned char SoftWareSPI_Recei_Byte(void);

/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节接收函数
  输入参数:存放的地址指针，以及需要接收的数据长度         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned int Data_Lent);


#endif