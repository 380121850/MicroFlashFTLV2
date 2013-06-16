  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include "Sys_Module.H"


/******************************************************************************
  ����(ģ��)����:void IOControl_Init(void)
  ����:IO�ڿ���ģ���ʼ��	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��: 
*******************************************************************************/
void IOControl_Init(void)
{
    P1DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //���0
    P1OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

    P2DIR |= (BIT3+BIT4+BIT5+BIT6+BIT7);                //���0
    P2OUT |= ~(BIT3+BIT4+BIT5+BIT6+BIT7);
    
    //P3DIR |= (BIT0+BIT1+BIT2+BIT3+BIT6+BIT7);           //���1
    //P3OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT6+BIT7);
    
    P4DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //���0
    P4OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

    P5DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);
    P5OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);

    P6DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //���0
    P6OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

    P7DIR |= (BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //���0
    P7OUT |= ~(BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    
    P8DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);//���0
    P8OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    
    P9DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //���0
    P9OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    
    P10DIR |= (BIT0+BIT3); //���0
    P10OUT |= ~(BIT0+BIT3);
  
}

/******************************************************************************
  ����(ģ��)����:void MBUSOn(void)
  ����:M-BUS���������	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��: 
*******************************************************************************/
void MBUSOn(void)
{
   P10OUT &=~BIT6;
   P10DIR |=BIT6;
}

/******************************************************************************
  ����(ģ��)����:void MBUSOFF(void)
  ����:M-BUS��������ر�	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��: 
*******************************************************************************/
void MBUSOFF(void)
{
   P10OUT |=BIT6;
   P10DIR |=BIT6;
}