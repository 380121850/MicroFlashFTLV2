#ifndef  __RX8025T_Module_H
#define  __RX8025T_Module_H

#define RX8025T_addr 0x64   //8025SA�ĵ�ַ

#define Sec_addr      0x00   //��Ĵ�����ַ
#define Minu_addr     0x01   //��
#define Hour_addr     0x02   //ʱ

#define Week_addr     0x03   //����(00~06)
#define Day_addr      0x04   //��
#define Mon_addr      0x05   //��
#define Year_addr     0x06   //��

#define RAM_addr      0x07   //�����д
#define AlarmW0_addr  0x08   //��ʱ���÷ּĴ���
#define AlarmW1_addr  0x09   //��ʱ����ʱ�Ĵ���
#define AlarmW2_addr  0x0A   //��ʱ�������ڼĴ���

#define TimerCounter0_addr  0x0B   //��ʱ������0
#define TimerCounter1_addr  0x0C   //��ʱ������1

#define Extern_addr    0x0D   //��չ�Ĵ���
        #define TEST_BIT       BIT7   //
        #define WADA_BIT       BIT6   //
        #define USEL_BIT       BIT5   //ʱ����±�־λ
        #define TE_BIT         BIT4   //�̶����ڶ�ʱ�жϱ�־λ
        #define FSEL1_BIT      BIT3   //�����жϱ�־Ȭ
        #define FSEL0_BIT      BIT2
        #define TESL1_BIT      BIT1
        #define TESL0_BIT      BIT0

#define Flag_Addr      0x0E    //��־�Ĵ���1
        //#define _BIT       BIT7   //
        //#define _BIT       BIT6   //
        #define UF_BIT       BIT5   //ʱ����±�־λ
        #define TF_BIT       BIT4   //�̶����ڶ�ʱ�жϱ�־λ
        #define AF_BIT       BIT3   //�����жϱ�־Ȭ
        //#define _BIT      BIT2
        #define VLF_BIT       BIT1
        #define VDET_BIT      BIT0
     

#define CTL_Addr     0x0F          //���ƼĴ���2
        #define CSEL1_BIT      BIT7   //
        #define CSEL0_BIT      BIT6   //
        #define UIE_BIT        BIT5     //
        #define TIE_BIT        BIT4    //
        #define AIE_BIT        BIT3  //
        //#define _BIT      BIT2
        //#define _BIT      BIT1
        #define REST_BIT      BIT0

/******************************************************************************
  ����(ģ��)����:unsigned char Read_Staus(void)
  ����:	8025T��״̬����,�жϵ͵�ѹλ��־λ
  �������: ��        	              		
  �������: 1���͵�ѹλ������0���͵�ѹλ����λ����Ĵ�������          	 		   		 
  ����˵��: ��ȡ8025��״̬��־λ�������־λ�����򷵻�ʧ��
*******************************************************************************/
unsigned char Read_Staus(void);

/******************************************************************************
  ����(ģ��)����:void RX8025S_Init(unsigned char Slave_Addr)
  ����:	8025SA��ʼ������
  �������: 8025SA�Ĵӻ���ַ        	              		
  �������: ��          	 		   		 
  ����˵��: ��ʼ���󣬻���ú�����ȡ״̬��־�Ĵ����������־�����������³�ʼ��.
*******************************************************************************/
unsigned char RX8025T_Init(unsigned char Slave_Addr);


/******************************************************************************
  ����(ģ��)����:unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent)
  ����:	��ȫ���Ĵ���
  �������:         	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent);

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
  ����(ģ��)����:unsigned char Write_AllTime(unsigned char *DATE_Buffer)
  ����:	��ȫ��ʱ��д��
  �������: ����7���ֽڴ�С�Ļ������׵�ַ        	              		
  �������: 1����ȡ�ɹ���0����ȡʧ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Write_AllTime(unsigned char *DATE_Buffer);

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
unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay);

unsigned char En_Alarm(void);
/******************************************************************************
  ����(ģ��)����:unsigned char En_Alarm(void)
  ����:	�����ӹ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Dia_Alarm(void);

/******************************************************************************
  ����(ģ��)����:unsigned char Clearn_AlarmFlag(void)
  ����:	�����ӱ�־λ
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Clearn_AlarmFlag(void);
#endif