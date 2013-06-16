  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P3.4/UCA0TXD|------------>
//            |                 | 115200 - 8N1
//            |     P3.5/UCA0RXD|<------------

#include "msp430x54x.h"
#include "USCIA0_UART_Module.H"

struct FrameAckTimeStruct 
{
    unsigned char Status;
    unsigned int TimeCnt;
    unsigned char ReTryCnt;
};
struct FrameAckTimeStruct USCIA0_FrameAckTime;

struct USCI_DataStruct USCI_A0TXBuffer;      //���ͽӿ�
CirQueueStruct USCI_QueueBuffer;             //���ջ������
static struct USCI_CountFrameStruct USCI_A0CountFrame;//����֡��ʱ

/******************************************************************************
  ����(ģ��)����:void USCIA0_FrameAckTime_ReTryCntDec(void)
  ����:	֡��Ӧ���������1
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA0_FrameAckTime_ReTryCntDec(void)
{
    if( USCIA0_FrameAckTime.ReTryCnt )
      USCIA0_FrameAckTime.ReTryCnt--;
}

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_FrameAckTime_ReTryCntRead(void)
  ����:	����ǰ֡��Ӧ�������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char USCIA0_FrameAckTime_ReTryCntRead(void)
{
    return USCIA0_FrameAckTime.ReTryCnt;
}

/******************************************************************************
  ����(ģ��)����:void USCIA0_FrameAckTime_ReTryCntWrite(unsigned char Cnt)
  ����:	д��ǰ֡��Ӧ�������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA0_FrameAckTime_ReTryCntWrite(unsigned char Cnt)
{
    USCIA0_FrameAckTime.ReTryCnt = Cnt;
}


unsigned int USCIA0_FrameAckTime_TimeCntRead(void)
{
    return USCIA0_FrameAckTime.TimeCnt;
}

void USCIA0_FrameAckTime_TimeCntWrite( unsigned int Cnt )
{
    USCIA0_FrameAckTime.TimeCnt = Cnt;
}




void USCIA0_FrameAckTime_En(unsigned char flag)
{
    if(flag)
      USCIA0_FrameAckTime.Status |= BIT2;   //�򿪼�ʱ�����ܿ���
    else
     USCIA0_FrameAckTime.Status &= ~BIT2;   //�رռ�ʱ�����ܿ��� 
}

void USCIA0_FrameAckTime_SubEn(unsigned char flag)
{
    if(flag)
      USCIA0_FrameAckTime.Status |= BIT1;   //�򿪼�ʱ�����ӿ���
    else
      USCIA0_FrameAckTime.Status &= ~BIT1;   //�رռ�ʱ�����ӿ���
      
}

/******************************************************************************
  ����(ģ��)����:void USCIA0_FrameAckTime_CountTime(void)
  ����:	����ͨ�������ʱ����
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:
******************************************************************************/
void USCIA0_FrameAckTime_CountTime(void)
{
    if( USCIA0_FrameAckTime.Status & BIT2 )   //�򿪼�ʱ�����ܿ���
    {
        if( USCIA0_FrameAckTime.Status & BIT1 )//��ѯ��ʱ����
        {
            if( USCIA0_FrameAckTime.TimeCnt )
            {
                USCIA0_FrameAckTime.TimeCnt--;
            }
        }
    }
}


