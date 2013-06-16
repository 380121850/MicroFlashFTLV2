#ifndef  __RX8025SA_Module_H
#define  __RX8025SA_Module_H

#define RX8025SA_addr 0x64   //8025SA�ĵ�ַ

#define Sec_addr      0x00   //��Ĵ�����ַ
#define Minu_addr     0x10   //��
#define Hour_addr     0x20   //ʱ

#define Week_addr     0x30   //����(00~06)
#define Day_addr      0x40   //��
#define Mon_addr      0x50   //��
#define Year_addr     0x60   //��

#define Offset_addr   0x70   //ƫ���������Ĵ���
#define AlarmW0_addr  0x80   //��ʱ���÷ּĴ���
#define AlarmW1_addr  0x90   //��ʱ����ʱ�Ĵ���
#define AlarmW2_addr  0xA0   //��ʱ�������ڼĴ���

#define AlarmD0_addr  0xB0   //��ʱ���÷ּĴ���
#define AlarmD1_addr  0xC0   //��ʱ����ʱ�Ĵ���

#define Reser_addr    0xD0   //�����Ĵ���
#define REGE_Addr     0xE0   //���ƼĴ���1
        #define WALE_BIT     BIT7   //WALEλ�����ڡ�ʱ���ֱ����趨λ
        #define DALE_BIT     BIT6   //DALEλ��ʱ���ֱ�����λ
        #define SET24_BIT    BIT5   //12Сʱ�ƻ�24Сʱ��
        #define TEST_BIT     BIT3   //����λ����һλ��Ӧд��1
        #define CT2_BIT      BIT2   //�ж�����ģʽѡ��
        #define CT1_BIT      BIT1
        #define CT0_BIT      BIT0
     

#define REGF_Addr     0xF0          //���ƼĴ���2
        #define VDSL_BIT      BIT7   //
        #define VDET_BIT      BIT6   //
        #define XST_BIT       BIT5     //
        #define PON_BIT       BIT4    //
        #define CTFG_BIT      BIT2  //
        #define WAFG_BIT      BIT1
        #define DAFG_BIT      BIT0

/******************************************************************************
  ����(ģ��)����:void RX8025S_Init(unsigned char Slave_Addr)
  ����:	8025SA��ʼ������
  �������: 8025SA�Ĵӻ���ַ        	              		
  �������: ��          	 		   		 
  ����˵��: ��ʼ���󣬻���ú�����ȡ״̬��־�Ĵ����������־�����������³�ʼ��.
*******************************************************************************/
void RX8025S_Init(unsigned char Slave_Addr);


/******************************************************************************
  ����(ģ��)����:unsigned char Read_Time(unsigned char *Time_Buffer)
  ����:	��ȫ��ʱ�䡢���ں���
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_Time(unsigned char *Time_Buffer);

/******************************************************************************
  ����(ģ��)����:unsigned char Read_Time(unsigned char *Time_Buffer)
  ����:	��ȫ��ʱ�䡢���ں���
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_Date(unsigned char *Date_Buffer);

/******************************************************************************
  ����(ģ��)����:unsigned char Read_AllTime(unsigned char *DATE_Buffer)
  ����:	��ȫ��ʱ�䡢���ں���
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_AllTime(unsigned char *DATE_Buffer);

/******************************************************************************
  ����(ģ��)����:void Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour)
  ����:	ʱ�����ú���
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour);

/******************************************************************************
  ����(ģ��)����:unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year)
  ����:	ʱ�����ú���
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year);

/******************************************************************************
  ����(ģ��)����:unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
  ����:	�����ú���
  �������: BCD���ʽ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour);


#endif