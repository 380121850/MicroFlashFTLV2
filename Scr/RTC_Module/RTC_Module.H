#ifndef  __RTC_Module_H
#define  __RTC_Module_H


unsigned char GetWeekDay(unsigned char year,unsigned char month,unsigned char day);

/******************************************************************************
  函数(模块)名称:void RTC_Init(unsigned char Time_Mod)
  功能:	时间初始化函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void RTC_Init(unsigned char Time_Mod);

/******************************************************************************
  函数(模块)名称:unsigned char ReadTime(unsigned char *TimeBuffer)
  功能:	时间读取函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadTime(unsigned char *TimeBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char ReadDate(unsigned char *DateBuffer)
  功能:	日期读取函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadDate(unsigned char *DateBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char ReadRTC(unsigned char *DateBuffer)
  功能:	读实时时间，ssmmhhDDMMYYYY
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadRTC(unsigned char *DateBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char SetRTC(unsigned char *DateBuffer)
  功能:	写实时时间，ssmmhhDDMMYYYY
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetRTC(unsigned char *DateBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char SetTime(unsigned char Second,unsigned char Minute,unsigned char Hour)
  功能:	时间设置函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetTime(unsigned char Second,unsigned char Minute,unsigned char Hour);

/******************************************************************************
  函数(模块)名称:unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year)
  功能:	日设置函数
  输入参数: BCD码格式        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year);


/******************************************************************************
  函数(模块)名称:unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent)
  功能:	读指定长度的数据出来
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent);

/******************************************************************************
  函数(模块)名称:unsigned char EnAlarm(void)
  功能:	闹钟功能使能
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char EnAlarm(void);

/******************************************************************************
  函数(模块)名称:unsigned char DiaAlarm(void)
  功能:	闹钟功能禁止
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char DiaAlarm(void);

/******************************************************************************
  函数(模块)名称:unsigned char Clearn_AlarmFlag(void)
  功能:	清闹钟标志位
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ClearnAlarmFlag(void);

/******************************************************************************
  函数(模块)名称:unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
  功能:	闹钟时间设定
  输入参数: BCD码格式        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay);


/******************************************************************************
  函数(模块)名称:unsigned char ReadRTCFlag(void)
  功能:	读RTC标志位
  输入参数: 无        	              		
  输出参数: RTC标志位寄存器          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadRTCFlag(void);

/******************************************************************************
  函数(模块)名称:void WriteRTCFlag(unsigned char RtcFlag)
  功能:	写RTC标志位
  输入参数: RTC标志寄存器        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
void WriteRTCFlag(unsigned char RtcFlag);

#endif