//************************************************************************
//
//  Lierda, Inc                      SST25VF010 
//            
//************************************************************************
#include "msp430x54x.h"
#include "Parallel_Module.h"
#include "SST29VF_Module.H"

/******************************************************************************
  函数(模块)名称:void SST29VF_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void SST29VF_Init(void)
{
    Parallel_CTL_Port; //控制口配置为普通IO口
    
    Parallel_CE_H;
    Parallel_CE_OUT;
    
    Parallel_OE_H;
    Parallel_OE_OUT;
    
    Parallel_WE_H;
    Parallel_WE_OUT;
    
    Parallel_Init();
}

/******************************************************************************
  函数(模块)名称:void ParallWritData(unsigned long WriteAddr,unsigned char WriteByte)
  功能:	向地址写入一个字节
  输入参数:要写入的地址，要写的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 以并行总线的方式，配合有写使能位。
*******************************************************************************/
void ParallWritData(unsigned long WriteAddr,unsigned char WriteByte)
{
    WrteAddrPort(WriteAddr);  
    Parallel_WE_L;
    WrteDataPort(WriteByte);    
    __delay_cycles(3);                 // Delay;
    Parallel_WE_H;
}

/******************************************************************************
  函数(模块)名称:unsigned char ParallReadData(unsigned long ReadAddr)
  功能:	从地址读出一个字节
  输入参数:要读取的地址         	              		
  输出参数: 读出来的数据        	 		   		 
  其它说明: 以并行总线的方式。
*******************************************************************************/
unsigned char ParallReadData(unsigned long ReadAddr)
{
    unsigned char ReadData=0;
    WrteAddrPort(ReadAddr);
    __delay_cycles(1);                 // Delay;
    Parallel_OE_L;
    __delay_cycles(1);                 // Delay;
    ReadData = ReadDataPort();
    Parallel_OE_H;
    return ReadData;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadID(unsigned char Manu_or_Device)
  功能:	读设备标识符
  输入参数:需要读取的标识答         	              		
  输出参数: 设备标识符        	 		   		 
  其它说明: 0，读取设备厂商标志 BF (FOR SST)，1，读取设备标识答 13 （FOR SST29SF040）
*******************************************************************************/
unsigned char ReadID_SST29(unsigned char *ReadBuffer)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //进入ID模式
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X90);
    __delay_cycles(5);                 // Delay;
    
    *ReadBuffer++ = ParallReadData(0X0000);       //读取设备厂商标识和ID号
    *ReadBuffer = ParallReadData(0X0001);
    
    ParallWritData(0X555,0XAA);             //退出ID模式
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0XF0);
    __delay_cycles(5);                 // Delay;
    
    Parallel_CE_H;
    return 1;
}


/******************************************************************************
  函数(模块)名称:unsigned char ParallReadData(unsigned long ReadAddr)
  功能:	从地址读出一个字节
  输入参数:要读取的地址         	              		
  输出参数: 读出来的数据        	 		   		 
  其它说明: 以并行总线的方式。
*******************************************************************************/
unsigned char ReadByte_SST29(unsigned long ReadAddr)
{
    unsigned char ReadData;
    Parallel_CE_L;
    WrteAddrPort(ReadAddr);
    Parallel_OE_L;
    __delay_cycles(2);                 // Delay;
    ReadData = ReadDataPort();
    Parallel_OE_H;
    Parallel_CE_H;
    return ReadData;
}


