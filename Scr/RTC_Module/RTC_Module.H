#ifndef  __RTC_Module_H
#define  __RTC_Module_H


unsigned char GetWeekDay(unsigned char year,unsigned char month,unsigned char day);

/******************************************************************************
  ����(ģ��)����:void RTC_Init(unsigned char Time_Mod)
  ����:	ʱ���ʼ������
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void RTC_Init(unsigned char Time_Mod);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadTime(unsigned char *TimeBuffer)
  ����:	ʱ���ȡ����
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadTime(unsigned char *TimeBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDate(unsigned char *DateBuffer)
  ����:	���ڶ�ȡ����
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDate(unsigned char *DateBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadRTC(unsigned char *DateBuffer)
  ����:	��ʵʱʱ�䣬ssmmhhDDMMYYYY
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadRTC(unsigned char *DateBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char SetRTC(unsigned char *DateBuffer)
  ����:	дʵʱʱ�䣬ssmmhhDDMMYYYY
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetRTC(unsigned char *DateBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char SetTime(unsigned char Second,unsigned char Minute,unsigned char Hour)
  ����:	ʱ�����ú���
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetTime(unsigned char Second,unsigned char Minute,unsigned char Hour);

/******************************************************************************
  ����(ģ��)����:unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year)
  ����:	�����ú���
  �������: BCD���ʽ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent)
  ����:	��ָ�����ȵ����ݳ���
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent);

/******************************************************************************
  ����(ģ��)����:unsigned char EnAlarm(void)
  ����:	���ӹ���ʹ��
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char EnAlarm(void);

/******************************************************************************
  ����(ģ��)����:unsigned char DiaAlarm(void)
  ����:	���ӹ��ܽ�ֹ
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char DiaAlarm(void);

/******************************************************************************
  ����(ģ��)����:unsigned char Clearn_AlarmFlag(void)
  ����:	�����ӱ�־λ
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ClearnAlarmFlag(void);

/******************************************************************************
  ����(ģ��)����:unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
  ����:	����ʱ���趨
  �������: BCD���ʽ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadRTCFlag(void)
  ����:	��RTC��־λ
  �������: ��        	              		
  �������: RTC��־λ�Ĵ���          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadRTCFlag(void);

/******************************************************************************
  ����(ģ��)����:void WriteRTCFlag(unsigned char RtcFlag)
  ����:	дRTC��־λ
  �������: RTC��־�Ĵ���        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
void WriteRTCFlag(unsigned char RtcFlag);

#endif