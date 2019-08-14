#ifndef  __SST29VF_Module_H
#define  __SST29VF_Module_H

#define Parallel_CE_BIT     BIT2    //要求  1、CE至少为输出IO口
#define Parallel_OE_BIT     BIT1    //      2、HOLD为输出IO口
#define Parallel_WE_BIT     BIT0    //      2、WP为输出IO口

#define Parallel_DIR_Port  P11DIR  //      3、若使用电源控制，则VCC至少为输出IO口
#define Parallel_OUT_Port  P11OUT
#define Parallel_IN_Port   P11IN
#define Parallel_SELL      P11SEL

#define Parallel_CTL_Port Parallel_SELL &= ~(Parallel_CE_BIT+Parallel_OE_BIT+Parallel_WE_BIT);

#define Parallel_CE_OUT   Parallel_DIR_Port |= Parallel_CE_BIT;
#define Parallel_CE_H     Parallel_OUT_Port |= Parallel_CE_BIT;
#define Parallel_CE_L     Parallel_OUT_Port &=~Parallel_CE_BIT;

#define Parallel_OE_OUT   Parallel_DIR_Port |= Parallel_OE_BIT;
#define Parallel_OE_H     Parallel_OUT_Port |= Parallel_OE_BIT;
#define Parallel_OE_L     Parallel_OUT_Port &=~Parallel_OE_BIT;

#define Parallel_WE_OUT   Parallel_DIR_Port |= Parallel_WE_BIT;
#define Parallel_WE_H     Parallel_OUT_Port |= Parallel_WE_BIT;
#define Parallel_WE_L     Parallel_OUT_Port &=~Parallel_WE_BIT;

void SST29VF_Init(void);

unsigned char ReadID_SST29(unsigned char *ReadBuffer);

unsigned char ReadByte_SST29(unsigned long ReadAddr);

unsigned char MultiByteRead_SST29(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent);

unsigned char ByteWrite_SST29(unsigned long WriteAddr,unsigned char WriteByte);
/******************************************************************************
  函数(模块)名称:unsigned char ReadID(unsigned char Manu_or_Device)
  功能:	读设备标识符
  输入参数:需要读取的标识答         	              		
  输出参数: 设备标识符        	 		   		 
  其它说明: 0，读取设备厂商标志 BF (FOR SST)，1，读取设备标识答 13 （FOR SST29SF040）
*******************************************************************************/
unsigned char MultiByteWrite_SST29(unsigned long WriteAddr,unsigned char *WriteBuffer,unsigned int Lent);
unsigned char SectorErase_SST29(unsigned int SectorAddr);
unsigned char BlockErase_SST29( unsigned int BlockAddr);
unsigned char ChipErase_SST29(void);
#endif