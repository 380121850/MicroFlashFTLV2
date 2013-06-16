  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P10.4/UCA3TXD|------------>
//            |                 | 115200 - 8N1       GPRS DTU
//            |     P10.5/UCA3RXD|<------------

#include "msp430x54x.h"
#include "USCIA3_UART_Module.H"

static USCA3CirQueueStruct USCIA3_TXQueueBuffer;             //���ͻ������
static USCA3CirQueueStruct USCIA3_RXQueueBuffer;             //���ջ������

static struct USCIA3_CountFrameStruct USCIA3_CountFrame;     //����֡ʱ����
                                                             //�ṹ��


/******************************************************************************
  ����(ģ��)����:void InitQueue(USCA3CirQueueStruct *Q)
  ����:	�öӿ�,��ʼ������
  �������: ����ָ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
static void InitQueue(USCA3CirQueueStruct *Q)
{
    Q->Queue_Front = 0;
    Q->Queue_rear = 0;
    Q->DataLent = 0;
}

/******************************************************************************
  ����(ģ��)����:static unsigned char QueueFull(USCA3CirQueueStruct *Q)
  ����:	�ж���
  �������: ����ָ��       	              		
  �������: 1��������0����û��         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char QueueFull(USCA3CirQueueStruct *Q)
{
    if( USCA3QueueBufferSize == Q->DataLent ) 
      return 1;
    else  
      return 0;
}


/******************************************************************************
  ����(ģ��)����:static unsigned char EnQueue(USCA3CirQueueStruct *Q,unsigned char DataByte)
  ����:	���
  �������: ����ָ�룬�ֽ�����       	              		
  �������: 1��ӣ�0�����Ѿ����������޷����         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char EnQueue(USCA3CirQueueStruct *Q,unsigned char DataByte)
{
    if( QueueFull(Q) )
    {
       return 0;
    }
    else
    {
        Q->DataLent++;
        Q->DataBuffer[Q->Queue_rear] = DataByte;
        Q->Queue_rear = (Q->Queue_rear+1)%USCA3QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:static unsigned char DeQueue(USCA3CirQueueStruct *Q,unsigned char *ReadData)
  ����:	����
  �������: ����ָ�룬�ֽ�����ָ��       	              		
  �������: 1���ӣ�0�����Ѿ��գ������ݳ���         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char DeQueue(USCA3CirQueueStruct *Q,unsigned char *ReadData)
{
    if(0 == Q->DataLent)
    {
        return 0;
    }
    else
    {
        *ReadData = Q->DataBuffer[Q->Queue_Front];
        Q->DataLent--;
        Q->Queue_Front = (Q->Queue_Front+1)%USCA3QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:static unsigned char DeQueueOffset(USCA3CirQueueStruct *Q,unsigned int Offset)
  ����:	���ص�ǰ��ͷָ��ƫ�Ƶ�����
  �������:       	              		
  �������:         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char DeQueueOffset(USCA3CirQueueStruct *Q,unsigned int Offset)
{
    Offset = (Q->Queue_Front+Offset)%USCA3QueueBufferSize;
    return Q->DataBuffer[Offset];
}




/******************************************************************************
  ����(ģ��)����:void USCIA3_UARTBufferInit(void)
  ����:	��������ʼ������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: �Է��ͺͽ��պ��������г�ʼ��
*******************************************************************************/
static void USCIA3_UARTBufferInit(void)
{
    USCIA3_TXQueueBuffer.Status = 0X00;    //���ͻ�������ʼ��
    USCIA3_TXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA3_TXQueueBuffer);
    
    USCIA3_RXQueueBuffer.Status = 0X00;    //���ջ�������ʼ��
    USCIA3_RXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA3_RXQueueBuffer);
    
    USCIA3_CountFrame.Status = 0x00;    //��֡���Ĵ�����ʼ��
    USCIA3_CountFrame.CountTime = 0X00;
}


