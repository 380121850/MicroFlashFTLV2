//************************************************************************
//  Lierda, Inc 
//  上电后，初始化时钟芯片，若通讯错误或时钟芯片内部标志寄存器出错，则置出
//  RTC错误标志位。
//************************************************************************
#include "msp430x54x.h"
#include ".\RX8025T_Module\RX8025T_Module.H"
#include "RTC_Module.H"

//BIT0 RTC闹钟标志位
//BIT1 掉电标志位
//BIT2 标志位
//BIT3 标志位
//BIT4 标志位
//BIT5 标志位
//BIT6 标志位
//BIT7 RTC错误标志位 时钟芯片停止或与时钟芯片通讯错误

unsigned char RTCFlag = 0x00;//RTC标志位


/******************************************************************************
  函数(模块)名称:unsigned char ReadRTCFlag(void)
  功能:	读RTC标志位
  输入参数: 无        	              		
  输出参数: RTC标志位寄存器          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadRTCFlag(void)
{
    return RTCFlag;
}

/******************************************************************************
  函数(模块)名称:void WriteRTCFlag(unsigned char RtcFlag)
  功能:	写RTC标志位
  输入参数: RTC标志寄存器        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
void WriteRTCFlag(unsigned char RtcFlag)
{
   RTCFlag = RtcFlag;
}

/******************************************************************************
  函数(模块)名称:void RTCINT_Init(void)
  功能:	RTC闹钟中断函数
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
void RTCINT_Init(void)
{     
    P2DIR &= ~(BIT2 + BIT0);                     //P2.2 设置为输入  
    P2OUT |= (BIT2 + BIT0);
    P2REN |= (BIT2 + BIT0) ;                     //上拉下拉电阻使能
    P2IES |= (BIT2 + BIT0);                      //P2.2下降触发
    P2IE  |= (BIT2 + BIT0);                      //P2.2中断使能
    P2IFG =0;                           //中断标志清除，防止配置过程中出现中断标志为1
}

/******************************************************************************
  函数(模块)名称:void RTC_Init(unsigned char Time_Mod)
  功能:	时间初始化函数
  输入参数: 24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void RTC_Init(unsigned char Time_Mod)
{
    RX8025T_Init(0X64);  //时钟芯片初始化
    RTCINT_Init();       //时钟闹钟中断功能初始化
    DiaAlarm();
    ClearnAlarmFlag();
    if( 0 == Read_Staus() )
    {
        WriteRTCFlag( ReadRTCFlag()|BIT7 );    //RTC时钟芯片错误，掉电停止或通讯错误
    }
    else
    {
        WriteRTCFlag( ReadRTCFlag()&(~BIT7) ); //清掉错误位
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadTime(unsigned char *TimeBuffer)
  功能:	时间读取函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadTime(unsigned char *TimeBuffer)
{
    return Read_Time(TimeBuffer);
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadDate(unsigned char *DateBuffer)
  功能:	日期读取函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadDate(unsigned char *DateBuffer)
{
    return Read_Date(DateBuffer);
}


/******************************************************************************
  函数(模块)名称:unsigned char SetTime(unsigned char Second,unsigned char Minute,unsigned char Hour)
  功能:	时间设置函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetTime(unsigned char Second,unsigned char Minute,unsigned char Hour)
{
    if(((Second&0x0F)<0x0A)&&((Second&0xF0)<0x60))
    {
        if(((Minute&0x0F)<0x0A)&&((Minute&0xF0)<0x60))
        {
            if(((Hour&0x0F)<0x04)&&((Hour&0xF0)<0x30))
            {
                return Set_Time(Second,Minute,Hour);
            }
        }
    }
    return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year)
  功能:	日设置函数
  输入参数: BCD码格式        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year)
{
    return Set_Date(Day,Mon,Year);
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadRTC(unsigned char *DateBuffer)
  功能:	读实时时间，ssmmhhDDMMYYYY
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadRTC(unsigned char *DateBuffer)
{
    if( Read_AllTime(DateBuffer) )
    {
        DateBuffer[3] = DateBuffer[4];
        DateBuffer[4] = DateBuffer[5];
        DateBuffer[5] = DateBuffer[6];
        DateBuffer[6] = 0x20;
        return 1;
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char SetRTC(unsigned char *DateBuffer)
  功能:	写实时时间，ssmmhhDDMMYYYY
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetRTC(unsigned char *DateBuffer)
{
    DateBuffer[6] = DateBuffer[5];
    DateBuffer[5] = DateBuffer[4];
    DateBuffer[4] = DateBuffer[3];
    DateBuffer[3] = 0x01;
    return Write_AllTime( DateBuffer );
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent)
  功能:	读指定长度的数据出来
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent)
{
    return Read_Lent(Time_Buffer,Lent);
}

/******************************************************************************
  函数(模块)名称:unsigned char EnAlarm(void)
  功能:	闹钟功能使能
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char EnAlarm(void)
{
    return En_Alarm();
}

/******************************************************************************
  函数(模块)名称:unsigned char DiaAlarm(void)
  功能:	闹钟功能禁止
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char DiaAlarm(void)
{
    return Dia_Alarm();
}

/******************************************************************************
  函数(模块)名称:unsigned char Clearn_AlarmFlag(void)
  功能:	清闹钟标志位
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ClearnAlarmFlag(void)
{ 
    return Clearn_AlarmFlag();
}


/******************************************************************************
  函数(模块)名称:unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
  功能:	闹钟时间设定
  输入参数: BCD码格式        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
{
    EnAlarm();
    ClearnAlarmFlag();
    return Set_Alarm(AlarmMinute,AlarmHour,AlarmDay);
}



/*******************端口中断程序********************/

#pragma vector = PORT2_VECTOR
__interrupt void port2(void)
{
     switch(P2IV)
    {
      case 0x02: 
        RTCFlag |= BIT1;     //置掉电标志位
        break;
        
      case 0x06: 
        ClearnAlarmFlag();  //清RTC标志位
        RTCFlag |= BIT0;
        break;
      default  :break;
    }
}
