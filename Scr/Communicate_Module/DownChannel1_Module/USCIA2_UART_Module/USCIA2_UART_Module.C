  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P9.4/UCA2TXD|------------>
//            |                 | 9600 - 8N1
//            |     P9.5/UCA2RXD|<------------

#include "msp430x54x.h"
#include "USCIA2_UART_Module.H"

static USCA2CirQueueStruct USCIA2_TXQueueBuffer;             //���ͻ������
static USCA2CirQueueStruct USCIA2_RXQueueBuffer;             //���ջ������

static struct USCIA2_CountFrameStruct USCIA2_CountFrame;



/******************************************************************************
  ����(ģ��)����:static void InitQueue(USCA2CirQueueStruct *Q)
  ����:	�öӿ�,��ʼ������
  �������: ����ָ��        	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
static void InitQueue(USCA2CirQueueStruct *Q)
{
    Q->Queue_Front = 0;
    Q->Queue_rear = 0;
    Q->DataLent = 0;
}

/******************************************************************************
  ����(ģ��)����:static unsigned char QueueFull(USCA2CirQueueStruct *Q)
  ����:	�ж���
  �������: ����ָ��       	              		
  �������: 1��������0����û��         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char QueueFull(USCA2CirQueueStruct *Q)
{
    if( USCA2QueueBufferSize == Q->DataLent ) 
      return 1;
    else  
      return 0;
}


/******************************************************************************
  ����(ģ��)����:static unsigned char EnQueue(USCA2CirQueueStruct *Q,unsigned char DataByte)
  ����:	���
  �������: ����ָ�룬�ֽ�����       	              		
  �������: 1��ӣ�0�����Ѿ����������޷����         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char EnQueue(USCA2CirQueueStruct *Q,unsigned char DataByte)
{
    if( QueueFull(Q) )
    {
       return 0;
    }
    else
    {
        Q->DataLent++;
        Q->DataBuffer[Q->Queue_rear] = DataByte;
        Q->Queue_rear = (Q->Queue_rear+1)%USCA2QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:static unsigned char DeQueue(USCA2CirQueueStruct *Q,unsigned char *ReadData)
  ����:	����
  �������: ����ָ�룬�ֽ�����ָ��       	              		
  �������: 1���ӣ�0�����Ѿ��գ������ݳ���         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char DeQueue(USCA2CirQueueStruct *Q,unsigned char *ReadData)
{
    if(0 == Q->DataLent)
    {
        return 0;
    }
    else
    {
        *ReadData = Q->DataBuffer[Q->Queue_Front];
        Q->DataLent--;
        Q->Queue_Front = (Q->Queue_Front+1)%USCA2QueueBufferSize;
        return 1;
    }
}

/******************************************************************************
  ����(ģ��)����:static unsigned char DeQueueOffset(USCA2CirQueueStruct *Q,unsigned int Offset)
  ����:	���ص�ǰ��ͷָ��ƫ�Ƶ�����
  �������:       	              		
  �������:         	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char DeQueueOffset(USCA2CirQueueStruct *Q,unsigned int Offset)
{
    Offset = (Q->Queue_Front+Offset)%USCA2QueueBufferSize;
    return Q->DataBuffer[Offset];
}




/******************************************************************************
  ����(ģ��)����:void USCIA2_UARTBufferInit(void)
  ����:	��������ʼ������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: �Է��ͺͽ��պ��������г�ʼ��
*******************************************************************************/
static void USCIA2_UARTBufferInit(void)
{
    USCIA2_TXQueueBuffer.Status = 0X00;    //���ͻ�������ʼ��
    USCIA2_TXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA2_TXQueueBuffer);
    
    USCIA2_RXQueueBuffer.Status = 0X00;    //���ջ�������ʼ��
    USCIA2_RXQueueBuffer.DataLent = 0X00;
    InitQueue(&USCIA2_RXQueueBuffer);
    
    USCIA2_CountFrame.Status = 0x00;    //��֡���Ĵ�����ʼ��
    USCIA2_CountFrame.CountTime = 0X00;
}


