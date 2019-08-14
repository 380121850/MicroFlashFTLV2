#ifndef  __Down0CJ188_Module_H
#define  __Down0CJ188_Module_H

#define Down0FrameMaxSize  64                    //帧大小
struct Down0FrameStruct
{
    unsigned int FrameLent;                   //帧长度
    unsigned char FrameBuffer[Down0FrameMaxSize];  //帧缓数组
};


/******************************************************************************
  函数(模块)名称:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  功能:对一系列字节进行累加求和	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 字节的累加和         	 		   		 
  其它说明:属于内部函数
*******************************************************************************/
unsigned char Down0CJ188_SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent);

/******************************************************************************
  函数(模块)名称:static unsigned char FrameCheckSum(struct Up0FrameStruct *FrameTemp)
  功能:检查帧校验和	
  输入参数:         	              		
  输出参数:         	 		   		 
  其它说明:属于内部函数
*******************************************************************************/
unsigned char Down0CJ188_FrameCheckSum(struct Down0FrameStruct *FrameTemp);


/******************************************************************************
  函数(模块)名称:CJ188_CommendPack(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
  功能:	188协议打包函数
  输入参数: 设备类型、设备ID，控制码，数据长度以及数据缓冲区
  输出参数: 成功或失败         	 		   		 
  其它说明: 若内存不足或串口忙，会导致失败。
*******************************************************************************/
unsigned char CJ188_CommendPackSend_USCIA0(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char GetFrame(struct FrameStruct *FrameTemp)
  功能:	从接收缓冲区中检出一帧数据
  输入参数:帧数据结构体指针，用来存储检出的帧数据         	              		
  输出参数: 1成功，0无帧数据输出         	 		   		 
  其它说明:检出的帧数据，可以确保是完整的，包括帧头，帧尾，数据域长度以及整个帧
           数据的合法长度，检测标准按188协议检测.
注:不检测帧内的校验和、设备类型以及设备ID、控制码
*******************************************************************************/
unsigned char Down0CJ188_GetFrame(struct Down0FrameStruct *FrameTemp);


#endif