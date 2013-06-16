  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P5.6/UCA1TXD|------------>
//            |                 | 115200 - 8N1       RS232C
//            |     P5.7/UCA1RXD|<------------

#include "msp430x54x.h"
#include "USCIA1_UART_Module.H"

static USCA1CirQueueStruct USCIA1_TXQueueBuffer;             //���ͻ������
static USCA1CirQueueStruct USCIA1_RXQueueBuffer;             //���ջ������

static struct USCIA1_CountFrameStruct USCIA1_CountFrame;     //����֡ʱ����
                                                             //�ṹ��





/******************************************************************************
  ����(ģ��)����:static void InitQueue(USCA1CirQueueStruct *Q)
  ����:	�öӿ�,��ʼ������
  �������: ����ָ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
static void InitQueue(USCA1CirQueueStruct *Q)
{
    Q->Queue_Front = 0;
    Q->Queue_rear = 0;
    Q->DataLent = 0;
}

/******************************************************************************
  ����(ģ��)����:static unsigned char QueueFull(USCA1CirQueueStruct *Q)
  ����:	�ж���
  �������: ����ָ��       	              		
  �������: 1��������0����û��         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char QueueFull(USCA1CirQueueStruct *Q)
{
    if( USCA1QueueBufferSize == Q->DataLent ) 
      return 1;
    else  
      return 0;
}


/******************************************************************************
  ����(ģ��)����:static unsigned char EnQueue(USCA1CirQueueStruct *Q,unsigned char DataByte)
  ����:	���
  �������: ����ָ�룬�ֽ�����       	              		
  �������: 1��ӣ�0�����Ѿ����������޷����         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char EnQueue(USCA1CirQueueStruct *Q,unsigned char DataByte)
{
    if( QueueFull(Q) )
    {
       return 0;
    }
    else
    {
        Q->DataLent++;
        Q->DataBuffer[Q->Queue_rear] = DataByte;
        Q->Queue_rear = (Q->Queue_rear+1)%USCA1QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:static unsigned char DeQueue(USCA1CirQueueStruct *Q,unsigned char *ReadData)
  ����:	����
  �������: ����ָ�룬�ֽ�����ָ��       	              		
  �������: 1���ӣ�0�����Ѿ��գ������ݳ���         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char DeQueue(USCA1CirQueueStruct *Q,unsigned char *ReadData)
{
    if(0 == Q->DataLent)
    {
        return 0;
    }
    else
    {
        *ReadData = Q->DataBuffer[Q->Queue_Front];
        Q->DataLent--;
        Q->Queue_Front = (Q->Queue_Front+1)%USCA1QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:static unsigned char DeQueueOffset(USCA1CirQueueStruct *Q,unsigned int Offset)
  ����:	���ص�ǰ��ͷָ��ƫ�Ƶ�����
  �������:       	              		
  �������:         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char DeQueueOffset(USCA1CirQueueStruct *Q,unsigned int Offset)
{
    Offset = (Q->Queue_Front+Offset)%USCA1QueueBufferSize;
    return Q->DataBuffer[Offset];
}




/******************************************************************************
  ����(ģ��)����:void USCIA1_UARTBufferInit(void)
  ����:	��������ʼ������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: �Է��ͺͽ��պ��������г�ʼ��
*******************************************************************************/
static void USCIA1_UARTBufferInit(void)
{
    USCIA1_TXQueueBuffer.Status = 0X00;    //���ͻ�������ʼ��
    USCIA1_TXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA1_TXQueueBuffer);
    
    USCIA1_RXQueueBuffer.Status = 0X00;    //���ջ�������ʼ��
    USCIA1_RXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA1_RXQueueBuffer);
    
    USCIA1_CountFrame.Status = 0x00;    //��֡���Ĵ�����ʼ��
    USCIA1_CountFrame.CountTime = 0X00;
}


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
                      unsigned char DataBit,unsigned char StopBit)
{
    P5SEL |= BIT6+BIT7;                       // P5.6,7 = USCIA1 TXD/RXD
    UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_2;                     // SMCLK
    
//У��λѡ�� 0��У�� 1��У�� 2żУ��
    if(ParityBit==0)
    {
        UCA1CTL0 &= ~UCPEN;                     // ��У��λ
    }
    else if(ParityBit==1)
    {
        UCA1CTL0 |= UCPEN;                     // ʹ��У��λ����
        UCA1CTL0 &= ~UCPAR;                    // ��У��
    }
    else if(ParityBit==2)
    {
        UCA1CTL0 |= UCPEN+UCPAR;                     // ʹ��У��λ���� żУ��
    }
    
//����λѡ�� 7 ���ݳ�����7λ 8 ���ݳ�����8λ    
    if(DataBit==8)
    {
        UCA1CTL0 &= ~UC7BIT;                     // 8λ����λ
    }
    else if(DataBit==7)
    {
        UCA1CTL0 |= UC7BIT;                    // 7λ����λ
    }

//ֹͣλѡ�� 1 1λֹͣλ 2 2λֹͣλ    
    if(StopBit==1)
    {
        UCA1CTL0 &= ~UCSPB;                     // 1λֹͣλ
    }
    else if(StopBit==2)
    {
        UCA1CTL0 |= UCSPB;                    // 2λֹͣλ
    }
    
//������ѡ�� Ĭ��ʱ��Ƶ��ΪDCO 1048578HZ
    UCA1BRW = 8000000/BaudRate;
    //UCA1BR0 = 109;                            // 1MHz 115200 (see User's Guide)
    //UCA1BR1 = 0;                              // 1MHz 115200
    
    UCA1MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA1_UARTBufferInit();                  //���ա����ͻ�������ʼ��
    UCA1IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}


