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
#include "msp430x54x.h"
#include "IIC_Module.H"
#include "RX8025SA_Module.H"


        
/******************************************************************************
  ����(ģ��)����:unsigned char Read_Staus(void)
  ����:	8025SA��״̬����
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: ��ȡ8025SA��״̬��־λ�������־λ�����򷵻�ʧ��
*******************************************************************************/
void Re_Init(void)
{
    unsigned char ReadTemp[2];
    ReadTemp[0]=SET24_BIT;//ѡ��Ϊ24Сʱ��
    ReadTemp[1]=XST_BIT;  //ʱ��λ��1
    IIC_Page_Write(REGE_Addr|BIT0,ReadTemp,2);//д��
}        
        
        
/******************************************************************************
  ����(ģ��)����:unsigned char Read_Staus(void)
  ����:	8025SA��״̬����
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: ��ȡ8025SA��״̬��־λ�������־λ�����򷵻�ʧ��
*******************************************************************************/
unsigned char Read_Staus(void)
{
    unsigned char ReadTemp[2];
    IIC_Multi_read(REGE_Addr,ReadTemp,2);   //��ȡE��F�������ƼĴ��������жϵ�ǰ״̬
    if((ReadTemp[0]&TEST_BIT)||(ReadTemp[1]&PON_BIT)||(ReadTemp[1]&XST_BIT==0))
    {
        return 0;
    }
    else return 1;
}

/******************************************************************************
  ����(ģ��)����:void RX8025S_Init(unsigned char Slave_Addr)
  ����:	8025SA��ʼ������
  �������: 8025SA�Ĵӻ���ַ        	              		
  �������: ��          	 		   		 
  ����˵��: ��ʼ���󣬻���ú�����ȡ״̬��־�Ĵ����������־�����������³�ʼ��.
*******************************************************************************/
void RX8025S_Init(unsigned char Slave_Addr)
{
    IIC_Init(RX8025SA_addr);             //�趨8025SA�ĵ�ַ����ʼ��ͨѶ�ӿڣ�
    if(!Read_Staus())
    {
        Re_Init();    //���³�ʼ��
    }
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
        return IIC_Multi_read(Sec_addr,Time_Buffer,3);    //��ȡʱ��
    }
    else return 0;
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
        return IIC_Multi_read(Day_addr,Date_Buffer,3);    //��ȡ����
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
        return IIC_Multi_read(Sec_addr,DATE_Buffer,7);    //��ȡ
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
        return IIC_Page_Write(Sec_addr|BIT0,DATE_Buffer,7);//��ʱ��д��
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
    return IIC_Page_Write(Sec_addr|BIT0,WriteTemp,3);
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
    return IIC_Page_Write(Day_addr|BIT0,WriteTemp,3);
}


/******************************************************************************
  ����(ģ��)����:unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
  ����:	�����ú���
  �������: BCD���ʽ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
{
    unsigned char ReadTemp[2];
    IIC_Multi_read(REGE_Addr,ReadTemp,2);   //��ȡE��F�������ƼĴ��������жϵ�ǰ״̬
    if(ReadTemp[0]&DALE_BIT)
    {
        ReadTemp[0]&=~DALE_BIT;//��ǰ���ӹ����Ѿ����������ȹرգ��ٽ����趨
        IIC_Single_Write(REGE_Addr|BIT0,ReadTemp[0]);
    }
    IIC_Single_Write(AlarmD0_addr|BIT0,AlarmMinute);//�趨����ʱ��
    IIC_Single_Write(AlarmD1_addr|BIT0,AlarmHour);
    
    ReadTemp[0] |= DALE_BIT;//�������ӹ���
    IIC_Page_Write(REGE_Addr|BIT0,ReadTemp,2);//д��
    IIC_Multi_read(REGE_Addr,ReadTemp,2);   //��ȡE��F�������ƼĴ��������жϵ�ǰ״̬
}