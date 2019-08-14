#ifndef  __Up0CJ188_Module_H
#define  __Up0CJ188_Module_H


#define Up1FrameMaxSize  64                    //帧大小
struct Up1FrameStruct
{
    unsigned int FrameLent;                      //帧长度
    unsigned char FrameBuffer[Up1FrameMaxSize];  //帧缓数组
};

/******************************************************************************
  函数(模块)名称:void CJ188_ReceivProcess(void)
  功能:188协议接收处理任务函数
  输入参数:无         	              		
  输出参数:无         	 		   		 
  其它说明:调用后，会查询一次接收缓冲区，若有帧标志位或缓冲区满置起，则启动处理程序
*******************************************************************************/
unsigned char Up0CJ188_ReceivProcess(struct Up1FrameStruct *FrameTemp);

/******************************************************************************
  函数(模块)名称:CJ188_CommendPack(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
  功能:	188协议打包函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
其它说明: 
*******************************************************************************/
unsigned char CJ188_CommendPackSend(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer);


/******************************************************************************
  函数(模块)名称:unsigned char Up1CJ188_ReceivProcess(struct Up1FrameStruct *FrameTemp)
  功能:188协议接收处理任务函数
  输入参数:无         	              		
  输出参数:无         	 		   		 
  其它说明:调用后，会查询一次接收缓冲区，若有帧标志位或缓冲区满置起，则启动处理程序
*******************************************************************************/
unsigned char Up1CJ188_ReceivProcess(struct Up1FrameStruct *FrameTemp);

#endif