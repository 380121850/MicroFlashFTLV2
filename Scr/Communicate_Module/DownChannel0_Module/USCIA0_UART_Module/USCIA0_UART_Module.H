#ifndef  __USCIA0_UART_Module_H
#define  __USCIA0_UART_Module_H

#define  DataBufferMax    128  //��������С
#define  FrameTime        100   //����Ϊ���ֽ�֮��Ľ�����������������Ϊ֡������ÿ����ʱ���ɶ�ʱ�����ơ�

//���͡��������ݻ������ṹ��
struct USCI_DataStruct        
{
    unsigned char Status;                     //������״̬�Ĵ�˳
    unsigned int  DataLent;                   //��ǰ���ݳ���
    unsigned char *BufferPoint;               //ָ�������ջ�����͵�����
    unsigned char DataBuffer[DataBufferMax];  //���黺��
}; 
//Status�Ĵ�����λ�궨��
#define  TXBusy_BIT         BIT0  //���Ϳ���λ
#define  BufferOV_BIT       BIT1  //����������־λ
#define  FrameTime_BIT      BIT2  //֡ʱ���־λ

#define USCIA0QueueBufferSize  64
typedef struct 
{
    unsigned char Status;                       //������״̬�Ĵ�˳
    unsigned char FrameCnt;                     //��֡��
    
    unsigned int  Queue_Front;                  //ͷָ�룬�ӷǿ�ʱָ���ͷԪ��
    unsigned int  Queue_rear;                   //βָ�룬�ӷǿ�ʱָ���βԪ�ص���һλ��
    unsigned int  DataLent;                     //��������
    unsigned char DataBuffer[ USCIA0QueueBufferSize ];  //���黺��  
}CirQueueStruct;


//����֡��ʱ��
struct USCI_CountFrameStruct
{
    unsigned char Status;   //֡ʱ����״̬λ
    unsigned int CountTime; //֡ʱ���������
};
//Status�Ĵ�����λ�궨��
#define  FrameTimeEN_BIT    BIT0  //֡ʱ����ʹ��λ


/******************************************************************************
  ����(ģ��)����:void USCIA0_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  ����:	
  �������:    ParityBit У��λ 0��ʾ��У��λ��1��ʾ��У�飬2��ʾżУ��
               DataBit   ����λ 8��ʾ8λ����λ��7��ʾ7λ����λ
               StopBit   ֹͣλ 1��ʾ1λֹͣλ��2��ʾ2λֹͣλ
  �������:          	 		   		 
  ����˵��: 4ϵ�е�Ĭ����1.048576MHz
*******************************************************************************/
void USCIA0_UARTSet(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit);


/******************************************************************************
  ����(ģ��)����:void ByteSend_UA0RT(unsigned char SendData)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵������ֽ�
  �������:          	 		   		 
  ����˵��: static 
*******************************************************************************/
void USCIA0_ByteSend(unsigned char SendData);

/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA0_MultiByteTX(unsigned char *Buffer,unsigned int Lent);

/******************************************************************************
  ����(ģ��)����:void USCIA0_CountFrameTime(void)
  ����:	֡ʱ���������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�У����֡ʱ����м�����
*******************************************************************************/
void USCIA0_CountFrameTime(void);


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_ReadRXBuffer(unsigned char *ReadData)
  ����:	���ջ��������һ���ֽ�
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffer(unsigned char *ReadData);


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffStatus(void);

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA0_ReadRXBuffFrame(void)
  ����:	�����ջ�������֡��
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffFrame(void);

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA0_ReadRXBuffSize(void)
  ����:	�����ջ��������ݳ���
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned int USCIA0_ReadRXBuffSize(void);

unsigned char *USCIA0_ReadRXBuffPoint(void);
void USCIA0_FrameDec(void);

unsigned char USCIA0_ReadBufferOffset(unsigned int Offset);

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
void USCIA0_RXBuffClearn(void);



/******************************************************************************
  ����(ģ��)����:void USCIA0_FrameAckTime_CountTime(void)
  ����:	����ͨ�������ʱ����
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:
******************************************************************************/
void USCIA0_FrameAckTime_CountTime(void);

/******************************************************************************
  ����(ģ��)����:void USCIA0_FrameAckTime_ReTryCntDec(void)
  ����:	֡��Ӧ���������1
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA0_FrameAckTime_ReTryCntDec(void);

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_FrameAckTime_ReTryCntRead(void)
  ����:	����ǰ֡��Ӧ�������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char USCIA0_FrameAckTime_ReTryCntRead(void);

/******************************************************************************
  ����(ģ��)����:void USCIA0_FrameAckTime_ReTryCntWrite(unsigned char Cnt)
  ����:	д��ǰ֡��Ӧ�������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA0_FrameAckTime_ReTryCntWrite(unsigned char Cnt);

unsigned int USCIA0_FrameAckTime_TimeCntRead(void);

void USCIA0_FrameAckTime_TimeCntWrite( unsigned int Cnt );
void USCIA0_FrameAckTime_En(unsigned char flag);
void USCIA0_FrameAckTime_SubEn(unsigned char flag);




#endif