/******************************************************************************
  函数(模块)名称:unsigned char MultiByteRead_SST29(unsigned long ReadAddr,
                                 unsigned char *ReadBuffer,unsigned int ReadLent)
  功能:	从地址读出一个字节
  输入参数:要读取的地址         	              		
  输出参数: 读出来的数据        	 		   		 
  其它说明: 以并行总线的方式。
*******************************************************************************/
unsigned char MultiByteRead_SST29(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
{
    DataPort_DIR_IN;
    Parallel_CE_L;
    Parallel_OE_L;    
    for(unsigned int i=0;i<ReadLent;i++)
    {
        WrteAddrPort(ReadAddr++);
         __delay_cycles(2);                 // Delay;
        *ReadBuffer++ = DataPort_IN;
    }
    Parallel_OE_H;
    Parallel_CE_H;
    return 1;
}


unsigned char CheckToggleBit(unsigned long ReadAddr)
{
    unsigned char PreData=0;
    unsigned char CurrData=0xff;
    unsigned int PollTime=1;
    while(PollTime)
    {
        PreData = ReadByte_SST29(ReadAddr)&BIT6;
        __delay_cycles(20);                 // Delay;
        CurrData = ReadByte_SST29(ReadAddr)&BIT6;
        if(CurrData==PreData)
        {
            return 1;
        }
    }
    return 0; 
}

unsigned char CheckDQ7Poling(unsigned long ReadAddr)
{
    unsigned char PreData=0;
    unsigned int PollTime=1;
    while(PollTime)
    {
        PreData = ReadByte_SST29(ReadAddr)&BIT7;
        __delay_cycles(20);                 // Delay;
        if(PreData)
        {
            return 1;
        }
    }
    return 0; 
}


/******************************************************************************
  函数(模块)名称:unsigned char ByteWrite_SST29(unsigned long WriteAddr,unsigned char WriteByte)
  功能:	读设备标识符
  输入参数:需要读取的标识答         	              		
  输出参数: 设备标识符        	 		   		 
  其它说明: 0，读取设备厂商标志 BF (FOR SST)，1，读取设备标识答 13 （FOR SST29SF040）
*******************************************************************************/
unsigned char ByteWrite_SST29(unsigned long WriteAddr,unsigned char WriteByte)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //进入ID模式
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0XA0);
    ParallWritData(WriteAddr,WriteByte);
    
    __delay_cycles(100);                 // Delay;
    CheckToggleBit(0);
    
    Parallel_CE_H;
    return 1;
  
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadID(unsigned char Manu_or_Device)
  功能:	读设备标识符
  输入参数:需要读取的标识答         	              		
  输出参数: 设备标识符        	 		   		 
  其它说明: 0，读取设备厂商标志 BF (FOR SST)，1，读取设备标识答 13 （FOR SST29SF040）
*******************************************************************************/
unsigned char MultiByteWrite_SST29(unsigned long WriteAddr,unsigned char *WriteBuffer,unsigned int Lent)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //进入ID模式
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0XA0);
    while(Lent--)
    {
        ParallWritData(WriteAddr++,*WriteBuffer++);
        __delay_cycles(20);                 // Delay;
        CheckToggleBit(0);
        
    }
    
    __delay_cycles(100);                 // Delay;
    CheckToggleBit(0);
    
    Parallel_CE_H;  
    return 1;
}


/******************************************************************************
  函数(模块)名称:void SectorErase_SST29(unsigned int SectorAddr)
  功能:	
  输入参数:         	              		
  输出参数:         	 		   		 
  其它说明: 扇区地址，最大16位长度容量。4M位实际使用12位地址，4096块扇区.
*******************************************************************************/
unsigned char SectorErase_SST29(unsigned int SectorAddr)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //6字节擦除序列
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X80);
    ParallWritData(0X555,0XAA);
    ParallWritData(0X2AA,0X55);
    
    ParallWritData((unsigned long)SectorAddr,0X20);
    
    __delay_cycles(100);                 // Delay;
    CheckToggleBit(0);
    
    Parallel_CE_H;
    return 1;
}

unsigned char BlockErase_SST29( unsigned int BlockAddr)
{
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadID(unsigned char Manu_or_Device)
  功能:	读设备标识符
  输入参数:需要读取的标识答         	              		
  输出参数: 设备标识符        	 		   		 
  其它说明: 0，读取设备厂商标志 BF (FOR SST)，1，读取设备标识答 13 （FOR SST29SF040）
*******************************************************************************/
unsigned char ChipErase_SST29(void)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //进入ID模式
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X80);
    
    ParallWritData(0X555,0XAA);             //进入ID模式
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X10);
    
    Parallel_CE_H;
    
    __delay_cycles(1000);                 // Delay;
    
    CheckToggleBit(0);    
    //CheckDQ7Poling(0);
    return 1;
    
}