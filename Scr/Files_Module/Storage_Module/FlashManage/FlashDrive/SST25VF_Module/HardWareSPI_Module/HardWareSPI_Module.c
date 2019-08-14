//************************************************************************
//
//  Lierda, Inc                      HardWareSPI 
//            
//************************************************************************
#include "msp430x54x.h"
#include "HardWareSPI_Module.H"

struct HardWareSPI_DataStruct
{
    unsigned char *DataPoint;
    unsigned int DataLent;
    unsigned char DataStatus;
};

struct HardWareSPI_DataStruct HardWareSPI_Data;


/******************************************************************************
  ����(ģ��)����:void HardWareSPI_Init(void)
  ����:	
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void HardWareSPI_Init(void)
{
    //P3DIR |= BIT1+BIT3;
    P3SEL |= BIT1+BIT2+BIT3;                  //
    //P3REN |= BIT1+BIT2+BIT3;
    UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCB0CTL0 |= UCMST+UCSYNC+UCMSB+UCCKPH;    //+UCCKPL;// 3-pin, 8-bit HardWareSPI master
                                              // Clock polarity high, MSB
    UCB0CTL1 |= UCSSEL_3;                     // SMCLK
    UCB0BR0 = 0;                              // /2
    UCB0BR1 = 0;                              //
    UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    //UCB0IE |= UCRXIE+UCTXIE;                  // Enable USCI_A0 RX interrupt
}

/******************************************************************************
  ����(ģ��)����:unsigned char HardWareSPI_Send_Byte(unsigned char Send_Data)
  ����:	HardWareSPI���ֽڷ��ͺ���
  �������:��Ҫ���͵�����         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
unsigned char HardWareSPI_Send_Byte(unsigned char Send_Data)
{
    unsigned int WaitTime=0;
    
    while (!(UCB0IFG&UCTXIFG)) // USCI_B0 TX buffer ready?
    {
        WaitTime++;
        if(WaitTime>32768)
          return 0;
    }              
    UCB0TXBUF = Send_Data;                     // Transmit first character
    WaitTime=0;
    while(UCB0STAT & UCBUSY)
    {
        WaitTime++;
        if(WaitTime>32768)
          return 0;
    }
    return 1;
}

/******************************************************************************
  ����(ģ��)����:void HardWareSPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	HardWareSPI���ֽڷ��ͺ���
  �������:��Ҫ���͵����ݵĵ�ַָ�룬���ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
void HardWareSPI_Send_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        HardWareSPI_Send_Byte(*Data_point++);
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char HardWareSPI_Recei_Byte(void)
  ����:	HardWareSPI���ֽڽ��պ���
  �������:��         	              		
  �������: ���յ����ֽ�����         	 		   		 
  ����˵��: ֻ�����ȡ����
*******************************************************************************/
unsigned char HardWareSPI_Recei_Byte(void)
{   
    unsigned int WaitTime=0;
    unsigned char ReadTemp = UCB0RXBUF;//��ս��ջ���������־λ
    HardWareSPI_Send_Byte(0XFF);
    while (!(UCB0IFG&UCRXIFG))           // USCI_B0 RX buffer
    {
        WaitTime++;
        if(WaitTime>32768)
          return 0;
    }  
    return UCB0RXBUF;                     // Transmit first character
}

/******************************************************************************
  ����(ģ��)����:void HardWareSPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	HardWareSPI���ֽڽ��պ���
  �������:��ŵĵ�ַָ�룬�Լ���Ҫ���յ����ݳ���         	              		
  �������: ���յ����ֽ�����         	 		   		 
  ����˵��: ֻ�����ȡ����
*******************************************************************************/
void HardWareSPI_Recei_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        *Data_point++ = HardWareSPI_Recei_Byte();
    }
}


/******************************************************************************
  ����(ģ��)����:void HardWareSPI_Send_Mass_Ext(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	HardWareSPI���ֽڷ��ͺ�����ʹ���жϷ�ʽ
  �������:��Ҫ���͵����ݵĵ�ַָ�룬���ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
void HardWareSPI_Send_Mass_Ext(unsigned char *Data_point,unsigned int Data_Lent)
{
    HardWareSPI_Data.DataPoint=Data_point+1;//����ָ��
    HardWareSPI_Data.DataLent=Data_Lent-1;  //���ݳ���
    HardWareSPI_Data.DataStatus=0X01;     //���ͺͽ���
    UCB0IFG &= ~UCRXIFG;   //��������жϱ�־λ
    UCB0IE |= UCTXIE;      // Enable USCI_B0 RX interrupt
    HardWareSPI_Send_Byte(*Data_point);
}


/******************************************************************************
  ����(ģ��)����:void HardWareSPI_Recei_Mass_Ext(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	HardWareSPI���ֽڽ��պ�����ʹ���жϷ�ʽ
  �������:������ָ�룬���ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
void HardWareSPI_Recei_Mass_Ext(unsigned char *Data_point,unsigned int Data_Lent)
{
    HardWareSPI_Data.DataPoint=Data_point;//����ָ��
    HardWareSPI_Data.DataLent=Data_Lent;  //���ݳ���
    HardWareSPI_Data.DataStatus=0X00;     //���ͺͽ���
    
    UCB0IFG &= ~UCRXIFG;   //��������жϱ�־λ
    UCB0IE |= UCRXIE;      // Enable USCI_B0 RX interrupt
    UCB0TXBUF = 0X55;      // Transmit first character
}




#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(UCB0IV)
  {
    case 0: 
      break;                                // Vector 0 - no interrupt
    case 2:                                 // Vector 2 - RXIFG
    case 4:
      if(HardWareSPI_Data.DataStatus)          //��������״̬
      {
          if(!HardWareSPI_Data.DataLent)       //�Ѿ������꣬����������жϱ�־λ�������ж�
          {
              UCB0IFG &= ~UCRXIFG;    //��������жϱ�־λ
              UCB0IE &= ~UCTXIE;
          }
          else
          {
              UCB0IFG &= ~UCRXIFG;    //��������жϱ�־λ
              UCB0TXBUF = *HardWareSPI_Data.DataPoint++;
              HardWareSPI_Data.DataLent--;            
          }
      }
      else                  //��������״̬
      {
          *HardWareSPI_Data.DataPoint++ = UCB0RXBUF;
          HardWareSPI_Data.DataLent--;
          if(!HardWareSPI_Data.DataLent)       //�Ѿ������꣬����������жϱ�־λ�������ж�
          {
              //UCB0IFG &= ~UCTXIFG;    //��������жϱ�־λ
              UCB0IE &= ~UCRXIE;      // Enable USCI_B0 RX interrupt
          }
          else
          {
              UCB0TXBUF = 0X55; // Transmit first character
          }
      }
      break;                                // Vector 4 - TXIFG
      
    default: 
      break;
  }
}

