#ifndef  __USCIA1_UART_Module_H
#define  __USCIA1_UART_Module_H


//���͡��������ݻ������ṹ��
#define USCA1QueueBufferSize  255
typedef struct 
{
    unsigned char Status;                       //������״̬�Ĵ�˳
    unsigned char FrameCnt;                     //��֡��
    
    unsigned int  Queue_Front;                  //ͷָ�룬�ӷǿ�ʱָ���ͷԪ��
    unsigned int  Queue_rear;                   //βָ�룬�ӷǿ�ʱָ���βԪ�ص���һλ��
    unsigned int  DataLent;                     //��������
    unsigned char DataBuffer[USCA1QueueBufferSize];  //���黺��  
}USCA1CirQueueStruct;
//Status�Ĵ�����λ�궨��
#define  TXBusy_BIT         BIT0  //���Ϳ���λ
#define  BufferOV_BIT       BIT1  //����������־λ
#define  FrameTime_BIT      BIT2  //֡ʱ���־λ


#define  USCIA1_FrameTime        8   //����Ϊ���ֽ�֮��Ľ�����������������Ϊ֡������ÿ����ʱ���ɶ�ʱ�����ơ�
//����֡��ʱ��
struct USCIA1_CountFrameStruct
{
    unsigned char Status;   //֡ʱ����״̬λ
    unsigned int CountTime; //֡ʱ���������
};
//Status�Ĵ�����λ�궨��
#define  FrameTimeEN_BIT    BIT0  //֡ʱ����ʹ��λ


/******************************************************************************
  ����(ģ��)����:void USCIA1_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  ����:	
  �������:    ParityBit У��λ 0��ʾ��У��λ��1��ʾ��У�飬2��ʾżУ��
               DataBit   ����λ 8��ʾ8λ����λ��7��ʾ7λ����λ
               StopBit   ֹͣλ 1��ʾ1λֹͣλ��2��ʾ2λֹͣλ
  �������:          	 		   		 
  ����˵��: 4ϵ�е�Ĭ����1.048576MHz
*******************************************************************************/
void USCIA1_UARTSet(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit);

/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA1_MultiByteTX(unsigned char *Buffer,unsigned int Lent);

/******************************************************************************
  ����(ģ��)����:void USCIA1_CountFrameTime(void)
  ����:	֡ʱ���������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�У����֡ʱ����м�����
*******************************************************************************/
void USCIA1_CountFrameTime(void);


/******************************************************************************
  ����(ģ��)����:USCIA1_FrameDec(void)
  ����:	��֡������1
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
void USCIA1_FrameDec(void);

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA1_ReadRXBuffSize(void)
  ����:	�����ջ��������ݳ���
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned int USCIA1_ReadRXBuffSize(void);

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA1_ReadRXBuffer(unsigned char *ReadData)
  ����:	���ջ��������һ���ֽ�
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA1_ReadRXBuffer(unsigned char *ReadData);

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA1_ReadBufferOffset(unsigned int Offset)
  ����:	����ָ��ƫ�Ƶ�����
  �������:         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA1_ReadBufferOffset(unsigned int Offset);

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA1_ReadRXBuffFrame(void)
  ����:	�����ջ�������֡��
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA1_ReadRXBuffFrame(void);

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA1_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA1_ReadRXBuffStatus(void);




#endif