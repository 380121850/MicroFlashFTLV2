  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include ".\DownChannel0_Module\USCIA0_UART_Module\USCIA0_UART_Module.H"//下行通道M-Bus
#include ".\UpChannel1_Module\USCIA1_UART_Module\USCIA1_UART_Module.H"  //上行通道RS232C
#include ".\UpChannel0_Module\USCIA3_UART_Module\USCIA3_UART_Module.H"  //上行通道GPRS DTU
#include "TimerB0_Module.H"

/******************************************************************************
  函数(模块)名称:void TimerB0Init(void)
  功能:通讯用定时器初始化	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:使用增计数模式，使用SMCLK时钟源，中断间隔大约为2.38ms。
*******************************************************************************/
void TimerB0Init(void)
{
  TBCCTL0 = CCIE;                         // CCR0 interrupt enabled
  TBCCR0 = 20000;                         // 大约2.38ms
  TBCTL = TBSSEL_2 + MC_1 + TBCLR;        // SMCLK, UPmode, clear TBR
}



/******************************************************************************
  函数(模块)名称:__interrupt void TIMERB0_ISR (void)
  功能:通讯用定时器中断函数	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:使用增计数模式，使用SMCLK时钟源，中断间隔大约为2.38ms。
*******************************************************************************/
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR (void)
{
/*  */
    static unsigned int TimeCnt=0;
    TimeCnt++;
    if(TimeCnt>10000)
    {
        USCIA3_MultiByteTX((unsigned char *)&TimeCnt,sizeof(TimeCnt));
        TimeCnt = 0;
    }
    
    USCIA0_CountFrameTime();           //USCI_A0接收帧间隔检测计数器
    USCIA0_FrameAckTime_CountTime();   //USCI_A0发送帧间隔检测计数器
    
    USCIA1_CountFrameTime();           //USCI_A1接收帧间隔检测计数器
    USCIA3_CountFrameTime();           //USCI_A3接收帧间隔检测计数器
}