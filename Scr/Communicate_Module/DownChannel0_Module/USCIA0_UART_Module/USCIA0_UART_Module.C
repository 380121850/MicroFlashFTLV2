  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P3.4/UCA0TXD|------------>
//            |                 | 115200 - 8N1
//            |     P3.5/UCA0RXD|<------------

#include "msp430x54x.h"
#include "USCIA0_UART_Module.H"

struct FrameAckTimeStruct 
{
    unsigned char Status;
    unsigned int TimeCnt;
    unsigned char ReTryCnt;
};
struct FrameAckTimeStruct USCIA0_FrameAckTime;

struct USCI_DataStruct USCI_A0TXBuffer;      //发送接口
CirQueueStruct USCI_QueueBuffer;             //接收缓冲队列
static struct USCI_CountFrameStruct USCI_A0CountFrame;//接收帧计时

/******************************************************************************
  函数(模块)名称:void USCIA0_FrameAckTime_ReTryCntDec(void)
  功能:	帧响应错误次数减1
  输入参数: 无        	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void USCIA0_FrameAckTime_ReTryCntDec(void)
{
    if( USCIA0_FrameAckTime.ReTryCnt )
      USCIA0_FrameAckTime.ReTryCnt--;
}

/******************************************************************************
  函数(模块)名称:unsigned char USCIA0_FrameAckTime_ReTryCntRead(void)
  功能:	读当前帧响应错误计数
  输入参数: 无        	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char USCIA0_FrameAckTime_ReTryCntRead(void)
{
    return USCIA0_FrameAckTime.ReTryCnt;
}

/******************************************************************************
  函数(模块)名称:void USCIA0_FrameAckTime_ReTryCntWrite(unsigned char Cnt)
  功能:	写当前帧响应错误计数
  输入参数: 无        	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
void USCIA0_FrameAckTime_ReTryCntWrite(unsigned char Cnt)
{
    USCIA0_FrameAckTime.ReTryCnt = Cnt;
}


unsigned int USCIA0_FrameAckTime_TimeCntRead(void)
{
    return USCIA0_FrameAckTime.TimeCnt;
}

void USCIA0_FrameAckTime_TimeCntWrite( unsigned int Cnt )
{
    USCIA0_FrameAckTime.TimeCnt = Cnt;
}




void USCIA0_FrameAckTime_En(unsigned char flag)
{
    if(flag)
      USCIA0_FrameAckTime.Status |= BIT2;   //打开计时任务总开关
    else
     USCIA0_FrameAckTime.Status &= ~BIT2;   //关闭计时任务总开关 
}

void USCIA0_FrameAckTime_SubEn(unsigned char flag)
{
    if(flag)
      USCIA0_FrameAckTime.Status |= BIT1;   //打开计时任务子开关
    else
      USCIA0_FrameAckTime.Status &= ~BIT1;   //关闭计时任务子开关
      
}

/******************************************************************************
  函数(模块)名称:void USCIA0_FrameAckTime_CountTime(void)
  功能:	下行通道任务计时函数
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:
******************************************************************************/
void USCIA0_FrameAckTime_CountTime(void)
{
    if( USCIA0_FrameAckTime.Status & BIT2 )   //打开计时任务总开关
    {
        if( USCIA0_FrameAckTime.Status & BIT1 )//查询计时开关
        {
            if( USCIA0_FrameAckTime.TimeCnt )
            {
                USCIA0_FrameAckTime.TimeCnt--;
            }
        }
    }
}


