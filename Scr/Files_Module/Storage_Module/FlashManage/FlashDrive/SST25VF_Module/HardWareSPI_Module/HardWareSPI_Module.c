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
  函数(模块)名称:void HardWareSPI_Init(void)
  功能:	
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char HardWareSPI_Send_Byte(unsigned char Send_Data)
  功能:	HardWareSPI单字节发送函数
  输入参数:需要发送的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
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
  函数(模块)名称:void HardWareSPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	HardWareSPI多字节发送函数
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void HardWareSPI_Send_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        HardWareSPI_Send_Byte(*Data_point++);
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char HardWareSPI_Recei_Byte(void)
  功能:	HardWareSPI单字节接收函数
  输入参数:无         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
unsigned char HardWareSPI_Recei_Byte(void)
{   
    unsigned int WaitTime=0;
    unsigned char ReadTemp = UCB0RXBUF;//清空接收缓冲区及标志位
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
  函数(模块)名称:void HardWareSPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	HardWareSPI多字节接收函数
  输入参数:存放的地址指针，以及需要接收的数据长度         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
void HardWareSPI_Recei_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        *Data_point++ = HardWareSPI_Recei_Byte();
    }
}


/******************************************************************************
  函数(模块)名称:void HardWareSPI_Send_Mass_Ext(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	HardWareSPI多字节发送函数，使用中断方式
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void HardWareSPI_Send_Mass_Ext(unsigned char *Data_point,unsigned int Data_Lent)
{
    HardWareSPI_Data.DataPoint=Data_point+1;//数据指针
    HardWareSPI_Data.DataLent=Data_Lent-1;  //数据长度
    HardWareSPI_Data.DataStatus=0X01;     //发送和接收
    UCB0IFG &= ~UCRXIFG;   //清除发送中断标志位
    UCB0IE |= UCTXIE;      // Enable USCI_B0 RX interrupt
    HardWareSPI_Send_Byte(*Data_point);
}


/******************************************************************************
  函数(模块)名称:void HardWareSPI_Recei_Mass_Ext(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	HardWareSPI多字节接收函数，使用中断方式
  输入参数:缓冲区指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void HardWareSPI_Recei_Mass_Ext(unsigned char *Data_point,unsigned int Data_Lent)
{
    HardWareSPI_Data.DataPoint=Data_point;//数据指针
    HardWareSPI_Data.DataLent=Data_Lent;  //数据长度
    HardWareSPI_Data.DataStatus=0X00;     //发送和接收
    
    UCB0IFG &= ~UCRXIFG;   //清除发送中断标志位
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
      if(HardWareSPI_Data.DataStatus)          //连续发送状态
      {
          if(!HardWareSPI_Data.DataLent)       //已经发送完，则清除发送中断标志位及发送中断
          {
              UCB0IFG &= ~UCRXIFG;    //清除发送中断标志位
              UCB0IE &= ~UCTXIE;
          }
          else
          {
              UCB0IFG &= ~UCRXIFG;    //清除发送中断标志位
              UCB0TXBUF = *HardWareSPI_Data.DataPoint++;
              HardWareSPI_Data.DataLent--;            
          }
      }
      else                  //连续接收状态
      {
          *HardWareSPI_Data.DataPoint++ = UCB0RXBUF;
          HardWareSPI_Data.DataLent--;
          if(!HardWareSPI_Data.DataLent)       //已经接收完，则清除接收中断标志位及接收中断
          {
              //UCB0IFG &= ~UCTXIFG;    //清除发送中断标志位
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