/******************************************************************************
  ����(ģ��)����:void USCIA1_ByteSend(unsigned char SendData)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵������ֽ�
  �������:          	 		   		 
  ����˵��: static 
*******************************************************************************/
void USCIA1_ByteSend(unsigned char SendData)
{
    while (!(UCA1IFG&UCTXIFG));            // USCI_A3 TX buffer ready?
    UCA1TXBUF = SendData;                  // TX -> RXed character
}

/**/
int putchar (int c)
{
    unsigned int WaitPoling=0;
    while(USCIA1_TXQueueBuffer.Status)//ȷ�����Ϳ���
    { 
      WaitPoling++;
      if(WaitPoling>8000) return -1;
    }
    if (c=='\n')
    {
          while ((UCA1IFG & UCTXIFG) == 0);        // USART0 TX buffer ready?
          UCA1TXBUF = 0x0a;
    }
    else
    {
          while ((UCA1IFG & UCTXIFG) == 0);        // USART0 TX buffer ready?
          UCA1TXBUF = c;                           // char to TXBUF0
    }
    return c;
}

/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA1_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    unsigned char DataTemp;
    
    if(!USCIA1_TXQueueBuffer.Status)//ȷ�����Ϳ���
    {
        if( Lent )              //ȷ���ֽ�������0
        {
            USCIA1_TXQueueBuffer.Status |= TXBusy_BIT;   //�÷���æ��־λ
            for(unsigned int i=0;i<Lent;i++)
            {
                EnQueue( &USCIA1_TXQueueBuffer,*Buffer++ );
            }
            DeQueue( &USCIA1_TXQueueBuffer,&DataTemp);
            USCIA1_ByteSend( DataTemp );                     //���͵�һ���ֽڣ��������жϷ���
            
            UCA1IE |= UCTXIE;                                // �򿪷����ж�
            return 1;
        }
        else return 0;
    }
    else return 0;
}






