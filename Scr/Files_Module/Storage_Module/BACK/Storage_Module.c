//************************************************************************
//
//  Lierda, Inc                      SST25VF010 
//            
//************************************************************************
#include "msp430x54x.h"
//#include "FlashStorage.H"
//#include ".\SST25VF_Module\SST25VF010_Module.H"
#include "Storage_Module.H"

__no_init struct StorageStruct FlashRAM; //读写缓冲区


/******************************************************************************
  函数(模块)名称:void Storage_Module_Init(void)
  功能:	FLASH芯片初始化
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void Storage_Module_Init(void)
{
    FlashRAM.Status = 0X00;
    FlashRAM.StorageMod = SPIFlashMod;
    FlashRAM.Status = 0X00;
    FlashRAM.Status = 0X00;
    
    FlashModule_Init();
    FlashRAM_UPdate(0X00,FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);
    __delay_cycles(100);                 // Delay;
}


/******************************************************************************
  函数(模块)名称:void Storage_Module_Init(void)
  功能:	FLASH缓冲区更新
  输入参数:需要操作的FLASH地址，数据缓冲区地址，数据长度，读写选择以及存储选择         	              		
  输出参数: 无         	 		   		 
  其它说明:分存储类型：SPI或是并行的FLASH，并分读写两大类，再 
*******************************************************************************/
void FlashRAM_UPdate(unsigned long ProceAddr,unsigned char *Buffer,
                              unsigned long DataLent,unsigned char PrecessMod,
                              unsigned char StorageMod)
{
    if(StorageMod == SPIFlashMod)       //SPI FLASH操作
    {
        if(ReadMOD == PrecessMod) //读更新
        {
            if(DataLent<128)    //若更新的比较少，则把当前读地址前后64个字节更新
            {
                DataLent = 128;
                if(ProceAddr>63)
                {
                    ProceAddr = ProceAddr-63;
                }
                else
                {
                    ProceAddr = 0;
                }
            }
            
            FlashModule_MultiByteRead(ProceAddr,FlashRAM.RAMBuffer,DataLent);//更新缓冲区
            FlashRAM.StorageMod = SPIFlashMod;
            FlashRAM.CurrenAddr = ProceAddr;
            FlashRAM.DataLent = DataLent;
        }
        else if(WriteMod == PrecessMod) //写更新
        {
            FlashModule_SectorErase((unsigned int)(ProceAddr>>12));
            FlashModule_MultiByteWrite(ProceAddr,FlashRAM.RAMBuffer,DataLent);
        }
        else
        {
          //无操作
        }
        
    }
    else if(FlashRAM.StorageMod == ParalFlashMod) //并行 FLASH操作
    {
      
    }
    else
    {
      
    }
}

/******************************************************************************
  函数(模块)名称:void CopyFlashRAM(unsigned long ReadAddr,unsigned char *CopyBuffer,
                                   unsigned long CopyLent)
  功能:	从缓冲区复制数据字节到指定缓冲区
  输入参数:复制的地址(是指FLASH地址)，指定缓冲区地址(MCU内RAM地址)，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 内部函数，对用户不公开使用。确保要读的地址在缓冲区地址范围内
*******************************************************************************/
void CopyFlashRAM(unsigned long ReadAddr,unsigned char *CopyBuffer,unsigned long CopyLent)
{
    unsigned long OffSetAddr;                  //地址偏移
    OffSetAddr = ReadAddr-FlashRAM.CurrenAddr;
    for(unsigned long i=0;i<CopyLent;i++)
    {
      *CopyBuffer++ = FlashRAM.RAMBuffer[OffSetAddr+i];
    }
}