/******************************************************************************
  ����(ģ��)����:void USCIA2_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  ����:	
  �������:    ParityBit У��λ 0��ʾ��У��λ��1��ʾ��У�飬2��ʾżУ��
               DataBit   ����λ 8��ʾ8λ����λ��7��ʾ7λ����λ
               StopBit   ֹͣλ 1��ʾ1λֹͣλ��2��ʾ2λֹͣλ
  �������:          	 		   		 
  ����˵��: 4ϵ�е�Ĭ����1.048576MHz
*******************************************************************************/
void USCIA2_UARTSet(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
{
    P9SEL |= BIT4+BIT5;                       // P9.4,5 = USCIA2 TXD/RXD
    UCA2CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA2CTL1 |= UCSSEL_2;                     // SMCLK
    
//У��λѡ�� 0��У�� 1��У�� 2żУ��
    if(ParityBit==0)
    {
        UCA2CTL0 &= ~UCPEN;                     // ��У��λ
    }
    else if(ParityBit==1)
    {
        UCA2CTL0 |= UCPEN;                     // ʹ��У��λ����
        UCA2CTL0 &= ~UCPAR;                    // ��У��
    }
    else if(ParityBit==2)
    {
        UCA2CTL0 |= UCPEN+UCPAR;                     // ʹ��У��λ���� żУ��
    }
    
//����λѡ�� 7 ���ݳ�����7λ 8 ���ݳ�����8λ    
    if(DataBit==8)
    {
        UCA2CTL0 &= ~UC7BIT;                     // 8λ����λ
    }
    else if(DataBit==7)
    {
        UCA2CTL0 |= UC7BIT;                    // 7λ����λ
    }

//ֹͣλѡ�� 1 1λֹͣλ 2 2λֹͣλ    
    if(StopBit==1)
    {
        UCA2CTL0 &= ~UCSPB;                     // 1λֹͣλ
    }
    else if(StopBit==2)
    {
        UCA2CTL0 |= UCSPB;                    // 2λֹͣλ
    }
    
//������ѡ�� Ĭ��ʱ��Ƶ��ΪDCO 1048578HZ
    UCA2BRW = 8000000/BaudRate;
    //UCA2BR0 = 109;                            // 1MHz 115200 (see User's Guide)
    //UCA2BR1 = 0;                              // 1MHz 115200
    
    UCA2MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA2CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA2_UARTBufferInit();                  //���ա����ͻ�������ʼ��
    UCA2IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}


/******************************************************************************
  ����(ģ��)����:void USCIA2_ByteSend(unsigned char SendData)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵������ֽ�
  �������:          	 		   		 
  ����˵��: static 
*******************************************************************************/
void USCIA2_ByteSend(unsigned char SendData)
{
    while (!(UCA2IFG&UCTXIFG));            // USCI_A3 TX buffer ready?
    UCA2TXBUF = SendData;                  // TX -> RXed character
}


/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA0RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA2_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    unsigned char DataTemp;
    
    if(!USCIA2_TXQueueBuffer.Status)//ȷ�����Ϳ���
    {
        if( Lent )              //ȷ���ֽ�������0
        {
            USCIA2_TXQueueBuffer.Status |= TXBusy_BIT;   //�÷���æ��־λ
            for(unsigned int i=0;i<Lent;i++)
            {
                EnQueue( &USCIA2_TXQueueBuffer,*Buffer++ );

            }
            DeQueue( &USCIA2_TXQueueBuffer,&DataTemp);
            USCIA2_ByteSend( DataTemp );                     //���͵�һ���ֽڣ��������жϷ���
            
            UCA2IE |= UCTXIE;                                // �򿪷����ж�
            return 1;
        }
        else return 0;
    }
    else return 0;
}






/******************************************************************************
  ����(ģ��)����:void USCIA2_ReSetFrameTime(void)
  ����:	֡���ʱ���⿪��������������0
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ���յ�һ���ֽں󣬵���������������֡ʱ���������0����ĩ��������ִ�п���������
*******************************************************************************/
static void USCIA2_ReSetFrameTime(void)
{
    USCIA2_CountFrame.Status |= FrameTimeEN_BIT;
    if(USCIA2_CountFrame.CountTime==USCIA2FrameTime)
    {
        USCIA2_RXQueueBuffer.FrameCnt++;              //��֡������1
    }
    USCIA2_CountFrame.CountTime = 0;              //��ռ���
}

