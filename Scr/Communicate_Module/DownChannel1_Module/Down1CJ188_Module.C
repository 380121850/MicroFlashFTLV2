  
//************************************************************************
//  Lierda, Inc      
//************************************************************************


#include "msp430x54x.h"
#include "stdlib.h"
#include ".\USCIA2_UART_Module\USCIA2_UART_Module.H"
#include "..\..\Storage_Module\Storage_Module.H"
#include "Down1CJ188_Module.H"


/******************************************************************************
  函数(模块)名称:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  功能:对一系列字节进行累加求和	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 字节的累加和         	 		   		 
  其它说明:属于内部函数
*******************************************************************************/
unsigned char Down1CJ188_SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    unsigned char SumCheck = 0X00;
    for(unsigned int i=0;i<SumCheckLent;i++) //累加求和
      SumCheck = SumCheck + *SumCheckBuff++;
    return SumCheck;                         //返回累加值
}

/******************************************************************************
  函数(模块)名称:static unsigned char FrameCheckSum(struct Up0FrameStruct *FrameTemp)
  功能:检查帧校验和	
  输入参数:         	              		
  输出参数:         	 		   		 
  其它说明:属于内部函数
*******************************************************************************/
unsigned char Down1CJ188_FrameCheckSum(struct Down1FrameStruct *FrameTemp)
{
    if(FrameTemp->FrameBuffer[FrameTemp->FrameLent-1-1] == Down1CJ188_SumCheck( FrameTemp->FrameBuffer,FrameTemp->FrameLent-1-1 ) )
      return 1;
    else
      return 0;
}


/******************************************************************************
  函数(模块)名称:CJ188_CommendPack(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
  功能:	188协议打包函数
  输入参数: 设备类型、设备ID，控制码，数据长度以及数据缓冲区
  输出参数: 成功或失败         	 		   		 
  其它说明: 若内存不足或串口忙，会导致失败。
*******************************************************************************/
unsigned char CJ188_CommendPackSend_USCIA2(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
{
    unsigned int i;
    unsigned char *FrameBuffer;
    FrameBuffer = (unsigned char *)malloc(15+DataLent); //申请帧数据空间
    if( NULL == FrameBuffer )//没有申请到，则返回
        return 0;
    else
    {
        FrameBuffer[0] = 0xFE;
        FrameBuffer[1] = 0xFE;
        FrameBuffer[2] = 0x68;         //帧开始字符
        FrameBuffer[3] = MeterType;    //设备类型
        for(i=4;i<11;i++)              //设备ID
          FrameBuffer[i] = *MeterIDbuff++;
        FrameBuffer[11] = CTL_Code;    //控制码
        FrameBuffer[12] = DataLent;    //数据长度
        for(i=13;i<DataLent+13;i++)    //数据域
          FrameBuffer[i] = *DataBuffer++;
        FrameBuffer[i++] = Down1CJ188_SumCheck( &FrameBuffer[2],11+DataLent );         //校验码
        FrameBuffer[i] = 0x16;         //帧停止字符
        
        i = USCIA2_MultiByteTX(FrameBuffer,15+DataLent);
        
        free(FrameBuffer);            //释放申请到的空间
        return i;
    }
}


/******************************************************************************
  函数(模块)名称:unsigned char GetFrame(struct FrameStruct *FrameTemp)
  功能:	从接收缓冲区中检出一帧数据
  输入参数:帧数据结构体指针，用来存储检出的帧数据         	              		
  输出参数: 1成功，0无帧数据输出         	 		   		 
  其它说明:检出的帧数据，可以确保是完整的，包括帧头，帧尾，数据域长度以及整个帧
           数据的合法长度，检测标准按188协议检测.
注:不检测帧内的校验和、设备类型以及设备ID、控制码
*******************************************************************************/
unsigned char Down1CJ188_GetFrame(struct Down1FrameStruct *FrameTemp)
{
    unsigned int  BufferLent;
    unsigned char ReadData;
    BufferLent = USCIA2_ReadRXBuffSize();          //取得当前缓冲区大小
    USCIA2_FrameDec();
    if(BufferLent >= 13)     //算上68，一帧数据最少13个字节
    {
        while(BufferLent--)
        { 
          if( USCIA2_ReadRXBuffer(&ReadData) )
          {
            if( 0x68 == ReadData)   //取得缓冲区一个字节数据//帧头判断
            {
                if(BufferLent > 12) 
                {
                    ReadData = USCIA2_ReadBufferOffset(9);
                    if( BufferLent >= (10+ReadData+2) )
                    {
                      ReadData = USCIA2_ReadBufferOffset( 9+ReadData+1+1 );
                        if(ReadData == 0x16)
                        {
                            ReadData = USCIA2_ReadBufferOffset(9);
                            FrameTemp->FrameBuffer[0]=0x68;
                            FrameTemp->FrameLent = 13+ ReadData;
                            for(unsigned int i=0;i<FrameTemp->FrameLent-1;i++)
                            {
                                USCIA2_ReadRXBuffer( &FrameTemp->FrameBuffer[i+1] );
                            }
                            return 1;
                        }
                        continue;
                    }
                    continue;
                }
                else return 0;//当前不够一帧数据
            }
          }
          else return 0;
        }
        return 0;     //已经取完数据
    }
    return 0;        //缓冲区数据长度不够
}