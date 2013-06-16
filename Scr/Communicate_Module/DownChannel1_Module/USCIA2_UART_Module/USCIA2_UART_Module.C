  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P9.4/UCA2TXD|------------>
//            |                 | 9600 - 8N1
//            |     P9.5/UCA2RXD|<------------

#include "msp430x54x.h"
#include "USCIA2_UART_Module.H"

static USCA2CirQueueStruct USCIA2_TXQueueBuffer;             //发送缓冲队列
static USCA2CirQueueStruct USCIA2_RXQueueBuffer;             //接收缓冲队列

static struct USCIA2_CountFrameStruct USCIA2_CountFrame;



/******************************************************************************
  函数(模块)名称:static void InitQueue(USCA2CirQueueStruct *Q)
  功能:	置队空,初始化队列
  输入参数: 队列指针        	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/
static void InitQueue(USCA2CirQueueStruct *Q)
{
    Q->Queue_Front = 0;
    Q->Queue_rear = 0;
    Q->DataLent = 0;
}

/******************************************************************************
  函数(模块)名称:static unsigned char QueueFull(USCA2CirQueueStruct *Q)
  功能:	判队满
  输入参数: 队列指针       	              		
  输出参数: 1队列满，0队列没满         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char QueueFull(USCA2CirQueueStruct *Q)
{
    if( USCA2QueueBufferSize == Q->DataLent ) 
      return 1;
    else  
      return 0;
}


/******************************************************************************
  函数(模块)名称:static unsigned char EnQueue(USCA2CirQueueStruct *Q,unsigned char DataByte)
  功能:	入队
  输入参数: 队列指针，字节数据       	              		
  输出参数: 1入队，0队列已经满，数据无法入队         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char EnQueue(USCA2CirQueueStruct *Q,unsigned char DataByte)
{
    if( QueueFull(Q) )
    {
       return 0;
    }
    else
    {
        Q->DataLent++;
        Q->DataBuffer[Q->Queue_rear] = DataByte;
        Q->Queue_rear = (Q->Queue_rear+1)%USCA2QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  函数(模块)名称:static unsigned char DeQueue(USCA2CirQueueStruct *Q,unsigned char *ReadData)
  功能:	出队
  输入参数: 队列指针，字节数据指针       	              		
  输出参数: 1出队，0队列已经空，无数据出队         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char DeQueue(USCA2CirQueueStruct *Q,unsigned char *ReadData)
{
    if(0 == Q->DataLent)
    {
        return 0;
    }
    else
    {
        *ReadData = Q->DataBuffer[Q->Queue_Front];
        Q->DataLent--;
        Q->Queue_Front = (Q->Queue_Front+1)%USCA2QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  函数(模块)名称:static unsigned char DeQueueOffset(USCA2CirQueueStruct *Q,unsigned int Offset)
  功能:	返回当前队头指定偏移的数据
  输入参数:       	              		
  输出参数:         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char DeQueueOffset(USCA2CirQueueStruct *Q,unsigned int Offset)
{
    Offset = (Q->Queue_Front+Offset)%USCA2QueueBufferSize;
    return Q->DataBuffer[Offset];
}




/******************************************************************************
  函数(模块)名称:void USCIA2_UARTBufferInit(void)
  功能:	缓冲区初始化函数
  输入参数: 无        	              		
  输出参数: 无         	 		   		 
  其它说明: 对发送和接收函数区进行初始化
*******************************************************************************/
static void USCIA2_UARTBufferInit(void)
{
    USCIA2_TXQueueBuffer.Status = 0X00;    //发送缓冲区初始化
    USCIA2_TXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA2_TXQueueBuffer);
    
    USCIA2_RXQueueBuffer.Status = 0X00;    //接收缓冲区初始化
    USCIA2_RXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA2_RXQueueBuffer);
    
    USCIA2_CountFrame.Status = 0x00;    //断帧检测寄存器初始化
    USCIA2_CountFrame.CountTime = 0X00;
}


