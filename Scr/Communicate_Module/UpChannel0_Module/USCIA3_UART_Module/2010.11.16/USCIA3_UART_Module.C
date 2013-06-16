  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |    P10.3/UCA1TXD|------------>
//            |                 | 115200 - 8N1
//            |    P10.4/UCA1RXD|<------------

#include "msp430x54x.h"
#include "USCIA3_UART_Module.H"


static struct USCIA3_DataStruct USCI_A3TXBuffer;
static struct USCIA3_DataStruct USCI_A3RXBuffer;
static struct USCIA3_CountFrameStruct USCI_A3CountFrame;

/******************************************************************************
  ����(ģ��)����:void USCIA3_UARTBufferInit(void)
  ����:	��������ʼ������
  �������: ��        	              		
  �������: ��         	 		   		 
  ����˵��: �Է��ͺͽ��պ��������г�ʼ��
*******************************************************************************/
void USCIA3_UARTBufferInit(void)
{
  USCI_A3TXBuffer.Status = 0X00;    //��������ʼ��
  USCI_A3TXBuffer.DataLent = 0X00;
  
  USCI_A3RXBuffer.Status = 0X00;
  USCI_A3RXBuffer.DataLent = 0X00;
  
  USCI_A3CountFrame.Status = 0x00;
  USCI_A3CountFrame.CountTime = 0X00;
}