/******************************************************************************
  ����(ģ��)����:void InitQueue(CirQueueStruct *Q)
  ����:	�öӿ�
  �������: ����ָ��        	              		
  �������: ��         	 		   		 
  ����˵��: ��ʼ�����У���ʱ����Ϊ��
*******************************************************************************/
void InitQueue(CirQueueStruct *Q)
{
    Q->Queue_Front = 0;
    Q->Queue_rear = 0;
    Q->DataLent = 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char QueueFull(CirQueueStruct *Q)
  ����:	�ж���
  �������: ����ָ��       	              		
  �������: 1��������0����û��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char QueueFull(CirQueueStruct *Q)
{
    if( USCIA0QueueBufferSize == Q->DataLent )
      return 1;
    else  return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char EnQueue(CirQueueStruct *Q,unsigned char DataByte)
  ����:	���
  �������: ����ָ�룬�ֽ�����       	              		
  �������: 1��ӣ�0�����Ѿ����������޷����         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char EnQueue(CirQueueStruct *Q,unsigned char DataByte)
{
    if( QueueFull(Q) )
    {
       return 0;
    }
    else
    {
        Q->DataLent++;
        Q->DataBuffer[Q->Queue_rear] = DataByte;
        Q->Queue_rear = (Q->Queue_rear+1)%USCIA0QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char DeQueue(CirQueueStruct *Q,unsigned char *ReadData)
  ����:	����
  �������: ����ָ�룬�ֽ�����ָ��       	              		
  �������: 1���ӣ�0�����Ѿ��գ������ݳ���         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char DeQueue(CirQueueStruct *Q,unsigned char *ReadData)
{
    if(0 == Q->DataLent)
    {
        return 0;
    }
    else
    {
        *ReadData = Q->DataBuffer[Q->Queue_Front];
        Q->DataLent--;
        Q->Queue_Front = (Q->Queue_Front+1)%USCIA0QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char DeQueueOffset(CirQueueStruct *Q,unsigned int Offset)
  ����:	���ص�ǰ��ͷָ��ƫ�Ƶ�����
  �������:       	              		
  �������:         	 		   		 
  ����˵��: ��ƫ����Ϊ1���򷵻ص�ǰ��ͷ����һλ��
*******************************************************************************/
unsigned char DeQueueOffset(CirQueueStruct *Q,unsigned int Offset)
{
    Offset = (Q->Queue_Front+Offset)%USCIA0QueueBufferSize;
    return Q->DataBuffer[Offset];
}

/******************************************************************************
  ����(ģ��)����:unsigned char *GetQueueFront(CirQueueStruct *Q)
  ����:	ȡ��ǰ����ͷ���ݵ�ָ��
  �������: ����ָ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char *GetQueueFront(CirQueueStruct *Q)
{
    return &Q->DataBuffer[Q->Queue_Front];
}

/******************************************************************************
  ����(ģ��)����:unsigned char *GetQueueFront(CirQueueStruct *Q)
  ����:	ȡ��ǰ����ͷ���ݵ�ָ��
  �������: ����ָ��       	              		
  �������:          	 		   		 
  ����˵��: ������ָ�������������ӣ���֮ǰ���ص�ָ�����ʱָ����ܲ�����
*******************************************************************************/
unsigned int GetQueueFront_Ext(CirQueueStruct *Q)
{
    return Q->Queue_Front;
}



/******************************************************************************
  ����(ģ��)����:void USCIA0_UARTBufferInit(void)
  ����:	��������ʼ������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: �Է��ͺͽ��պ��������г�ʼ��
*******************************************************************************/
static void USCIA0_UARTBufferInit(void)
{
  USCI_A0TXBuffer.Status = 0X00;    //��������ʼ��
  USCI_A0TXBuffer.DataLent = 0X00;
  
  USCI_QueueBuffer.Status = 0X00;
  USCI_QueueBuffer.DataLent = 0X00;
  
  USCI_A0CountFrame.Status = 0x00;
  USCI_A0CountFrame.CountTime = 0X00;
}


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
                      unsigned char DataBit,unsigned char StopBit)
{
    P3SEL |= BIT4+BIT5;                       // P3.4,5 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    if(ParityBit==0)
    {
        UCA0CTL0 &= ~UCPEN;                     // ��У��λ
    }
    else if(ParityBit==1)
    {
        UCA0CTL0 |= UCPEN;                     // ʹ��У��λ����
        UCA0CTL0 &= ~UCPAR;                    // ��У��
    }
    else if(ParityBit==2)
    {
        UCA0CTL0 |= UCPEN+UCPAR;                     // ʹ��У��λ���� żУ��
    }
    
    if(DataBit==8)
    {
        UCA0CTL0 &= ~UC7BIT;                     // 8λ����λ
    }
    else if(DataBit==7)
    {
        UCA0CTL0 |= UC7BIT;                    // 7λ����λ
    }
    
    if(StopBit==1)
    {
        UCA0CTL0 &= ~UCSPB;                     // 1λֹͣλ
    }
    else if(StopBit==2)
    {
        UCA0CTL0 |= UCSPB;                    // 2λֹͣλ
    }
    UCA0BRW = 8000000/BaudRate;
    //UCA0BR0 = 109;                            // 1MHz 115200 (see User's Guide)
    //UCA0BR1 = 0;                              // 1MHz 115200
    UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA0_UARTBufferInit();                  //���ա����ͻ�������ʼ��
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}


/******************************************************************************
  ����(ģ��)����:void ByteSend_UA0RT(unsigned char SendData)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵������ֽ�
  �������:          	 		   		 
  ����˵��: static 
*******************************************************************************/
void USCIA0_ByteSend(unsigned char SendData)
{
    while (!(UCA0IFG&UCTXIFG));            // USCI_A0 TX buffer ready?
    UCA0TXBUF = SendData;                  // TX -> RXed character
}


/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA0_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    if(!USCI_A0TXBuffer.Status)//ȷ�����Ϳ���
    {
        if(Lent)             //ȷ���ֽ�������0
        {
            USCI_A0TXBuffer.Status |= BIT0;       //��æ��־λ
            USCI_A0TXBuffer.DataLent = Lent-1;    //ȡ�ֽ���
            for(unsigned int i=0;i<Lent;i++)
            USCI_A0TXBuffer.DataBuffer[i] = *Buffer++;
            USCI_A0TXBuffer.BufferPoint = USCI_A0TXBuffer.DataBuffer;      //ȡ�÷����׵�ַ
            USCIA0_ByteSend(*USCI_A0TXBuffer.BufferPoint++);                //���͵�һ���ֽڣ��������жϷ���
            UCA0IE |= UCTXIE;                                              // �򿪷����ж�
            return 1;
        }
        else return 0;
    }
    else return 0;
}



/******************************************************************************
  ����(ģ��)����:void USCIA0_ReSetFrameTime(void)
  ����:	֡���ʱ���⿪��������������0
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ���յ�һ���ֽں󣬵���������������֡ʱ���������0����ĩ��������ִ�п���������
*******************************************************************************/
static void USCIA0_ReSetFrameTime(void)
{
    USCI_A0CountFrame.Status |= FrameTimeEN_BIT;
    if(USCI_A0CountFrame.CountTime==FrameTime)
    {
        USCI_QueueBuffer.FrameCnt++;              //��֡������1
    }
    USCI_A0CountFrame.CountTime = 0;              //��ռ���
}

/******************************************************************************
  ����(ģ��)����:void USCIA0_CountFrameTime(void)
  ����:	֡ʱ���������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�У����֡ʱ����м�����
*******************************************************************************/
void USCIA0_CountFrameTime(void)
{
    if(USCI_A0CountFrame.Status & FrameTimeEN_BIT)
    {
        USCI_A0CountFrame.CountTime++;//����;
        if(USCI_A0CountFrame.CountTime==FrameTime)
        {
            USCI_QueueBuffer.FrameCnt++;              //��֡������1
            USCI_A0CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
            USCI_A0CountFrame.CountTime = 0;              //��ռ���
        }
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_ReadRXBuffer(unsigned char *ReadData)
  ����:	���ջ��������һ���ֽ�
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffer(unsigned char *ReadData)
{
    return DeQueue(&USCI_QueueBuffer,ReadData);
}

unsigned char USCIA0_ReadBufferOffset(unsigned int Offset)
{
    return DeQueueOffset(&USCI_QueueBuffer,Offset);
}

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffStatus(void)
{
    return USCI_QueueBuffer.Status;
}


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA0_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
void USCIA0_RXBuffClearn(void)
{
    USCI_QueueBuffer.Status = 0X00;
    USCI_QueueBuffer.DataLent = 0X00;
    InitQueue( &USCI_QueueBuffer );
}
/******************************************************************************
  ����(ģ��)����:unsigned int USCIA0_ReadRXBuffFrame(void)
  ����:	�����ջ�������֡��
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char USCIA0_ReadRXBuffFrame(void)
{
    return USCI_QueueBuffer.FrameCnt;
}

void USCIA0_FrameDec(void)
{
    if(USCI_QueueBuffer.FrameCnt)
      USCI_QueueBuffer.FrameCnt--;
}

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA0_ReadRXBuffSize(void)
  ����:	�����ջ��������ݳ���
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned int USCIA0_ReadRXBuffSize(void)
{
    return USCI_QueueBuffer.DataLent;
}


unsigned char *USCIA0_ReadRXBuffPoint(void)
{
    return GetQueueFront(&USCI_QueueBuffer);
}



unsigned char USCIA0_ReadRXBuffQueFront(unsigned int Num)
{
    return GetQueueFront_Ext(&USCI_QueueBuffer);
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:  // Vector 0 - no interrupt
    break;                             
  case 2:  // Vector 2 - RXIFG
    //USCIA1_ByteSend(UCA0RXBUF);
       if(EnQueue(&USCI_QueueBuffer,UCA0RXBUF))
       {
           if( QueueFull(&USCI_QueueBuffer) )
           {
               USCI_QueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
               USCI_A0CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
               USCI_A0CountFrame.CountTime = 0;              //��ռ���               
           }
           else             
           {
               USCI_QueueBuffer.Status &= ~BufferOV_BIT;       //�建��������־λ
               USCIA0_ReSetFrameTime();   //������û���������������ݣ�����λ֡����������
           }
       }
       else
       {
         USCI_A0CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
         USCI_QueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
       }
       if( USCIA0_FrameAckTime.Status & BIT2 )
       USCIA0_FrameAckTime_SubEn(0);//�رռ�ʱ�ӿ���
       
    break;
  case 4:  // Vector 4 - TXIFG
    if(USCI_A0TXBuffer.DataLent)
    {
        USCI_A0TXBuffer.DataLent--;
        UCA0TXBUF = *USCI_A0TXBuffer.BufferPoint++;
        if(USCI_A0TXBuffer.DataLent == 0)
        {
            USCI_A0TXBuffer.Status &= ~TXBusy_BIT;  //������ϣ����������λ
            UCA0IE &= ~UCTXIE; 
            
            if( USCIA0_FrameAckTime.Status & BIT2 )
            USCIA0_FrameAckTime_SubEn(1);//�򿪷��ͼ�ʱ�ӿ���
            //USCIA1_ByteSend(0X55);
        }
    }
    break;                                  
  default: 
    break;
  }
}