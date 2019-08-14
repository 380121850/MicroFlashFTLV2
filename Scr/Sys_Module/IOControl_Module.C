  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include "Sys_Module.H"


/******************************************************************************
  函数(模块)名称:void IOControl_Init(void)
  功能:IO口控制模块初始化	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明: 
*******************************************************************************/
void IOControl_Init(void)
{
    P1DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //输出0
    P1OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

    P2DIR |= (BIT3+BIT4+BIT5+BIT6+BIT7);                //输出0
    P2OUT |= ~(BIT3+BIT4+BIT5+BIT6+BIT7);
    
    //P3DIR |= (BIT0+BIT1+BIT2+BIT3+BIT6+BIT7);           //输出1
    //P3OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT6+BIT7);
    
    P4DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //输出0
    P4OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

    P5DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);
    P5OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5);

    P6DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //输出0
    P6OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

    P7DIR |= (BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //输出0
    P7OUT |= ~(BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    
    P8DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);//输出0
    P8OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    
    P9DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7); //输出0
    P9OUT |= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
    
    P10DIR |= (BIT0+BIT3); //输出0
    P10OUT |= ~(BIT0+BIT3);
  
}

/******************************************************************************
  函数(模块)名称:void MBUSOn(void)
  功能:M-BUS总线输出打开	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明: 
*******************************************************************************/
void MBUSOn(void)
{
   P10OUT &=~BIT6;
   P10DIR |=BIT6;
}

/******************************************************************************
  函数(模块)名称:void MBUSOFF(void)
  功能:M-BUS总线输出关闭	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明: 
*******************************************************************************/
void MBUSOFF(void)
{
   P10OUT |=BIT6;
   P10DIR |=BIT6;
}