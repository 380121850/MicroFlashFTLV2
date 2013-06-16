  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include "..\RTC_Module\RTC_Module.H"             //RTC模块
#include "Sys_Module.H"

unsigned int TimeCnt = 0;

/******************************************************************************
  函数(模块)名称:void TimerB0Init(void)
  功能:通讯用定时器初始化	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:使用增计数模式，使用SMCLK时钟源，中断间隔大约为2.38ms。
*******************************************************************************/
void TimerA0Init(void)
{
    TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
    TA1CCR0 = 100;
    TA1CTL = TASSEL_1 + MC_1 + TACLR;         // ALCK,UPmode, clear TBR
}


/******************************************************************************
  函数(模块)名称:__interrupt void TIMER1_A0_ISR(void)
  功能:上电检测	       	 		   		 
  其它说明:使用增计数模式，使用ALCK时钟源，中断间隔大约为ms。
           连续10次查检到上电状态，则才退出低功耗
*******************************************************************************/
// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    static unsigned int CheckPowerCnt = 0;         //检查掉电次数
    TimeCnt++;
    if( BIT0 == (P2IN & BIT0) )                    //查掉电
    {   //没有掉电
        CheckPowerCnt++;                           //掉电检查计数
        if( CheckPowerCnt>10 )                     //连续10次检查到掉电状态
        {
            WriteRTCFlag( ReadRTCFlag()&(~BIT1) ); //清掉电标志位
            CheckPowerCnt = 0;                     //计数清0          
            LPM3_EXIT;   
        }
    }
    else
    {   //已经掉电
        CheckPowerCnt = 0;                          //计数清0
    }

}