/******************************************************************************
  函数(模块)名称:void USCIA2_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  功能:	
  输入参数:    ParityBit 校验位 0表示无校验位，1表示奇校验，2表示偶校验
               DataBit   数据位 8表示8位数据位，7表示7位数据位
               StopBit   停止位 1表示1位停止位，2表示2位停止位
  输出参数:          	 		   		 
  其它说明: 4系列的默认是1.048576MHz
*******************************************************************************/
void USCIA2_UARTSet(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
{
    P9SEL |= BIT4+BIT5;                       // P9.4,5 = USCIA2 TXD/RXD
    UCA2CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA2CTL1 |= UCSSEL_2;                     // SMCLK
    
//校验位选择 0无校验 1奇校验 2偶校验
    if(ParityBit==0)
    {
        UCA2CTL0 &= ~UCPEN;                     // 无校验位
    }
    else if(ParityBit==1)
    {
        UCA2CTL0 |= UCPEN;                     // 使能校验位功能
        UCA2CTL0 &= ~UCPAR;                    // 奇校验
    }
    else if(ParityBit==2)
    {
        UCA2CTL0 |= UCPEN+UCPAR;                     // 使能校验位功能 偶校验
    }
    
//数据位选择 7 数据长度是7位 8 数据长度是8位    
    if(DataBit==8)
    {
        UCA2CTL0 &= ~UC7BIT;                     // 8位数据位
    }
    else if(DataBit==7)
    {
        UCA2CTL0 |= UC7BIT;                    // 7位数据位
    }

//停止位选择 1 1位停止位 2 2位停止位    
    if(StopBit==1)
    {
        UCA2CTL0 &= ~UCSPB;                     // 1位停止位
    }
    else if(StopBit==2)
    {
        UCA2CTL0 |= UCSPB;                    // 2位停止位
    }
    
//波特率选择 默认时钟频率为DCO 1048578HZ
    UCA2BRW = 8000000/BaudRate;
    //UCA2BR0 = 109;                            // 1MHz 115200 (see User's Guide)
    //UCA2BR1 = 0;                              // 1MHz 115200
    
    UCA2MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA2CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA2_UARTBufferInit();                  //接收、发送缓冲区初始化
    UCA2IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}


/******************************************************************************
  函数(模块)名称:void USCIA2_ByteSend(unsigned char SendData)
  功能:	单字节发送函数
  输入参数: 要发送的数据字节
  输出参数:          	 		   		 
  其它说明: static 
*******************************************************************************/
void USCIA2_ByteSend(unsigned char SendData)
{
    while (!(UCA2IFG&UCTXIFG));            // USCI_A3 TX buffer ready?
    UCA2TXBUF = SendData;                  // TX -> RXed character
}


/******************************************************************************
  函数(模块)名称:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  功能:	多字节发送函数
  输入参数: 要发送的数据缓冲区地址，长度
  输出参数: 成功或失败         	 		   		 
  其它说明: 若当前已经有任务在发送或传入的参数有问题，则返回0
*******************************************************************************/
unsigned char USCIA2_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    unsigned char DataTemp;
    
    if(!USCIA2_TXQueueBuffer.Status)//确保发送空闲
    {
        if( Lent )              //确保字节数大于0
        {
            USCIA2_TXQueueBuffer.Status |= TXBusy_BIT;   //置发送忙标志位
            for(unsigned int i=0;i<Lent;i++)
            {
                EnQueue( &USCIA2_TXQueueBuffer,*Buffer++ );

            }
            DeQueue( &USCIA2_TXQueueBuffer,&DataTemp);
            USCIA2_ByteSend( DataTemp );                     //发送第一个字节，以启动中断发送
            
            UCA2IE |= UCTXIE;                                // 打开发送中断
            return 1;
        }
        else return 0;
    }
    else return 0;
}






/******************************************************************************
  函数(模块)名称:void USCIA2_ReSetFrameTime(void)
  功能:	帧间隔时间检测开启及检测计数器清0
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 接收到一个字节后，调用这个函数，会把帧时间计数器清0。若末开启，则执行开启动作。
*******************************************************************************/
static void USCIA2_ReSetFrameTime(void)
{
    USCIA2_CountFrame.Status |= FrameTimeEN_BIT;
    if(USCIA2_CountFrame.CountTime==USCIA2FrameTime)
    {
        USCIA2_RXQueueBuffer.FrameCnt++;              //断帧次数加1
    }
    USCIA2_CountFrame.CountTime = 0;              //清空计数
}

/******************************************************************************
  函数(模块)名称:void USCIA2_CountFrameTime(void)
  功能:	帧时间检测计数器
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 执行，则对帧时间进行计数。
*******************************************************************************/
void USCIA2_CountFrameTime(void)
{
    if(USCIA2_CountFrame.Status & FrameTimeEN_BIT)
    {
        USCIA2_CountFrame.CountTime++; //计数;
        if(USCIA2_CountFrame.CountTime==USCIA2FrameTime)
        {
            USCIA2_RXQueueBuffer.FrameCnt++;              //断帧次数加1
            USCIA2_CountFrame.Status &= ~FrameTimeEN_BIT; //关掉帧检测计时
            USCIA2_CountFrame.CountTime = 0;              //清空计数
        }
    }
}


