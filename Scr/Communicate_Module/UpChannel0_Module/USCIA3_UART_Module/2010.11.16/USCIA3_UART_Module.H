#ifndef  __USCIA3_UART_Module_H
#define  __USCIA3_UART_Module_H

#define  DataBufferMax    128 //缓冲区大小
#define  FrameTime        2   //定义为两字节之间的节拍数，超过，则视为帧结束，每节拍时间由定时器控制。

//发送、接收数据缓冲区
struct USCIA3_DataStruct        
{
    unsigned char Status;                    //缓冲区状态寄存顺
    unsigned int DataLent;                   //当前数据长度
    unsigned char *BufferPoint;              //指向最后接收或最后发送的数据
    unsigned char DataBuffer[DataBufferMax]; //数组缓冲
}; 
//Status寄存器各位宏定义
#define  TXBusy_BIT         BIT0  //发送空闲位
#define  BufferOV_BIT       BIT1  //缓冲区满标志位
#define  FrameTime_BIT      BIT2  //帧时间标志位

//接收帧定时器
struct USCIA3_CountFrameStruct
{
    unsigned char Status;   //帧时间检测状态位
    unsigned int CountTime; //帧时间检测计数器
};
//Status寄存器各位宏定义
#define  FrameTimeEN_BIT    BIT0  //帧时间检测使能位




/******************************************************************************
  函数(模块)名称:void USCIA3_UART_Init(void)
  功能:	时间初始化函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void USCIA3_UART_Init(void);

/******************************************************************************
  函数(模块)名称:unsigned char MultiByteTX_UA3RT(unsigned char *Buffer,unsigned int Lent)
  功能:	多字节发送函数
  输入参数: 要发送的数据缓冲区地址，长度
  输出参数: 成功或失败         	 		   		 
  其它说明: 若当前已经有任务在发送或传入的参数有问题，则返回0
*******************************************************************************/
unsigned char USCIA3_MultiByteTX(unsigned char *Buffer,unsigned int Lent);

/******************************************************************************
  函数(模块)名称:void USCIA3_CountFrameTime(void)
  功能:	帧时间检测计数器
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 执行，则对帧时间进行计数。
*******************************************************************************/
void USCIA3_CountFrameTime(void);



#endif