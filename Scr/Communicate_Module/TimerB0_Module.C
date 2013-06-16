  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include ".\DownChannel0_Module\USCIA0_UART_Module\USCIA0_UART_Module.H"//����ͨ��M-Bus
#include ".\UpChannel1_Module\USCIA1_UART_Module\USCIA1_UART_Module.H"  //����ͨ��RS232C
#include ".\UpChannel0_Module\USCIA3_UART_Module\USCIA3_UART_Module.H"  //����ͨ��GPRS DTU
#include "TimerB0_Module.H"

/******************************************************************************
  ����(ģ��)����:void TimerB0Init(void)
  ����:ͨѶ�ö�ʱ����ʼ��	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:ʹ��������ģʽ��ʹ��SMCLKʱ��Դ���жϼ����ԼΪ2.38ms��
*******************************************************************************/
void TimerB0Init(void)
{
  TBCCTL0 = CCIE;                         // CCR0 interrupt enabled
  TBCCR0 = 20000;                         // ��Լ2.38ms
  TBCTL = TBSSEL_2 + MC_1 + TBCLR;        // SMCLK, UPmode, clear TBR
}



/******************************************************************************
  ����(ģ��)����:__interrupt void TIMERB0_ISR (void)
  ����:ͨѶ�ö�ʱ���жϺ���	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:ʹ��������ģʽ��ʹ��SMCLKʱ��Դ���жϼ����ԼΪ2.38ms��
*******************************************************************************/
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR (void)
{
/*  */
    static unsigned int TimeCnt=0;
    TimeCnt++;
    if(TimeCnt>10000)
    {
        USCIA3_MultiByteTX((unsigned char *)&TimeCnt,sizeof(TimeCnt));
        TimeCnt = 0;
    }
    
    USCIA0_CountFrameTime();           //USCI_A0����֡�����������
    USCIA0_FrameAckTime_CountTime();   //USCI_A0����֡�����������
    
    USCIA1_CountFrameTime();           //USCI_A1����֡�����������
    USCIA3_CountFrameTime();           //USCI_A3����֡�����������
}