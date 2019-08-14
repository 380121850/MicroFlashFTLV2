#ifndef  __SST25VF010_Module_H
#define  __SST25VF010_Module_H

//芯片控制接口
#define SPI_CE_BIT     BIT0    //要求  1、CE至少为输出IO口
#define SPI_HOLD_BIT   BIT6    //      2、HOLD为输出IO口
#define SPI_WP_BIT     BIT7    //      2、WP为输出IO口

#define SPI_DIR_Port  P3DIR  //      3、若使用电源控制，则VCC至少为输出IO口
#define SPI_OUT_Port  P3OUT
#define SPI_IN_Port   P3IN

//电源
#define SPI_PowerOut   P10DIR |=BIT7;
#define SPI_PowerOn    P10OUT &=~BIT7;
#define SPI_PowerOff   P10OUT |=BIT7;

#define SPI_CE_OUT   SPI_DIR_Port |=SPI_CE_BIT;
#define SPI_CE_H     SPI_OUT_Port |= SPI_CE_BIT;
#define SPI_CE_L     SPI_OUT_Port &=~SPI_CE_BIT;

#define SPI_HOLD_OUT   SPI_DIR_Port |=SPI_HOLD_BIT;
#define SPI_HOLD_H     SPI_OUT_Port |= SPI_HOLD_BIT;
#define SPI_HOLD_L     SPI_OUT_Port &=~SPI_HOLD_BIT;

#define SPI_WP_OUT   SPI_DIR_Port |=SPI_WP_BIT;
#define SPI_WP_H     SPI_OUT_Port |= SPI_WP_BIT;
#define SPI_WP_L     SPI_OUT_Port &=~SPI_WP_BIT;

#define Read_Command            0x03
#define SectorErase_Command     0x20
#define BlorkErase_Command      0x52
#define ChipErase_Command       0x60
#define ByteProgram_Command     0x02
#define AAIProgram_Command      0xAF
#define RDSR_Command            0x05
#define EWSR_Command            0x50
#define WRSR_Command            0x01
#define WREN_Command            0x06
#define WRDI_Command            0x04
#define ReadID_Command          0x90

#define Busy_BIT  0X01
#define WEL_BIT   0X02
#define BP0_BIT   0X04
#define BP1_BIT   0X08

#define AAI_BIT   0X40
#define BPL_BIT   0X80


/******************************************************************************
  函数(模块)名称:void SST25VF010_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void SST25VF010_Init(void);

/******************************************************************************
  函数(模块)名称:unsigned char ReadID(unsigned char Manu_or_Device)
  功能:	读设备标识符
  输入参数:需要读取的标识答         	              		
  输出参数: 设备标识符        	 		   		 
  其它说明: 0，读取设备厂商标志 BF (FOR SST)，1，读取设备标识答 13 （FOR SST29SF040）
*******************************************************************************/
unsigned char ReadID_SST25(unsigned char *ReadBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char ReadByte_SST25(unsigned long ReadAddr)
  功能:	FLASH读函数
  输入参数:         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadByte_SST25(unsigned long ReadAddr);

/******************************************************************************
  函数(模块)名称:void ReadData(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
  功能:	FLASH读函数
  输入参数:读起始地址，缓冲区地址，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char MultiByteRead_SST25(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent);

/******************************************************************************
  函数(模块)名称:void ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData)
  功能:	FLASH字节写函数
  输入参数:写地址，写数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData);

/******************************************************************************
  函数(模块)名称:unsigned char MultiByteWrite(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent)
  功能:	FLASH字节写函数
  输入参数:写地址，写数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char MultiByteWrite_SST25(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent);

/******************************************************************************
  函数(模块)名称:unsigned char SectorErase_SST25(unsigned int SectorAddr)
  功能:	FLASH扇区擦除函数
  输入参数:扇区地址，从0开始         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SectorErase_SST25(unsigned int SectorAddr);

/******************************************************************************
  函数(模块)名称:unsigned char SectorErase(unsigned int SectorAddr)
  功能:	FLASH块擦除函数
  输入参数:FLASH块地址，从0开始         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char BlockErase_SST25(unsigned int BlockAddr);

/******************************************************************************
  函数(模块)名称:unsigned char ChipErase(void)
  功能:	FLASH芯片全部擦除函数
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ChipErase_SST25(void);

#endif