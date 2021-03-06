#ifndef  __USCIA3_UART_Module_H
#define  __USCIA3_UART_Module_H


#define  DataBufferMax    250  //缓冲区大小
//发送、接收数据缓冲区结构体
struct USCIA3_DataStruct        
{
    unsigned char Status;                     //缓冲区状态寄存顺
    unsigned int  DataLent;                   //当前数据长度
    unsigned char *BufferPoint;               //指向最后接收或最后发送的数据
    unsigned char DataBuffer[DataBufferMax];  //数组缓冲
}; 
//Status寄存器各位宏定义
#define  TXBusy_BIT         BIT0  //发送空闲位
#define  BufferOV_BIT       BIT1  //缓冲区满标志位
#define  FrameTime_BIT      BIT2  //帧时间标志位

#define QueueBufferSize  255
typedef struct 
{
    unsigned char Status;                       //缓冲区状态寄存顺
    unsigned char FrameCnt;                     //断帧数
    
    unsigned int  Queue_Front;                  //头指针，队非空时指向队头元素
    unsigned int  Queue_rear;                   //尾指针，队非空时指向队尾元素的下一位置
    unsigned int  DataLent;                     //队中数据
    unsigned char DataBuffer[QueueBufferSize];  //数组缓冲  
}A3CirQueueStruct;

#define  FrameTime        8   //定义为两字节之间的节拍数，超过，则视为帧结束，每节拍时间由定时器控制。

//接收帧定时器
struct USCIA3_CountFrameStruct
{
    unsigned char Status;   //帧时间检测状态位
    unsigned int CountTime; //帧时间检测计数器
};
//Status寄存器各位宏定义
#define  FrameTimeEN_BIT    BIT0  //帧时间检测使能位


/******************************************************************************
  函数(模块)名称:void USCIA3_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  功能:	
  输入参数:    ParityBit 校验位 0表示无校验位，1表示奇校验，2表示偶校验
               DataBit   数据位 8表示8位数据位，7表示7位数据位
               StopBit   停止位 1表示1位停止位，2表示2位停止位
  输出参数:          	 		   		 
  其它说明: 4系列的默认是1.048576MHz
*******************************************************************************/
void USCIA3_UARTSet(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit);

/******************************************************************************
  函数(模块)名称:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
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


/******************************************************************************
  函数(模块)名称:unsigned char USCIA3_ReadRXBuffer(unsigned char *ReadData)
  功能:	接收缓冲区输出一个字节
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char USCIA3_ReadRXBuffer(unsigned char *ReadData);


/******************************************************************************
  函数(模块)名称:unsigned char USCIA3_ReadRXBuffStatus(void)
  功能:	读接收缓冲区状态寄存器
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char USCIA3_ReadRXBuffStatus(void);

/******************************************************************************
  函数(模块)名称:unsigned int USCIA3_ReadRXBuffFrame(void)
  功能:	读接收缓冲区断帧数
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char USCIA3_ReadRXBuffFrame(void);

/******************************************************************************
  函数(模块)名称:unsigned int USCIA3_ReadRXBuffSize(void)
  功能:	读接收缓冲区数据长度
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned int USCIA3_ReadRXBuffSize(void);

void USCIA3_FrameDec(void);

unsigned char USCIA3_ReadBufferOffset(unsigned int Offset);

#endif