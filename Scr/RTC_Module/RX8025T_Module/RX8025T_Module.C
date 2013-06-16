//************************************************************************
//
//                 MSP430F5XX
//             -----------------
//         /|\|              XIN|-  
//          | |                 |    ------
//          --|RST          XOUT|-  |  8   |
//            |                 |   |  0   |
//            |        SCL P10.1|-->|  2   |
//            |                 |   |  5   |
//            |        SDA P10.2|-->|  S   |
//                                  |      |
//                                  |______|
//  Lierda, Inc                      IIC 
//            
//************************************************************************
#include "stdio.h"
#include "msp430x54x.h"
#include "IIC_Module.H"
#include "RX8025T_Module.H"

/******************************************************************************
  ����(ģ��)����:unsigned char GetWeekDay(unsigned char year,unsigned char month,unsigned char day)
  ����:	���� 2000-2099 ������һ�������ڼ�
  �������:
          year : 00-99 
          month: 01-12 
          day  : 01-31              		
  �������: ���ڼ�         	 		   		 
  ����˵��: ��������0��ʾ
*******************************************************************************/
unsigned char GetWeekDay(unsigned char year,unsigned char month,unsigned char day)  
{
    const unsigned char WeekData[]={0,3,3,6,1,4,6,2,5,0,3,5};
    return ((6+WeekData[month-1]+year+day+year/4)%7); 
} 
        
/******************************************************************************
  ����(ģ��)����:unsigned char Re_Init(void)
  ����:	
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: ��RTC����Ϊ24Сʱ��
*******************************************************************************/
unsigned char Re_Init(void)
{
  unsigned char const REGBuffer[16] = {0x00,0x00,0x00,//0��0��0ʱ��ʼ �롢�֡�ʱ
                                 0x01,          //����
                                 0x27,0x04,0x11,//�ա��¡���
                                 0x00,          //RAM
                                 0x00,0x00,0x00,//���ӷ֡�ʱ����
                                 0x00,0x00,     //������
                                 0x00,          //��չ�Ĵ���
                                 0x00,          //��־λ�Ĵ���
                                 0x40 };        //���ƼĴ���
    return IIC_MultiWrite(Sec_addr,(unsigned char *)REGBuffer,16);
}        
        
        
/******************************************************************************
  ����(ģ��)����:unsigned char Read_Staus(void)
  ����:	8025T��״̬����,�жϵ͵�ѹλ��־λ
  �������: ��        	              		
  �������: 1���͵�ѹλ������0���͵�ѹλ����λ����Ĵ�������          	 		   		 
  ����˵��: ��ȡ8025��״̬��־λ�������־λ�����򷵻�ʧ��
*******************************************************************************/
unsigned char Read_Staus(void)
{
    unsigned char Flag_REGData;  
    if(IIC_SingleRead(Flag_Addr,&Flag_REGData))//��ȡE��־λ�Ĵ��������жϵ�ǰ״̬
    {
        if((Flag_REGData & VLF_BIT))
        {
            return 0;
        }
        else return 1;
    }
    else return 0;//��״̬�Ĵ�������
}

/******************************************************************************
  ����(ģ��)����:unsigned char RX8025T_Init(unsigned char Slave_Addr)
  ����:	8025SA��ʼ������
  �������: 8025SA�Ĵӻ���ַ        	              		
  �������: ��          	 		   		 
  ����˵��: ��ʼ���󣬻���ú�����ȡ״̬��־�Ĵ����������־�����������³�ʼ��.
*******************************************************************************/
unsigned char RX8025T_Init(unsigned char Slave_Addr)
{
    IIC_Init(RX8025T_addr);             //�趨8025T�ĵ�ַ����ʼ��ͨѶ�ӿڣ�
    if(!Read_Staus())
    {
        return Re_Init();    //���³�ʼ��
    }
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char Read_Time(unsigned char *Time_Buffer)
  ����:	��ȫ��ʱ�䡢���ں���
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_Time(unsigned char *Time_Buffer)
{
    if(Read_Staus())
    {
        return IIC_MultiRead(Sec_addr,Time_Buffer,3);    //��ȡʱ��
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent)
  ����:	��ȫ���Ĵ���
  �������:         	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent)
{
    return IIC_MultiRead(Sec_addr,Time_Buffer,16);    //��ȡʱ��
}

/******************************************************************************
  ����(ģ��)����:unsigned char Read_Time(unsigned char *Time_Buffer)
  ����:	��ȫ��ʱ�䡢���ں���
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_Date(unsigned char *Date_Buffer)
{
    if(Read_Staus())
    {
        return IIC_MultiRead(Day_addr,Date_Buffer,3);    //��ȡ����
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char Read_AllTime(unsigned char *DATE_Buffer)
  ����:	��ȫ��ʱ�䡢���ں���
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_AllTime(unsigned char *DATE_Buffer)
{
    if(Read_Staus())
    {
        return IIC_MultiRead(Sec_addr,DATE_Buffer,7);    //��ȡ
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char Write_AllTime(unsigned char *DATE_Buffer)
  ����:	��ȫ��ʱ��д��
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Write_AllTime(unsigned char *DATE_Buffer)
{
    if(Read_Staus())
    {
        return IIC_MultiWrite(Sec_addr,DATE_Buffer,7);//��ʱ��д��
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:void Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour)
  ����:	ʱ�����ú���
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour)
{
    unsigned char WriteTemp[3];
    WriteTemp[0]=Second;
    WriteTemp[1]=Minute;
    WriteTemp[2]=Hour;
    return IIC_MultiWrite(Sec_addr,WriteTemp,3);
}

/******************************************************************************
  ����(ģ��)����:unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year)
  ����:	ʱ�����ú���
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year)
{
    unsigned char WriteTemp[3];
    WriteTemp[0]=Day;
    WriteTemp[1]=Mon;
    WriteTemp[2]=Year;
    return IIC_MultiWrite(Day_addr,WriteTemp,3);
}

/******************************************************************************
  ����(ģ��)����:unsigned char En_Alarm(void)
  ����:	�����ӹ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Dia_Alarm(void)
{
    return IIC_SingleWrite(CTL_Addr,0x40);
}

/******************************************************************************
  ����(ģ��)����:unsigned char En_Alarm(void)
  ����:	�����ӹ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char En_Alarm(void)
{
    return IIC_SingleWrite(CTL_Addr,0x48);
}

/******************************************************************************
  ����(ģ��)����:unsigned char Clearn_AlarmFlag(void)
  ����:	�����ӱ�־λ
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Clearn_AlarmFlag(void)
{
    unsigned char ReadTemp;
    if( IIC_SingleRead(Flag_Addr,&ReadTemp) )
    {
        return IIC_SingleWrite(Flag_Addr,ReadTemp & (~AF_BIT) ); 
    }
    else return 0;   
}

/******************************************************************************
  ����(ģ��)����:unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
  ����:	�����ú���
  �������: BCD���ʽ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
{
    unsigned char WriteTemp[3];
    WriteTemp[0]=AlarmMinute;
    WriteTemp[1]=AlarmHour;
    WriteTemp[2]=AlarmDay;
    return IIC_MultiWrite(AlarmW0_addr,WriteTemp,3);
}