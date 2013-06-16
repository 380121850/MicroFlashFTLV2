  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include "..\RTC_Module\RTC_Module.H"             //RTCģ��
#include "Sys_Module.H"

unsigned int TimeCnt = 0;

/******************************************************************************
  ����(ģ��)����:void TimerB0Init(void)
  ����:ͨѶ�ö�ʱ����ʼ��	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:ʹ��������ģʽ��ʹ��SMCLKʱ��Դ���жϼ����ԼΪ2.38ms��
*******************************************************************************/
void TimerA0Init(void)
{
    TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
    TA1CCR0 = 100;
    TA1CTL = TASSEL_1 + MC_1 + TACLR;         // ALCK,UPmode, clear TBR
}


/******************************************************************************
  ����(ģ��)����:__interrupt void TIMER1_A0_ISR(void)
  ����:�ϵ���	       	 		   		 
  ����˵��:ʹ��������ģʽ��ʹ��ALCKʱ��Դ���жϼ����ԼΪms��
           ����10�β�쵽�ϵ�״̬������˳��͹���
*******************************************************************************/
// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    static unsigned int CheckPowerCnt = 0;         //���������
    TimeCnt++;
    if( BIT0 == (P2IN & BIT0) )                    //�����
    {   //û�е���
        CheckPowerCnt++;                           //���������
        if( CheckPowerCnt>10 )                     //����10�μ�鵽����״̬
        {
            WriteRTCFlag( ReadRTCFlag()&(~BIT1) ); //������־λ
            CheckPowerCnt = 0;                     //������0          
            LPM3_EXIT;   
        }
    }
    else
    {   //�Ѿ�����
        CheckPowerCnt = 0;                          //������0
    }

}