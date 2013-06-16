#ifndef  __USCIA3_UART_Module_H
#define  __USCIA3_UART_Module_H

#define  DataBufferMax    128 //��������С
#define  FrameTime        2   //����Ϊ���ֽ�֮��Ľ�����������������Ϊ֡������ÿ����ʱ���ɶ�ʱ�����ơ�

//���͡��������ݻ�����
struct USCIA3_DataStruct        
{
    unsigned char Status;                    //������״̬�Ĵ�˳
    unsigned int DataLent;                   //��ǰ���ݳ���
    unsigned char *BufferPoint;              //ָ�������ջ�����͵�����
    unsigned char DataBuffer[DataBufferMax]; //���黺��
}; 
//Status�Ĵ�����λ�궨��
#define  TXBusy_BIT         BIT0  //���Ϳ���λ
#define  BufferOV_BIT       BIT1  //����������־λ
#define  FrameTime_BIT      BIT2  //֡ʱ���־λ

//����֡��ʱ��
struct USCIA3_CountFrameStruct
{
    unsigned char Status;   //֡ʱ����״̬λ
    unsigned int CountTime; //֡ʱ���������
};
//Status�Ĵ�����λ�궨��
#define  FrameTimeEN_BIT    BIT0  //֡ʱ����ʹ��λ




/******************************************************************************
  ����(ģ��)����:void USCIA3_UART_Init(void)
  ����:	ʱ���ʼ������
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA3_UART_Init(void);

/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA3RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA3_MultiByteTX(unsigned char *Buffer,unsigned int Lent);

/******************************************************************************
  ����(ģ��)����:void USCIA3_CountFrameTime(void)
  ����:	֡ʱ���������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�У����֡ʱ����м�����
*******************************************************************************/
void USCIA3_CountFrameTime(void);



#endif