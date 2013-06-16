//************************************************************************
//  Lierda, Inc 
//  �ϵ�󣬳�ʼ��ʱ��оƬ����ͨѶ�����ʱ��оƬ�ڲ���־�Ĵ������������ó�
//  RTC�����־λ��
//************************************************************************
#include "msp430x54x.h"
#include ".\RX8025T_Module\RX8025T_Module.H"
#include "RTC_Module.H"

//BIT0 RTC���ӱ�־λ
//BIT1 �����־λ
//BIT2 ��־λ
//BIT3 ��־λ
//BIT4 ��־λ
//BIT5 ��־λ
//BIT6 ��־λ
//BIT7 RTC�����־λ ʱ��оƬֹͣ����ʱ��оƬͨѶ����

unsigned char RTCFlag = 0x00;//RTC��־λ


/******************************************************************************
  ����(ģ��)����:unsigned char ReadRTCFlag(void)
  ����:	��RTC��־λ
  �������: ��        	              		
  �������: RTC��־λ�Ĵ���          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadRTCFlag(void)
{
    return RTCFlag;
}

/******************************************************************************
  ����(ģ��)����:void WriteRTCFlag(unsigned char RtcFlag)
  ����:	дRTC��־λ
  �������: RTC��־�Ĵ���        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
void WriteRTCFlag(unsigned char RtcFlag)
{
   RTCFlag = RtcFlag;
}

/******************************************************************************
  ����(ģ��)����:void RTCINT_Init(void)
  ����:	RTC�����жϺ���
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
void RTCINT_Init(void)
{     
    P2DIR &= ~(BIT2 + BIT0);                     //P2.2 ����Ϊ����  
    P2OUT |= (BIT2 + BIT0);
    P2REN |= (BIT2 + BIT0) ;                     //������������ʹ��
    P2IES |= (BIT2 + BIT0);                      //P2.2�½�����
    P2IE  |= (BIT2 + BIT0);                      //P2.2�ж�ʹ��
    P2IFG =0;                           //�жϱ�־�������ֹ���ù����г����жϱ�־Ϊ1
}

/******************************************************************************
  ����(ģ��)����:void RTC_Init(unsigned char Time_Mod)
  ����:	ʱ���ʼ������
  �������: 24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void RTC_Init(unsigned char Time_Mod)
{
    RX8025T_Init(0X64);  //ʱ��оƬ��ʼ��
    RTCINT_Init();       //ʱ�������жϹ��ܳ�ʼ��
    DiaAlarm();
    ClearnAlarmFlag();
    if( 0 == Read_Staus() )
    {
        WriteRTCFlag( ReadRTCFlag()|BIT7 );    //RTCʱ��оƬ���󣬵���ֹͣ��ͨѶ����
    }
    else
    {
        WriteRTCFlag( ReadRTCFlag()&(~BIT7) ); //�������λ
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadTime(unsigned char *TimeBuffer)
  ����:	ʱ���ȡ����
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadTime(unsigned char *TimeBuffer)
{
    return Read_Time(TimeBuffer);
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadDate(unsigned char *DateBuffer)
  ����:	���ڶ�ȡ����
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDate(unsigned char *DateBuffer)
{
    return Read_Date(DateBuffer);
}


/******************************************************************************
  ����(ģ��)����:unsigned char SetTime(unsigned char Second,unsigned char Minute,unsigned char Hour)
  ����:	ʱ�����ú���
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
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
  ����(ģ��)����:unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year)
  ����:	�����ú���
  �������: BCD���ʽ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetDate(unsigned char Day,unsigned char Mon,unsigned char Year)
{
    return Set_Date(Day,Mon,Year);
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadRTC(unsigned char *DateBuffer)
  ����:	��ʵʱʱ�䣬ssmmhhDDMMYYYY
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
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
  ����(ģ��)����:unsigned char SetRTC(unsigned char *DateBuffer)
  ����:	дʵʱʱ�䣬ssmmhhDDMMYYYY
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
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
  ����(ģ��)����:unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent)
  ����:	��ָ�����ȵ����ݳ���
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadLent(unsigned char *Time_Buffer,unsigned int Lent)
{
    return Read_Lent(Time_Buffer,Lent);
}

/******************************************************************************
  ����(ģ��)����:unsigned char EnAlarm(void)
  ����:	���ӹ���ʹ��
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char EnAlarm(void)
{
    return En_Alarm();
}

/******************************************************************************
  ����(ģ��)����:unsigned char DiaAlarm(void)
  ����:	���ӹ��ܽ�ֹ
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char DiaAlarm(void)
{
    return Dia_Alarm();
}

/******************************************************************************
  ����(ģ��)����:unsigned char Clearn_AlarmFlag(void)
  ����:	�����ӱ�־λ
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ClearnAlarmFlag(void)
{ 
    return Clearn_AlarmFlag();
}


/******************************************************************************
  ����(ģ��)����:unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
  ����:	����ʱ���趨
  �������: BCD���ʽ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetAlarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
{
    EnAlarm();
    ClearnAlarmFlag();
    return Set_Alarm(AlarmMinute,AlarmHour,AlarmDay);
}



/*******************�˿��жϳ���********************/

#pragma vector = PORT2_VECTOR
__interrupt void port2(void)
{
     switch(P2IV)
    {
      case 0x02: 
        RTCFlag |= BIT1;     //�õ����־λ
        break;
        
      case 0x06: 
        ClearnAlarmFlag();  //��RTC��־λ
        RTCFlag |= BIT0;
        break;
      default  :break;
    }
}