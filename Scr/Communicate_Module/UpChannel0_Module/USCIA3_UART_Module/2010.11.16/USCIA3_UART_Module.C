  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |    P10.3/UCA1TXD|------------>
//            |                 | 115200 - 8N1
//            |    P10.4/UCA1RXD|<------------

#include "msp430x54x.h"
#include "USCIA3_UART_Module.H"


static struct USCIA3_DataStruct USCI_A3TXBuffer;
static struct USCIA3_DataStruct USCI_A3RXBuffer;
static struct USCIA3_CountFrameStruct USCI_A3CountFrame;

/******************************************************************************
  函数(模块)名称:void USCIA3_UARTBufferInit(void)
  功能:	缓冲区初始化函数
  输入参数: 无        	              		
  输出参数: 无         	 		   		 
  其它说明: 对发送和接收函数区进行初始化
*******************************************************************************/
void USCIA3_UARTBufferInit(void)
{
  USCI_A3TXBuffer.Status = 0X00;    //缓冲区初始化
  USCI_A3TXBuffer.DataLent = 0X00;
  
  USCI_A3RXBuffer.Status = 0X00;
  USCI_A3RXBuffer.DataLent = 0X00;
  
  USCI_A3CountFrame.Status = 0x00;
  USCI_A3CountFrame.CountTime = 0X00;
}