/******************************************************************************
  函数(模块)名称:unsigned char USCIA2_ReadRXBuffStatus(void)
  功能:	读接收缓冲区状态寄存器
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:接收缓冲区操作函数
*******************************************************************************/
unsigned char USCIA2_ReadRXBuffStatus(void)
{
    return USCIA2_RXQueueBuffer.Status;
}
/******************************************************************************
  函数(模块)名称:unsigned int USCIA2_ReadRXBuffFrame(void)
  功能:	读接收缓冲区断帧数
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:接收缓冲区操作函数
*******************************************************************************/
unsigned char USCIA2_ReadRXBuffFrame(void)
{
    return USCIA2_RXQueueBuffer.FrameCnt;
}

/******************************************************************************
  函数(模块)名称:USCIA2_FrameDec(void)
  功能:	断帧次数减1
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:接收缓冲区操作函数
*******************************************************************************/
void USCIA2_FrameDec(void)
{
    if(USCIA2_RXQueueBuffer.FrameCnt)
      USCIA2_RXQueueBuffer.FrameCnt--;
}

/******************************************************************************
  函数(模块)名称:unsigned int USCIA2_ReadRXBuffSize(void)
  功能:	读接收缓冲区数据长度
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:接收缓冲区操作函数
*******************************************************************************/
unsigned int USCIA2_ReadRXBuffSize(void)
{
    return USCIA2_RXQueueBuffer.DataLent;
}


/******************************************************************************
  函数(模块)名称:unsigned char USCIA2_ReadRXBuffer(unsigned char *ReadData)
  功能:	接收缓冲区输出一个字节
  输入参数:存储指针         	              		
  输出参数:          	 		   		 
  其它说明:接收缓冲区操作函数
*******************************************************************************/
unsigned char USCIA2_ReadRXBuffer(unsigned char *ReadData)
{
    return DeQueue(&USCIA2_RXQueueBuffer,ReadData);
}

/******************************************************************************
  函数(模块)名称:unsigned char USCIA2_ReadBufferOffset(unsigned int Offset)
  功能:	返回指定偏移的数据
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:接收缓冲区操作函数
*******************************************************************************/
unsigned char USCIA2_ReadBufferOffset(unsigned int Offset)
{
    return DeQueueOffset(&USCIA2_RXQueueBuffer,Offset);
}


#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
    unsigned char DataTemp;
    switch(__even_in_range(UCA2IV,4))
    {
        case 0:  // Vector 0 - no interrupt
          break; 
          
        case 2:  // Vector 2 - RXIFG
             if(EnQueue(&USCIA2_RXQueueBuffer,UCA2RXBUF))
             {
                 if( QueueFull(&USCIA2_RXQueueBuffer) )
                 {
                     USCIA2_RXQueueBuffer.Status |= BufferOV_BIT;       //置缓冲区满标志位
                     USCIA2_CountFrame.Status &= ~FrameTimeEN_BIT; //关掉帧检测计时
                     USCIA2_CountFrame.CountTime = 0;              //清空计数               
                 }
                 else             
                 {
                     USCIA2_RXQueueBuffer.Status &= ~BufferOV_BIT;       //置缓冲区满标志位
                     USCIA2_ReSetFrameTime();   //缓冲区没满，继续接收数据，并复位帧检测计数器。
                 }
             }
             else
             {
               USCIA2_CountFrame.Status &= ~FrameTimeEN_BIT;      //关掉帧检测计时
               USCIA2_RXQueueBuffer.Status |= BufferOV_BIT;       //置缓冲区满标志位
             }
          break;
          
        case 4:  // Vector 4 - TXIFG
          if( USCIA2_TXQueueBuffer.DataLent )
          {
              DeQueue( &USCIA2_TXQueueBuffer,&DataTemp);
              UCA2TXBUF = DataTemp;
              
              if( 0 == USCIA2_TXQueueBuffer.DataLent )
              {
                  USCIA2_TXQueueBuffer.Status &= ~TXBusy_BIT;  //发送完毕，则清掉空闲位
                  UCA2IE &= ~UCTXIE;   
              }
          }
          break;
          
        default: 
          break;
    }
}