/******************************************************************************
  ����(ģ��)����:void USCIA3_UART_Init(void)
  ����:	ʱ���ʼ������
  �������: BCD���ʽ��24Сʱ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA3_UART_Init(void)
{
    P10SEL |= BIT4+BIT5;                       // P10.4,5 = USCI_A3 TXD/RXD
    UCA3CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA3CTL1 |= UCSSEL_2;                     // SMCLK
    UCA3BR0 = 9;                              // 1MHz 115200 (see User's Guide)
    UCA3BR1 = 0;                              // 1MHz 115200
    UCA3MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    USCIA3_UARTBufferInit();                  //���ա����ͻ�������ʼ��
    UCA3IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}

/******************************************************************************
  ����(ģ��)����:void USCIA3_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
  ����:	
  �������:    ParityBit У��λ 0��ʾ��У��λ��1��ʾ��У�飬2��ʾżУ��
               DataBit   ����λ 0��ʾ8λ����λ��1��ʾ7λ����λ
               StopBit   ֹͣλ 0��ʾ1λֹͣλ��1��ʾ2λֹͣλ
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA3_UART_Set(unsigned long BaudRate,unsigned char ParityBit,
                      unsigned char DataBit,unsigned char StopBit)
{
    P3SEL |= BIT4+BIT5;                       // P3.4,5 = USCI_A3 TXD/RXD
    UCA3CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA3CTL1 |= UCSSEL_2;                     // SMCLK
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
    
    if(DataBit==0)
    {
        UCA3CTL0 &= ~UC7BIT;                     // 8λ����λ
    }
    else if(DataBit==1)
    {
        UCA3CTL0 |= UC7BIT;                    // 7λ����λ
    }
    
    if(StopBit==0)
    {
        UCA3CTL0 &= ~UCSPB;                     // 1λֹͣλ
    }
    else if(StopBit==1)
    {
        UCA3CTL0 |= UCSPB;                    // 2λֹͣλ
    }
    
    UCA3BR0 = 9;                              // 1MHz 115200 (see User's Guide)
    UCA3BR1 = 0;                              // 1MHz 115200
    UCA3MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA3IE |= UCRXIE;                         // Enable USCI_A3 RX interrupt
}


/******************************************************************************
  ����(ģ��)����:void ByteSend_UA3RT(unsigned char SendData)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵������ֽ�
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void USCIA3_ByteSend(unsigned char SendData)
{
    while (!(UCA3IFG&UCTXIFG));            // USCI_A3 TX buffer ready?
    UCA3TXBUF = SendData;                  // TX -> RXed character
}


/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteTX_UA3RT(unsigned char *Buffer,unsigned int Lent)
  ����:	���ֽڷ��ͺ���
  �������: Ҫ���͵����ݻ�������ַ������
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����ǰ�Ѿ��������ڷ��ͻ���Ĳ��������⣬�򷵻�0
*******************************************************************************/
unsigned char USCIA3_MultiByteTX(unsigned char *Buffer,unsigned int Lent)
{
    if(!USCI_A3TXBuffer.Status)//ȷ�����Ϳ���
    {
        if(Lent)             //ȷ���ֽ�������0
        {
            USCI_A3TXBuffer.Status |= BIT0;       //��æ��־λ
            USCI_A3TXBuffer.DataLent = Lent-1;    //ȡ�ֽ���
            for(unsigned int i=0;i<Lent;i++)
              USCI_A3TXBuffer.DataBuffer[i] = *Buffer++;
            USCI_A3TXBuffer.BufferPoint = USCI_A3TXBuffer.DataBuffer;       //ȡ�÷����׵�ַ
            USCIA3_ByteSend(*USCI_A3TXBuffer.BufferPoint++);                //���͵�һ���ֽڣ��������жϷ���
            UCA3IE |= UCTXIE;                                               // �򿪷����ж�
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



/******************************************************************************
  ����(ģ��)����:void USCIA3_ReSetFrameTime(void)
  ����:	֡���ʱ���⿪��������������0
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ���յ�һ���ֽں󣬵���������������֡ʱ���������0����ĩ��������ִ�п���������
*******************************************************************************/
void USCIA3_ReSetFrameTime(void)
{
    USCI_A3CountFrame.Status |= FrameTimeEN_BIT;
    if(USCI_A3CountFrame.CountTime==FrameTime)
    {
        USCI_A3RXBuffer.Status |= FrameTime_BIT;  //��֡��־λ
    }
    USCI_A3CountFrame.CountTime = 0;              //��ռ���
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
    if(USCI_A3CountFrame.Status & FrameTimeEN_BIT)
    {
        USCI_A3CountFrame.CountTime++;//����;
        if(USCI_A3CountFrame.CountTime==FrameTime)
        {
            USCI_A3RXBuffer.Status |= FrameTime_BIT;//��֡��־λ
            USCI_A3CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
            USCI_A3CountFrame.CountTime = 0;              //��ռ���
        }
    }
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:  // Vector 0 - no interrupt
    break;                             
  case 2:  // Vector 2 - RXIFG
    if(USCI_A3RXBuffer.DataLent < DataBufferMax)       //������û��
    {
        USCI_A3RXBuffer.DataBuffer[USCI_A3RXBuffer.DataLent++] = UCA3RXBUF;//�ѽ��յ������ݷŵ�������
        if(USCI_A3RXBuffer.DataLent == DataBufferMax) //��������
        {
            USCI_A3RXBuffer.Status |= BufferOV_BIT;       //�û���������־λ
            USCI_A3CountFrame.Status &= ~FrameTimeEN_BIT; //�ص�֡����ʱ
            USCI_A3CountFrame.CountTime = 0;              //��ռ���
        }
        else
        {
            USCIA3_ReSetFrameTime();                  //������û���������������ݣ�����λ֡����������
        }
    }
    else                                   
    {
        ;                                  //�������������ٽ��յ������ݾ�����
    }
    break;
  case 4:  // Vector 4 - TXIFG
    if(USCI_A3TXBuffer.DataLent)
    {
        USCI_A3TXBuffer.DataLent--;
        UCA3TXBUF = *USCI_A3TXBuffer.BufferPoint++;
    }
    else
    {
        USCI_A3TXBuffer.Status &= ~TXBusy_BIT;  //������ϣ����������λ
        UCA3IE &= ~UCTXIE;                      // �رշ����ж�
    }
    break;                                  
  default: 
    break;
  }
}