/******************************************************************************
  函数(模块)名称:void WriteFlashRAM(unsigned long WriteAddr,unsigned char *WriteBuffer,
                                    unsigned long WriteLent)
  功能:	向缓冲区定稿指定字节
  输入参数:         	              		
  输出参数: 无         	 		   		 
  其它说明: 内部函数，对用户不公开使用
*******************************************************************************/
void WriteFlashRAM(unsigned long WriteAddr,unsigned char *WriteBuffer,unsigned long WriteLent)
{
    unsigned long OffSetAddr;                  //地址偏移
    OffSetAddr = WriteAddr-FlashRAM.CurrenAddr;
    for(unsigned long i=0;i<WriteLent;i++)
    {
      FlashRAM.RAMBuffer[OffSetAddr+i] = *WriteBuffer++;
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  功能:	
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 缓冲区映射FLASH存储某个区，地址递增
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned int ReadLent, unsigned char StorageMod)
{
    unsigned char ReadFlag=0;
    unsigned long FlashRAM_EndaAddr = FlashRAM.CurrenAddr + FlashRAM.DataLent;
    unsigned long ReadEndAddr = ReadAddr + ReadLent;
    
    if((FlashRAM.Status & RAMBufferBusy_BIT)==0)
    {
        FlashRAM.Status |= RAMBufferBusy_BIT;  //置忙标志位，以防止其它程序操作缓冲区
        if(StorageMod==FlashRAM.StorageMod)    //判断当前缓冲区类型 SPI的FLASH或并行FLASH
        {
            if( ReadAddr > FlashRAM_EndaAddr )            //1 数据块在缓冲区块右边 （数据块地址域大于缓冲区块）
            {
                //分离,需要读取的数据块完全不在当前缓冲区
                FlashModule_MultiByteRead(ReadAddr,ReadBuffer,ReadLent);
                ReadAddr += ReadLent>>1;   //算出中间地址
                FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                SectorSize+1,ReadMOD,SPIFlashMod);//更新中间地址所在的扇区数据到缓冲区
                ReadFlag = 1;
            }
            else if( ReadAddr < FlashRAM.CurrenAddr) //2
            {   
                if( ReadEndAddr > FlashRAM_EndaAddr )      //2.1
                {                                       //要读数据块包含缓冲区块
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,FlashRAM.CurrenAddr-ReadAddr);
                    ReadBuffer += FlashRAM.CurrenAddr-ReadAddr+1;
                    
                    CopyFlashRAM(FlashRAM.CurrenAddr,ReadBuffer,FlashRAM.DataLent);
                    ReadBuffer += FlashRAM.DataLent;
                    
                    FlashModule_MultiByteRead(FlashRAM_EndaAddr,ReadBuffer,ReadEndAddr-FlashRAM_EndaAddr);
                    ReadAddr += ReadLent>>1;   //算出中间地址  
                    FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                    SectorSize+1,ReadMOD,SPIFlashMod);//更新中间地址所在的扇区数据到缓冲区                    
                    ReadFlag = 1;
                }
                else if( ReadEndAddr < FlashRAM.CurrenAddr)//2.2
                {
                                      //分离,需要读取的数据块完全不在当前缓冲区
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,ReadLent);
                    ReadAddr += ReadLent>>1;   //算出中间地址
                    FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                    SectorSize+1,ReadMOD,SPIFlashMod);//更新中间地址所在的扇区数据到缓冲区
                    ReadFlag = 1;
                }
                else                                       //2.3
                {   
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,FlashRAM.CurrenAddr-ReadAddr);
                    CopyFlashRAM(FlashRAM.CurrenAddr,ReadBuffer+(FlashRAM.CurrenAddr-ReadAddr)+1,ReadEndAddr-FlashRAM.CurrenAddr+1);
                    ReadAddr += ReadLent>>1;   //算出中间地址
                    FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                    SectorSize+1,ReadMOD,SPIFlashMod);//更新中间地址所在的扇区数据到缓冲区
                    ReadFlag = 1;
                }
            }
            else                                        //3
            {
                if( ReadEndAddr > FlashRAM_EndaAddr )      //3.1
                {
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,ReadLent);
                    ReadFlag = 1;
                }
                else                                     //3.3  缓冲区命中
                {   
                    CopyFlashRAM(ReadAddr,ReadBuffer,ReadLent);
                    ReadFlag = 1;
                }
                
            }
        } 
        FlashRAM.Status &= ~RAMBufferBusy_BIT;  //清忙标志位，允许其它程序操作缓冲区
    }
    return ReadFlag;
}


/******************************************************************************
  函数(模块)名称:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  功能:	
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WritedAddr,unsigned char *WritedBuffer,
                                  unsigned int WriteLent, unsigned char StorageMod)
{
    unsigned long MasDataAddr_End = WritedAddr + (unsigned long)WriteLent;    //要写的数据块的结束地址
    
    unsigned long CurrenSectorWriteAddr_Start; //当前扇区写起始地址
    unsigned long CurrenSectorWriteAddr_End;   //当前扇区写结束地址
    unsigned int  CurrenSectorWriteCnt;        //当前扇区写要写的字节数
    unsigned long CurrenSectorAddr_End;        //当前页结束地址
    
    if(0 == WriteLent)
    {
        return 0;
    }
    
    if((FlashRAM.Status & RAMBufferBusy_BIT)==0)
    {
        FlashRAM.Status |= RAMBufferBusy_BIT;  //置忙标志位，以防止其它程序操作缓冲区
        if(StorageMod==FlashRAM.StorageMod)    //判断当前缓冲区类型 SPI的FLASH或并行FLASH
        {
            CurrenSectorWriteAddr_Start =  WritedAddr;            //初始化当前写开始地址
            CurrenSectorAddr_End = CurrenSectorWriteAddr_Start| (unsigned long)SectorSize; //取得当前扇区结束地址
         
            while(1)
            {
                if(MasDataAddr_End > CurrenSectorAddr_End)
                {//数据块结束地址在当前页之外
                    CurrenSectorWriteAddr_End = CurrenSectorAddr_End;           //页写结束地址使用当前页结束地址
                    CurrenSectorWriteCnt = CurrenSectorWriteAddr_End - CurrenSectorWriteAddr_Start+1;  //计算当前页要写的字节数
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);//把扇区更新到缓冲区      
                    WriteFlashRAM(CurrenSectorWriteAddr_Start,WritedBuffer,CurrenSectorWriteCnt);  //数据写入到缓冲区
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,WriteMod,SPIFlashMod); //把缓冲区更新到扇区
                    
                    CurrenSectorWriteAddr_Start = CurrenSectorWriteAddr_Start + CurrenSectorWriteCnt;//计算下页开始地址
                    CurrenSectorAddr_End = CurrenSectorWriteAddr_Start| (unsigned long)SectorSize;   //取得当前扇区结束地址
                    WritedBuffer = WritedBuffer + CurrenSectorWriteCnt;                              //计算新的数据块指针
                }
                else  
                {//数据块结束地址在当前页内
                    CurrenSectorWriteAddr_End = MasDataAddr_End;           //页写结束地址使用数据块结束地址
                    CurrenSectorWriteCnt = CurrenSectorWriteAddr_End - CurrenSectorWriteAddr_Start;  //计算当前页要写的字节数
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);//把扇区更新到缓冲区
                    WriteFlashRAM(CurrenSectorWriteAddr_Start,WritedBuffer,CurrenSectorWriteCnt);                               //数据写入到缓冲区
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,WriteMod,SPIFlashMod); //把缓冲区写到扇区
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);  //把扇区读到缓冲区
                    break;
                }
            }
        }
        FlashRAM.Status &= ~RAMBufferBusy_BIT;  //清忙标志位，允许其它程序操作缓冲区
        return 1;
    }
    return 0;
}

