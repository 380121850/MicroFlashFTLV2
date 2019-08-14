//************************************************************************
//
//  Lierda, Inc                      SST25VF010 
//            
//************************************************************************
#include "msp430x54x.h"
#include "SPI_Module.h"
#include "SST25VF010_Module.H"

/******************************************************************************
  函数(模块)名称:unsigned char ReadStatus(void)
  功能:	读状态寄存器
  输入参数:无         	              		
  输出参数: 状态寄存器         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadStatus(void)
{
    unsigned char StatusTemp;
    SPI_CE_L;
    SPI_Send_Byte(RDSR_Command);//发送命令
    //__delay_cycles(10);                 // Delay;
    StatusTemp = SPI_Recei_Byte();
    SPI_CE_H;
    return StatusTemp;
}


/******************************************************************************
  函数(模块)名称:void StatusWrite(unsigned char StatusData)
  功能:	状态寄存器写
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 修改当前寄存器
*******************************************************************************/
void StatusWrite(unsigned char StatusData)
{
    //寄存器写使能
    SPI_CE_L;
    SPI_Send_Byte(EWSR_Command);//发送命令
    SPI_CE_H;
    //__delay_cycles(50);                 // Delay;
    SPI_CE_L;
    SPI_Send_Byte(WRSR_Command);//发送命令
    SPI_Send_Byte(StatusData);  //发送数据
    SPI_CE_H;
}

/******************************************************************************
  函数(模块)名称:void WriteEnable(void)
  功能:	写使能函数
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 执行后，FLASH可写
*******************************************************************************/
void WriteEnable(void)
{
    SPI_CE_L;
    SPI_Send_Byte(WREN_Command);//发送命令
    SPI_CE_H;
}
/******************************************************************************
  函数(模块)名称:void WriteDisable(void)
  功能:	写禁止函数
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 执行后，FLASH禁止写
*******************************************************************************/
void WriteDisable(void)
{
    SPI_CE_L;
    SPI_Send_Byte(WRDI_Command);//发送命令
    SPI_CE_H;
}

/******************************************************************************
  函数(模块)名称:void SST25VF010_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void SST25VF010_Init(void)
{
    SPI_CE_H;
    SPI_HOLD_H;
    SPI_WP_H;
    
    SPI_CE_OUT;
    SPI_HOLD_OUT;
    SPI_WP_OUT;
    SPI_PowerOut;
    SPI_PowerOn;
    __delay_cycles(1000);                 // Delay;
    SPI_Init();
    StatusWrite(0X00);
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadID(unsigned char Manu_or_Device)
  功能:	读设备标识符
  输入参数:需要读取的标识答         	              		
  输出参数: 设备标识符        	 		   		 
  其它说明: 0，读取设备厂商标志 BF (FOR SST)，1，读取设备标识答 13 （FOR SST29SF040）
*******************************************************************************/
unsigned char ReadID_SST25(unsigned char *ReadBuffer)
{
    SPI_CE_L;
    SPI_Send_Byte(ReadID_Command);//发送命令
    SPI_Send_Byte(0X00);
    SPI_Send_Byte(0X00);
    SPI_Send_Byte(0X01);
    SPI_Recei_Mass(ReadBuffer,2);
    SPI_CE_H;
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadByte_SST25(unsigned long ReadAddr)
  功能:	FLASH读函数
  输入参数:         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadByte_SST25(unsigned long ReadAddr)
{
    unsigned char Addr0,Addr1,Addr2,ReadData;
    Addr0=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr1=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr2=(unsigned char)ReadAddr;
    
    SPI_CE_L;
    SPI_Send_Byte(Read_Command);//发送读命令
    SPI_Send_Byte(Addr2);       //发送读地址
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    ReadData = SPI_Recei_Byte(); //接收数据
    SPI_CE_H;
    return ReadData;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadData(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
  功能:	FLASH读函数
  输入参数:读起始地址，缓冲区地址，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char MultiByteRead_SST25(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
{
    unsigned char Addr0,Addr1,Addr2;
    Addr0=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr1=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr2=(unsigned char)ReadAddr;
    
    SPI_CE_L;
    SPI_Send_Byte(Read_Command);//发送读命令
    SPI_Send_Byte(Addr2);       //发送读地址
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_Recei_Mass(ReadBuffer,ReadLent); //接收数据
    SPI_CE_H;
    return 1;
}


/******************************************************************************
  函数(模块)名称:void ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData)
  功能:	FLASH字节写函数
  输入参数:写地址，写数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData)
{
    unsigned char Addr0,Addr1,Addr2;
    Addr0=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr1=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr2=(unsigned char)WritedAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(ByteProgram_Command);//发送命令
    SPI_Send_Byte(Addr2);              //发送地址
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_Send_Byte(WriteData);          //发送数据
    SPI_CE_H;
    //WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
    
}


/******************************************************************************
  函数(模块)名称:unsigned char MultiByteWrite(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent)
  功能:	FLASH字节写函数
  输入参数:写地址，写数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char MultiByteWrite_SST25(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent)
{
    unsigned char Addr0,Addr1,Addr2;
    Addr0=(unsigned char)WriteAddr;
    WriteAddr=WriteAddr>>8;
    
    Addr1=(unsigned char)WriteAddr;
    WriteAddr=WriteAddr>>8;
    
    Addr2=(unsigned char)WriteAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(AAIProgram_Command);  //发送命令
    SPI_Send_Byte(Addr2);               //发送地址
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_Send_Byte(*WriteData++);        //发送数据
    SPI_CE_H;
    //__delay_cycles(50);                 // Delay;
    while(ReadStatus()&Busy_BIT);
    for(unsigned int i=0;i<WriteLent-1;i++)
    {
        SPI_CE_L;
        SPI_Send_Byte(AAIProgram_Command);    //发送命令
        SPI_Send_Byte(*WriteData++);          //发送数据
        SPI_CE_H;
        while(ReadStatus()&Busy_BIT);
    }
    WriteDisable();
    while(ReadStatus()&WEL_BIT);
    return 1;
}


/******************************************************************************
  函数(模块)名称:unsigned char SectorErase_SST25(unsigned int SectorAddr)
  功能:	FLASH扇区擦除函数
  输入参数:扇区地址，从0开始         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SectorErase_SST25(unsigned int SectorAddr)
{
    unsigned char Addr0,Addr1,Addr2;
    unsigned long WritedAddr;
    WritedAddr = (unsigned long)SectorAddr;
    WritedAddr=WritedAddr<<12;
    
    Addr0=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr1=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr2=(unsigned char)WritedAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(SectorErase_Command);//发送命令
    SPI_Send_Byte(Addr2);              //发送地址
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_CE_H;
    WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
    
}


/******************************************************************************
  函数(模块)名称:unsigned char SectorErase(unsigned int SectorAddr)
  功能:	FLASH块擦除函数
  输入参数:FLASH块地址，从0开始         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char BlockErase_SST25(unsigned int BlockAddr)
{
    unsigned char Addr0,Addr1,Addr2;
    unsigned long WritedAddr;
    WritedAddr = (unsigned long)BlockAddr;
    WritedAddr=WritedAddr<<15;
    
    Addr0=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr1=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr2=(unsigned char)WritedAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(BlorkErase_Command);//发送命令
    SPI_Send_Byte(Addr2);              //发送地址
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_CE_H;
    WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char ChipErase(void)
  功能:	FLASH芯片全部擦除函数
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ChipErase_SST25(void)
{
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(ChipErase_Command);//发送命令
    SPI_CE_H;
    WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
}