/******************************************************************************
  ����(ģ��)����:void USCIA1_ReSetFrameTime(void)
  ����:	֡���ʱ���⿪��������������0
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ���յ�һ���ֽں󣬵���������������֡ʱ���������0����ĩ��������ִ�п���������
*******************************************************************************/
static void USCIA1_ReSetFrameTime(void)
{
    USCIA1_CountFrame.Status |= FrameTimeEN_BIT;
    if(USCIA1_CountFrame.CountTime==USCIA1_FrameTime)
    {
        USCIA1_RXQueueBuffer.FrameCnt++;              //��֡������1
    }
    USCIA1_CountFrame.CountTime = 0;              //��ռ���
}

/******************************************************************************
  ����(ģ��)����:void USCIA1_CountFrameTime(void)
  ����:	֡ʱ���������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�У����֡ʱ����м�����
*******************************************************************************/
void USCIA1_CountFrameTime(void)
{
    if(USCIA1_CountFrame.Status & FrameTimeEN_BIT)
    {
        USCIA1_CountFrame.CountTime++; //����;
        if(USCIA1_CountFrame.CountTime==USCIA1_FrameTime)
        {
            USCIA1_RXQueueBuffer.FrameCnt++;              //��֡������1
            USCIA1_CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
            USCIA1_CountFrame.CountTime = 0;              //��ռ���
        }
    }
}


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA1_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA1_ReadRXBuffStatus(void)
{
    return USCIA1_RXQueueBuffer.Status;
}
/******************************************************************************
  ����(ģ��)����:unsigned int USCIA1_ReadRXBuffFrame(void)
  ����:	�����ջ�������֡��
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA1_ReadRXBuffFrame(void)
{
    return USCIA1_RXQueueBuffer.FrameCnt;
}

/******************************************************************************
  ����(ģ��)����:USCIA1_FrameDec(void)
  ����:	��֡������1
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
void USCIA1_FrameDec(void)
{
    if(USCIA1_RXQueueBuffer.FrameCnt)
      USCIA1_RXQueueBuffer.FrameCnt--;
}

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA1_ReadRXBuffSize(void)
  ����:	�����ջ��������ݳ���
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned int USCIA1_ReadRXBuffSize(void)
{
    return USCIA1_RXQueueBuffer.DataLent;
}


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA1_ReadRXBuffer(unsigned char *ReadData)
  ����:	���ջ��������һ���ֽ�
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA1_ReadRXBuffer(unsigned char *ReadData)
{
    return DeQueue(&USCIA1_RXQueueBuffer,ReadData);
}

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA1_ReadBufferOffset(unsigned int Offset)
  ����:	����ָ��ƫ�Ƶ�����
  �������:         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA1_ReadBufferOffset(unsigned int Offset)
{
    return DeQueueOffset(&USCIA1_RXQueueBuffer,Offset);
}


// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    unsigned char DataTemp;
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:  // Vector 0 - no interrupt
    break;                             
  case 2:  // Vector 2 - RXIFG
       if(EnQueue(&USCIA1_RXQueueBuffer,UCA1RXBUF))
       {
           if( QueueFull(&USCIA1_RXQueueBuffer) )
           {
               USCIA1_RXQueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
               USCIA1_CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
               USCIA1_CountFrame.CountTime = 0;              //��ռ���               
           }
           else             
           {
               USCIA1_RXQueueBuffer.Status &= ~BufferOV_BIT;       //�û���������־λ
               USCIA1_ReSetFrameTime();   //������û���������������ݣ�����λ֡����������
           }
       }
       else
       {
         USCIA1_CountFrame.Status &= ~FrameTimeEN_BIT;      //�ص�֡����ʱ
         USCIA1_RXQueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
       }
    break;
  case 4:  // Vector 4 - TXIFG
    if( USCIA1_TXQueueBuffer.DataLent )
    {
        DeQueue( &USCIA1_TXQueueBuffer,&DataTemp);
        UCA1TXBUF = DataTemp;
        
        if( 0 == USCIA1_TXQueueBuffer.DataLent )
        {
            USCIA1_TXQueueBuffer.Status &= ~TXBusy_BIT;  //������ϣ����������λ
            UCA1IE &= ~UCTXIE;   
        }
    }
    break;                                  
  default: 
    break;
  }
}