/******************************************************************************
  ����(ģ��)����:void USCIA2_CountFrameTime(void)
  ����:	֡ʱ���������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�У����֡ʱ����м�����
*******************************************************************************/
void USCIA2_CountFrameTime(void)
{
    if(USCIA2_CountFrame.Status & FrameTimeEN_BIT)
    {
        USCIA2_CountFrame.CountTime++; //����;
        if(USCIA2_CountFrame.CountTime==USCIA2FrameTime)
        {
            USCIA2_RXQueueBuffer.FrameCnt++;              //��֡������1
            USCIA2_CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
            USCIA2_CountFrame.CountTime = 0;              //��ռ���
        }
    }
}


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA2_ReadRXBuffStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA2_ReadRXBuffStatus(void)
{
    return USCIA2_RXQueueBuffer.Status;
}
/******************************************************************************
  ����(ģ��)����:unsigned int USCIA2_ReadRXBuffFrame(void)
  ����:	�����ջ�������֡��
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA2_ReadRXBuffFrame(void)
{
    return USCIA2_RXQueueBuffer.FrameCnt;
}

/******************************************************************************
  ����(ģ��)����:USCIA2_FrameDec(void)
  ����:	��֡������1
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
void USCIA2_FrameDec(void)
{
    if(USCIA2_RXQueueBuffer.FrameCnt)
      USCIA2_RXQueueBuffer.FrameCnt--;
}

/******************************************************************************
  ����(ģ��)����:unsigned int USCIA2_ReadRXBuffSize(void)
  ����:	�����ջ��������ݳ���
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned int USCIA2_ReadRXBuffSize(void)
{
    return USCIA2_RXQueueBuffer.DataLent;
}


/******************************************************************************
  ����(ģ��)����:unsigned char USCIA2_ReadRXBuffer(unsigned char *ReadData)
  ����:	���ջ��������һ���ֽ�
  �������:�洢ָ��         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA2_ReadRXBuffer(unsigned char *ReadData)
{
    return DeQueue(&USCIA2_RXQueueBuffer,ReadData);
}

/******************************************************************************
  ����(ģ��)����:unsigned char USCIA2_ReadBufferOffset(unsigned int Offset)
  ����:	����ָ��ƫ�Ƶ�����
  �������:         	              		
  �������:          	 		   		 
  ����˵��:���ջ�������������
*******************************************************************************/
unsigned char USCIA2_ReadBufferOffset(unsigned int Offset)
{
    return DeQueueOffset(&USCIA2_RXQueueBuffer,Offset);
}


#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
    unsigned char DataTemp;
    switch(__even_in_range(UCA2IV,4))
    {
        case 0:  // Vector 0 - no interrupt
          break; 
          
        case 2:  // Vector 2 - RXIFG
             if(EnQueue(&USCIA2_RXQueueBuffer,UCA2RXBUF))
             {
                 if( QueueFull(&USCIA2_RXQueueBuffer) )
                 {
                     USCIA2_RXQueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
                     USCIA2_CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
                     USCIA2_CountFrame.CountTime = 0;              //��ռ���               
                 }
                 else             
                 {
                     USCIA2_RXQueueBuffer.Status &= ~BufferOV_BIT;       //�û���������־λ
                     USCIA2_ReSetFrameTime();   //������û���������������ݣ�����λ֡����������
                 }
             }
             else
             {
               USCIA2_CountFrame.Status &= ~FrameTimeEN_BIT;      //�ص�֡����ʱ
               USCIA2_RXQueueBuffer.Status |= BufferOV_BIT;       //�û���������־λ
             }
          break;
          
        case 4:  // Vector 4 - TXIFG
          if( USCIA2_TXQueueBuffer.DataLent )
          {
              DeQueue( &USCIA2_TXQueueBuffer,&DataTemp);
              UCA2TXBUF = DataTemp;
              
              if( 0 == USCIA2_TXQueueBuffer.DataLent )
              {
                  USCIA2_TXQueueBuffer.Status &= ~TXBusy_BIT;  //������ϣ����������λ
                  UCA2IE &= ~UCTXIE;   
              }
          }
          break;
          
        default: 
          break;
    }
}