/******************************************************************************
  函数(模块)名称:void USCIA3_UART_Init(void)
  功能:	时间初始化函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void USCIA3_UART_Init(void)
{
    P10SEL |= BIT4+BIT5;                       // P10.4,5 = USCI_A3 TXD/RXD
    UCA3CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA3CTL1 |= UCSSEL_2;                     // SMCLK
    UCA3BR0 = 9;                              // 1MHz 115200 (see User's Guide)
    UCA3BR1 = 0;                              // 1MHz 115200
    UCA3MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA3_UARTBufferInit();                  //接收、发送缓冲区初始化
    UCA3IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}

/******************************************************************************
  函数(模块)名称:void USCIA3_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  功能:	
  输入参数:    ParityBit 校验位 0表示无校验位，1表示奇校验，2表示偶校验
               DataBit   数据位 0表示8位数据位，1表示7位数据位
               StopBit   停止位 0表示1位停止位，1表示2位停止位
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void USCIA3_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
{
    P3SEL |= BIT4+BIT5;                       // P3.4,5 = USCI_A3 TXD/RXD
    UCA3CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA3CTL1 |= UCSSEL_2;                     // SMCLK
    if(ParityBit==0)
    {
        UCA3CTL0 &= ~UCPEN;                     // 无校验位
    }
    else if(ParityBit==1)
    {
        UCA3CTL0 |= UCPEN;                     // 使能校验位功能
        UCA3CTL0 &= ~UCPAR;                    // 奇校验
    }
    else if(ParityBit==2)
    {
        UCA3CTL0 |= UCPEN+UCPAR;                     // 使能校验位功能 偶校验
    }
    
    if(DataBit==0)
    {
        UCA3CTL0 &= ~UC7BIT;                     // 8位数据位
    }
    else if(DataBit==1)
    {
        UCA3CTL0 |= UC7BIT;                    // 7位数据位
    }
    
    if(StopBit==0)
    {
        UCA3CTL0 &= ~UCSPB;                     // 1位停止位
    }
    else if(StopBit==1)
    {
        UCA3CTL0 |= UCSPB;                    // 2位停止位
    }
    
    UCA3BR0 = 9;                              // 1MHz 115200 (see User's Guide)
    UCA3BR1 = 0;                              // 1MHz 115200
    UCA3MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA3IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}


/******************************************************************************
  函数(模块)名称:void ByteSend_UA3RT(unsigned char SendData)
  功能:	单字节发送函数
  输入参数: 要发送的数据字节
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void USCIA3_ByteSend(unsigned char SendData)
{
    while (!(UCA3IFG&UCTXIFG));            // USCI_A3 TX buffer ready?
    UCA3TXBUF = SendData;                  // TX -> RXed character
}


/******************************************************************************
  函数(模块)名称:unsigned char MultiByteTX_UA3RT(unsigned char *Buffer,unsigned int Lent)
  功能:	多字节发送函数
  输入参数: 要发送的数据缓冲区地址，长度
  输出参数: 成功或失败         	 		   		 
  其它说明: 若当前已经有任务在发送或传入的参数有问题，则返回0
*******************************************************************************/
unsigned char USCIA3_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    if(!USCI_A3TXBuffer.Status)//确保发送空闲
    {
        if(Lent)             //确保字节数大于0
        {
            USCI_A3TXBuffer.Status |= BIT0;       //置忙标志位
            USCI_A3TXBuffer.DataLent = Lent-1;    //取字节数
            for(unsigned int i=0;i<Lent;i++)
              USCI_A3TXBuffer.DataBuffer[i] = *Buffer++;
            USCI_A3TXBuffer.BufferPoint = USCI_A3TXBuffer.DataBuffer;       //取得发送首地址
            USCIA3_ByteSend(*USCI_A3TXBuffer.BufferPoint++);                //发送第一个字节，以启动中断发送
            UCA3IE |= UCTXIE;                                               // 打开发送中断
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



/******************************************************************************
  函数(模块)名称:void USCIA3_ReSetFrameTime(void)
  功能:	帧间隔时间检测开启及检测计数器清0
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 接收到一个字节后，调用这个函数，会把帧时间计数器清0。若末开启，则执行开启动作。
*******************************************************************************/
void USCIA3_ReSetFrameTime(void)
{
    USCI_A3CountFrame.Status |= FrameTimeEN_BIT;
    if(USCI_A3CountFrame.CountTime==FrameTime)
    {
        USCI_A3RXBuffer.Status |= FrameTime_BIT;  //置帧标志位
    }
    USCI_A3CountFrame.CountTime = 0;              //清空计数
}

/******************************************************************************
  函数(模块)名称:void USCIA3_CountFrameTime(void)
  功能:	帧时间检测计数器
  输入参数:无         	              		
  输出参数: 无         	 		   		 
  其它说明: 执行，则对帧时间进行计数。
*******************************************************************************/
void USCIA3_CountFrameTime(void)
{
    if(USCI_A3CountFrame.Status & FrameTimeEN_BIT)
    {
        USCI_A3CountFrame.CountTime++;//计数;
        if(USCI_A3CountFrame.CountTime==FrameTime)
        {
            USCI_A3RXBuffer.Status |= FrameTime_BIT;//置帧标志位
            USCI_A3CountFrame.Status &= ~FrameTimeEN_BIT; //关掉帧检测计时
            USCI_A3CountFrame.CountTime = 0;              //清空计数
        }
    }
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:  // Vector 0 - no interrupt
    break;                             
  case 2:  // Vector 2 - RXIFG
    if(USCI_A3RXBuffer.DataLent < DataBufferMax)       //缓冲区没满
    {
        USCI_A3RXBuffer.DataBuffer[USCI_A3RXBuffer.DataLent++] = UCA3RXBUF;//把接收到的数据放到缓冲区
        if(USCI_A3RXBuffer.DataLent == DataBufferMax) //缓冲区满
        {
            USCI_A3RXBuffer.Status |= BufferOV_BIT;       //置缓冲区满标志位
            USCI_A3CountFrame.Status &= ~FrameTimeEN_BIT; //关掉帧检测计时
            USCI_A3CountFrame.CountTime = 0;              //清空计数
        }
        else
        {
            USCIA3_ReSetFrameTime();                  //缓冲区没满，继续接收数据，并复位帧检测计数器。
        }
    }
    else                                   
    {
        ;                                  //缓冲区满，则再接收到的数据均丢弃
    }
    break;
  case 4:  // Vector 4 - TXIFG
    if(USCI_A3TXBuffer.DataLent)
    {
        USCI_A3TXBuffer.DataLent--;
        UCA3TXBUF = *USCI_A3TXBuffer.BufferPoint++;
    }
    else
    {
        USCI_A3TXBuffer.Status &= ~TXBusy_BIT;  //发送完毕，则清掉空闲位
        UCA3IE &= ~UCTXIE;                      // 关闭发送中断
    }
    break;                                  
  default: 
    break;
  }
}