/******************************************************************************
  ����(ģ��)����:void USCIA3_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  ����:	
  �������:    ParityBit У��λ 0��ʾ��У��λ��1��ʾ��У�飬2��ʾżУ��
               DataBit   ����λ 8��ʾ8λ����λ��7��ʾ7λ����λ
               StopBit   ֹͣλ 1��ʾ1λֹͣλ��2��ʾ2λֹͣλ
  �������:          	 		   		 
  ����˵��: 4ϵ�е�Ĭ����1.048576MHz
*******************************************************************************/
void USCIA3_UARTSet(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
{
    P10SEL |= BIT4+BIT5;                       // P10.4,5 = USCIA3 TXD/RXD
    UCA3CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA3CTL1 |= UCSSEL_2;                     // SMCLK
    
//У��λѡ�� 0��У�� 1��У�� 2żУ��
    if(ParityBit==0)
    {
        UCA3CTL0 &= ~UCPEN;                     // ��У��λ
    }
    else if(ParityBit==1)
    {
        UCA3CTL0 |= UCPEN;                     // ʹ��У��λ����
        UCA3CTL0 &= ~UCPAR;                    // ��У��
    }
    else if(ParityBit==2)
    {
        UCA3CTL0 |= UCPEN+UCPAR;                     // ʹ��У��λ���� żУ��
    }
    
//����λѡ�� 7 ���ݳ�����7λ 8 ���ݳ�����8λ    
    if(DataBit==8)
    {
        UCA3CTL0 &= ~UC7BIT;                     // 8λ����λ
    }
    else if(DataBit==7)
    {
        UCA3CTL0 |= UC7BIT;                    // 7λ����λ
    }

//ֹͣλѡ�� 1 1λֹͣλ 2 2λֹͣλ    
    if(StopBit==1)
    {
        UCA3CTL0 &= ~UCSPB;                     // 1λֹͣλ
    }
    else if(StopBit==2)
    {
        UCA3CTL0 |= UCSPB;                    // 2λֹͣλ
    }
    
//������ѡ�� Ĭ��ʱ��Ƶ��ΪDCO 1048578HZ
    UCA3BRW = 8000000/BaudRate;
    //UCA3BR0 = 109;                            // 1MHz 115200 (see User's Guide)
    //UCA3BR1 = 0;                              // 1MHz 115200
    
    UCA3MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA3_UARTBufferInit();                  //���ա����ͻ�������ʼ��
    UCA3IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}

/*
int putchar (int c)
{
    unsigned int WaitPoling=0;
    while(USCIA3_TXQueueBuffer.Status)//ȷ�����Ϳ���
    { 
      WaitPoling++;
      if(WaitPoling>8000) return -1;
    }
    if (c=='\n')
    {
          while ((UCA3IFG & UCTXIFG) == 0);        // USART0 TX buffer ready?
          UCA3TXBUF = 0x0a;
    }
    else
    {
          while ((UCA3IFG & UCTXIFG) == 0);        // USART0 TX buffer ready?
          UCA3TXBUF = c;                           // char to TXBUF0
    }
    return c;
}
*/

/******************************************************************************
  ����(ģ��)����:void USCIA3_ByteSend(unsigned char SendData)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵������ֽ�
  �������:          	 		   		 
  ����˵��: static 
*******************************************************************************/
void USCIA3_ByteSend(unsigned char SendData)
{
    while (!(UCA3IFG&UCTXIFG));            // USCI_A3 TX buffer ready?
    UCA3TXBUF = SendData;                  // TX -> RXed character
}


/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA3_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    unsigned char DataTemp;
    
    if(!USCIA3_TXQueueBuffer.Status)//ȷ�����Ϳ���
    {
        if( Lent )              //ȷ���ֽ�������0
        {
            USCIA3_TXQueueBuffer.Status |= TXBusy_BIT;   //�÷���æ��־λ
            for(unsigned int i=0;i<Lent;i++)
            {
                EnQueue( &USCIA3_TXQueueBuffer,*Buffer++ );
            }
            DeQueue( &USCIA3_TXQueueBuffer,&DataTemp);
            USCIA3_ByteSend( DataTemp );                     //���͵�һ���ֽڣ��������жϷ���
            
            UCA3IE |= UCTXIE;                                // �򿪷����ж�
            return 1;
        }
        else return 0;
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:void USCIA3_ReSetFrameTime(void)
  ����:	֡���ʱ���⿪��������������0
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ���յ�һ���ֽں󣬵���������������֡ʱ���������0����ĩ��������ִ�п���������
*******************************************************************************/
static void USCIA3_ReSetFrameTime(void)
{
    USCIA3_CountFrame.Status |= FrameTimeEN_BIT;
    if(USCIA3_CountFrame.CountTime==USCIA3_FrameTime)
    {
        USCIA3_RXQueueBuffer.FrameCnt++;              //��֡������1
    }
    USCIA3_CountFrame.CountTime = 0;                 //��ռ���
}

/******************************************************************************
  ����(ģ��)����:void USCIA3_CountFrameTime(void)
  ����:	֡ʱ���������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�У����֡ʱ����м�����
*******************************************************************************/
void USCIA3_CountFrameTime(void)
{
    if(USCIA3_CountFrame.Status & FrameTimeEN_BIT)
    {
        USCIA3_CountFrame.CountTime++; //����;
        if(USCIA3_CountFrame.CountTime==USCIA3_FrameTime)
        {
            USCIA3_RXQueueBuffer.FrameCnt++;              //��֡������1
            USCIA3_CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
            USCIA3_CountFrame.CountTime = 0;              //��ռ���
        }
    }
}


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA3_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA3_ReadRXBuffStatus(void)
{
    return USCIA3_RXQueueBuffer.Status;
}
/******************************************************************************
  ����(ģ��)����:unsigned int USCIA3_ReadRXBuffFrame(void)
  ����:	�����ջ�������֡��
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA3_ReadRXBuffFrame(void)
{
    return USCIA3_RXQueueBuffer.FrameCnt;
}

/******************************************************************************
  ����(ģ��)����:USCIA3_FrameDec(void)
  ����:	��֡������1
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
void USCIA3_FrameDec(void)
{
    if(USCIA3_RXQueueBuffer.FrameCnt)
      USCIA3_RXQueueBuffer.FrameCnt--;
}

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA3_ReadRXBuffSize(void)
  ����:	�����ջ��������ݳ���
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned int USCIA3_ReadRXBuffSize(void)
{
    return USCIA3_RXQueueBuffer.DataLent;
}


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA3_ReadRXBuffer(unsigned char *ReadData)
  ����:	���ջ��������һ���ֽ�
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA3_ReadRXBuffer(unsigned char *ReadData)
{
    return DeQueue(&USCIA3_RXQueueBuffer,ReadData);
}

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA3_ReadBufferOffset(unsigned int Offset)
  ����:	����ָ��ƫ�Ƶ�����
  �������:         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA3_ReadBufferOffset(unsigned int Offset)
{
    return DeQueueOffset(&USCIA3_RXQueueBuffer,Offset);
}


// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
    unsigned char DataTemp;
    switch(__even_in_range(UCA3IV,4))
    {
      case 0:  // Vector 0 - no interrupt
        break;                             
      case 2:  // Vector 2 - RXIFG
           if(EnQueue(&USCIA3_RXQueueBuffer,UCA3RXBUF))
           {
               if( QueueFull(&USCIA3_RXQueueBuffer) )
               {
                   USCIA3_RXQueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
                   USCIA3_CountFrame.Status &= ~FrameTimeEN_BIT;      //�ص�֡����ʱ
                   USCIA3_CountFrame.CountTime = 0;                   //��ռ���               
               }
               else             
               {
                   USCIA3_RXQueueBuffer.Status &= ~BufferOV_BIT;       //�û���������־λ
                   USCIA3_ReSetFrameTime();                            //������û���������������ݣ�����λ֡����������
               }
           }
           else
           {
               USCIA3_CountFrame.Status &= ~FrameTimeEN_BIT;      //�ص�֡����ʱ
               USCIA3_RXQueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
           }
        break;
        
      case 4:  // Vector 4 - TXIFG
        if( USCIA3_TXQueueBuffer.DataLent )
        {
            DeQueue( &USCIA3_TXQueueBuffer,&DataTemp);
            UCA3TXBUF = DataTemp;
            
            if( 0 == USCIA3_TXQueueBuffer.DataLent )
            {
                USCIA3_TXQueueBuffer.Status &= ~TXBusy_BIT;  //������ϣ����������λ
                UCA3IE &= ~UCTXIE;   
            }
        }
        break; 
        
      default: 
        break;
    }
}