/******************************************************************************
  函数(模块)名称:void InitQueue(CirQueueStruct *Q)
  功能:	置队空
  输入参数: 队列指针        	              		
  输出参数: 无         	 		   		 
  其它说明: 初始化队列，此时队列为空
*******************************************************************************/
void InitQueue(CirQueueStruct *Q)
{
    Q->Queue_Front = 0;
    Q->Queue_rear = 0;
    Q->DataLent = 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char QueueFull(CirQueueStruct *Q)
  功能:	判队满
  输入参数: 队列指针       	              		
  输出参数: 1队列满，0队列没满         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char QueueFull(CirQueueStruct *Q)
{
    if( USCIA0QueueBufferSize == Q->DataLent )
      return 1;
    else  return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char EnQueue(CirQueueStruct *Q,unsigned char DataByte)
  功能:	入队
  输入参数: 队列指针，字节数据       	              		
  输出参数: 1入队，0队列已经满，数据无法入队         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char EnQueue(CirQueueStruct *Q,unsigned char DataByte)
{
    if( QueueFull(Q) )
    {
       return 0;
    }
    else
    {
        Q->DataLent++;
        Q->DataBuffer[Q->Queue_rear] = DataByte;
        Q->Queue_rear = (Q->Queue_rear+1)%USCIA0QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char DeQueue(CirQueueStruct *Q,unsigned char *ReadData)
  功能:	出队
  输入参数: 队列指针，字节数据指针       	              		
  输出参数: 1出队，0队列已经空，无数据出队         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char DeQueue(CirQueueStruct *Q,unsigned char *ReadData)
{
    if(0 == Q->DataLent)
    {
        return 0;
    }
    else
    {
        *ReadData = Q->DataBuffer[Q->Queue_Front];
        Q->DataLent--;
        Q->Queue_Front = (Q->Queue_Front+1)%USCIA0QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char DeQueueOffset(CirQueueStruct *Q,unsigned int Offset)
  功能:	返回当前队头指定偏移的数据
  输入参数:       	              		
  输出参数:         	 		   		 
  其它说明: 如偏移量为1，则返回当前队头的下一位数
*******************************************************************************/
unsigned char DeQueueOffset(CirQueueStruct *Q,unsigned int Offset)
{
    Offset = (Q->Queue_Front+Offset)%USCIA0QueueBufferSize;
    return Q->DataBuffer[Offset];
}

/******************************************************************************
  函数(模块)名称:unsigned char *GetQueueFront(CirQueueStruct *Q)
  功能:	取当前队列头数据的指针
  输入参数: 队列指针       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char *GetQueueFront(CirQueueStruct *Q)
{
    return &Q->DataBuffer[Q->Queue_Front];
}

/******************************************************************************
  函数(模块)名称:unsigned char *GetQueueFront(CirQueueStruct *Q)
  功能:	取当前队列头数据的指针
  输入参数: 队列指针       	              		
  输出参数:          	 		   		 
  其它说明: 若返回指针后，又有数据入队，则之前返回的指针与此时指针可能不符。
*******************************************************************************/
unsigned int GetQueueFront_Ext(CirQueueStruct *Q)
{
    return Q->Queue_Front;
}



/******************************************************************************
  函数(模块)名称:void USCIA0_UARTBufferInit(void)
  功能:	缓冲区初始化函数
  输入参数: 无        	              		
  输出参数: 无         	 		   		 
  其它说明: 对发送和接收函数区进行初始化
*******************************************************************************/
static void USCIA0_UARTBufferInit(void)
{
  USCI_A0TXBuffer.Status = 0X00;    //缓冲区初始化
  USCI_A0TXBuffer.DataLent = 0X00;
  
  USCI_QueueBuffer.Status = 0X00;
  USCI_QueueBuffer.DataLent = 0X00;
  
  USCI_A0CountFrame.Status = 0x00;
  USCI_A0CountFrame.CountTime = 0X00;
}


/******************************************************************************
  函数(模块)名称:void USCIA0_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  功能:	
  输入参数:    ParityBit 校验位 0表示无校验位，1表示奇校验，2表示偶校验
               DataBit   数据位 8表示8位数据位，7表示7位数据位
               StopBit   停止位 1表示1位停止位，2表示2位停止位
  输出参数:          	 		   		 
  其它说明: 4系列的默认是1.048576MHz
*******************************************************************************/
void USCIA0_UARTSet(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
{
    P3SEL |= BIT4+BIT5;                       // P3.4,5 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    if(ParityBit==0)
    {
        UCA0CTL0 &= ~UCPEN;                     // 无校验位
    }
    else if(ParityBit==1)
    {
        UCA0CTL0 |= UCPEN;                     // 使能校验位功能
        UCA0CTL0 &= ~UCPAR;                    // 奇校验
    }
    else if(ParityBit==2)
    {
        UCA0CTL0 |= UCPEN+UCPAR;                     // 使能校验位功能 偶校验
    }
    
    if(DataBit==8)
    {
        UCA0CTL0 &= ~UC7BIT;                     // 8位数据位
    }
    else if(DataBit==7)
    {
        UCA0CTL0 |= UC7BIT;                    // 7位数据位
    }
    
    if(StopBit==1)
    {
        UCA0CTL0 &= ~UCSPB;                     // 1位停止位
    }
    else if(StopBit==2)
    {
        UCA0CTL0 |= UCSPB;                    // 2位停止位
    }
    UCA0BRW = 8000000/BaudRate;
    //UCA0BR0 = 109;                            // 1MHz 115200 (see User's Guide)
    //UCA0BR1 = 0;                              // 1MHz 115200
    UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA0_UARTBufferInit();                  //接收、发送缓冲区初始化
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}


/******************************************************************************
  函数(模块)名称:void ByteSend_UA0RT(unsigned char SendData)
  功能:	单字节发送函数
  输入参数: 要发送的数据字节
  输出参数:          	 		   		 
  其它说明: static 
*******************************************************************************/
void USCIA0_ByteSend(unsigned char SendData)
{
    while (!(UCA0IFG&UCTXIFG));            // USCI_A0 TX buffer ready?
    UCA0TXBUF = SendData;                  // TX -> RXed character
}


/******************************************************************************
  函数(模块)名称:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  功能:	多字节发送函数
  输入参数: 要发送的数据缓冲区地址，长度
  输出参数: 成功或失败         	 		   		 
  其它说明: 若当前已经有任务在发送或传入的参数有问题，则返回0
*******************************************************************************/
unsigned char USCIA0_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    if(!USCI_A0TXBuffer.Status)//确保发送空闲
    {
        if(Lent)             //确保字节数大于0
        {
            USCI_A0TXBuffer.Status |= BIT0;       //置忙标志位
            USCI_A0TXBuffer.DataLent = Lent-1;    //取字节数
            for(unsigned int i=0;i<Lent;i++)
            USCI_A0TXBuffer.DataBuffer[i] = *Buffer++;
            USCI_A0TXBuffer.BufferPoint = USCI_A0TXBuffer.DataBuffer;      //取得发送首地址
            USCIA0_ByteSend(*USCI_A0TXBuffer.BufferPoint++);                //发送第一个字节，以启动中断发送
            UCA0IE |= UCTXIE;                                              // 打开发送中断
            return 1;
        }
        else return 0;
    }
    else return 0;
}



/******************************************************************************
  函数(模块)名称:void USCIA0_ReSetFrameTime(void)
  功能:	帧间隔时间检测开启及检测计数器清0
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 接收到一个字节后，调用这个函数，会把帧时间计数器清0。若末开启，则执行开启动作。
*******************************************************************************/
static void USCIA0_ReSetFrameTime(void)
{
    USCI_A0CountFrame.Status |= FrameTimeEN_BIT;
    if(USCI_A0CountFrame.CountTime==FrameTime)
    {
        USCI_QueueBuffer.FrameCnt++;              //断帧次数加1
    }
    USCI_A0CountFrame.CountTime = 0;              //清空计数
}

/******************************************************************************
  函数(模块)名称:void USCIA0_CountFrameTime(void)
  功能:	帧时间检测计数器
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 执行，则对帧时间进行计数。
*******************************************************************************/
void USCIA0_CountFrameTime(void)
{
    if(USCI_A0CountFrame.Status & FrameTimeEN_BIT)
    {
        USCI_A0CountFrame.CountTime++;//计数;
        if(USCI_A0CountFrame.CountTime==FrameTime)
        {
            USCI_QueueBuffer.FrameCnt++;              //断帧次数加1
            USCI_A0CountFrame.Status &= ~FrameTimeEN_BIT; //关掉帧检测计时
            USCI_A0CountFrame.CountTime = 0;              //清空计数
        }
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char USCIA0_ReadRXBuffer(unsigned char *ReadData)
  功能:	接收缓冲区输出一个字节
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffer(unsigned char *ReadData)
{
    return DeQueue(&USCI_QueueBuffer,ReadData);
}

unsigned char USCIA0_ReadBufferOffset(unsigned int Offset)
{
    return DeQueueOffset(&USCI_QueueBuffer,Offset);
}

/******************************************************************************
  函数(模块)名称:unsigned char USCIA0_ReadRXBuffStatus(void)
  功能:	读接收缓冲区状态寄存器
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffStatus(void)
{
    return USCI_QueueBuffer.Status;
}


/******************************************************************************
  函数(模块)名称:unsigned char USCIA0_ReadRXBuffStatus(void)
  功能:	读接收缓冲区状态寄存器
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
void USCIA0_RXBuffClearn(void)
{
    USCI_QueueBuffer.Status = 0X00;
    USCI_QueueBuffer.DataLent = 0X00;
    InitQueue( &USCI_QueueBuffer );
}
/******************************************************************************
  函数(模块)名称:unsigned int USCIA0_ReadRXBuffFrame(void)
  功能:	读接收缓冲区断帧数
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffFrame(void)
{
    return USCI_QueueBuffer.FrameCnt;
}

void USCIA0_FrameDec(void)
{
    if(USCI_QueueBuffer.FrameCnt)
      USCI_QueueBuffer.FrameCnt--;
}

/******************************************************************************
  函数(模块)名称:unsigned int USCIA0_ReadRXBuffSize(void)
  功能:	读接收缓冲区数据长度
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned int USCIA0_ReadRXBuffSize(void)
{
    return USCI_QueueBuffer.DataLent;
}


unsigned char *USCIA0_ReadRXBuffPoint(void)
{
    return GetQueueFront(&USCI_QueueBuffer);
}



unsigned char USCIA0_ReadRXBuffQueFront(unsigned int Num)
{
    return GetQueueFront_Ext(&USCI_QueueBuffer);
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:  // Vector 0 - no interrupt
    break;                             
  case 2:  // Vector 2 - RXIFG
    //USCIA1_ByteSend(UCA0RXBUF);
       if(EnQueue(&USCI_QueueBuffer,UCA0RXBUF))
       {
           if( QueueFull(&USCI_QueueBuffer) )
           {
               USCI_QueueBuffer.Status |= BufferOV_BIT;       //置缓冲区满标志位
               USCI_A0CountFrame.Status &= ~FrameTimeEN_BIT; //关掉帧检测计时
               USCI_A0CountFrame.CountTime = 0;              //清空计数               
           }
           else             
           {
               USCI_QueueBuffer.Status &= ~BufferOV_BIT;       //清缓冲区满标志位
               USCIA0_ReSetFrameTime();   //缓冲区没满，继续接收数据，并复位帧检测计数器。
           }
       }
       else
       {
         USCI_A0CountFrame.Status &= ~FrameTimeEN_BIT; //关掉帧检测计时
         USCI_QueueBuffer.Status |= BufferOV_BIT;       //置缓冲区满标志位
       }
       if( USCIA0_FrameAckTime.Status & BIT2 )
       USCIA0_FrameAckTime_SubEn(0);//关闭计时子开关
       
    break;
  case 4:  // Vector 4 - TXIFG
    if(USCI_A0TXBuffer.DataLent)
    {
        USCI_A0TXBuffer.DataLent--;
        UCA0TXBUF = *USCI_A0TXBuffer.BufferPoint++;
        if(USCI_A0TXBuffer.DataLent == 0)
        {
            USCI_A0TXBuffer.Status &= ~TXBusy_BIT;  //发送完毕，则清掉空闲位
            UCA0IE &= ~UCTXIE; 
            
            if( USCIA0_FrameAckTime.Status & BIT2 )
            USCIA0_FrameAckTime_SubEn(1);//打开发送计时子开关
            //USCIA1_ByteSend(0X55);
        }
    }
    break;                                  
  default: 
    break;
  }
}