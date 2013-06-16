
#include "stdio.h"
#include "msp430x54x.h"
#include "..\RTC_Module\RTC_Module.H"             //RTCģ��
#include "..\Application Layer\Application_Layer.H"                //Ӧ�ò����

#include "..\Communicate_Module\TimerB0_Module.H"
#include "..\Communicate_Module\DownChannel0_Module\DownChannel0_Module.H" //����M-Busͨ��ģ��
#include "..\Communicate_Module\UpChannel0_Module\UpChannel0_Module.H"     //����GPRS DTUͨѶģ��
#include "..\Communicate_Module\UpChannel1_Module\UpChannel1_Module.H"     //����RS232CͨѶģ��
//#include "..\Storage_Module\Storage_Module.H"
#include "Sys_Module.H"

void FlashPowerOff(void)
{
    UCB0CTL1 |= UCSWRST;
    UCB0CTL1 |= UCSSEL__ACLK;                     // ACLK
    UCB0CTL1 &= ~UCSWRST;
    
    P3SEL &= ~(BIT0+BIT1+BIT2+BIT3+BIT6+BIT7);
    P3OUT |= (BIT0+BIT1+BIT2+BIT3+BIT6+BIT7);
    P3DIR |= (BIT0+BIT1+BIT2+BIT3+BIT6+BIT7); 
    P3DIR &=~BIT2;
    
/**/    
    P11OUT |=(BIT0+BIT1+BIT2);
    P11DIR |=(BIT0+BIT1+BIT2);
    P4DIR =0;
    P6DIR = 0;
    P8DIR= 0;
    
    P10OUT |=BIT7;    //�ص�FLASHģ���Դ
}


void FlashPowerON(void)
{
    P10OUT &=~BIT7;    //��FLASHģ���Դ
    Storage_Module_Init();
/*
    P11OUT |=(BIT0+BIT1+BIT2);
    P11DIR |=(BIT0+BIT1+BIT2);
    P4DIR =0;
    P6DIR = 0;
    P8DIR= 0;
*/
}

void UartPowerOff(void)
{

    TBCTL = MC_0;        // SMCLK, UPmode, clear TBR
  
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL__ACLK;                     // SMCLK
    UCA0CTL1 &= ~UCSWRST;
    
    P3SEL &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD
    P3OUT &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD
    P3DIR |= (BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD

    UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL__ACLK;                     // SMCLK
    UCA1CTL1 &= ~UCSWRST;
    
    P5SEL &= ~(BIT6+BIT7); // P3.4,5 = USCI_A0 TXD/RXD
    P5OUT &= ~(BIT6+BIT7); // P3.4,5 = USCI_A0 TXD/RXD
    P5DIR &= ~(BIT6+BIT7); // P3.4,5 = USCI_A0 TXD/RXD

    UCA2CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA2CTL1 |= UCSSEL__ACLK;                     // SMCLK
    UCA2CTL1 &= ~UCSWRST;    
    
    P9SEL &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD
    P9OUT &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD
    P9DIR &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD

    UCA3CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA3CTL1 |= UCSSEL__ACLK;                     // SMCLK
    UCA3CTL1 &= ~UCSWRST;    
    P10SEL &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD
    P10OUT &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD
    P10DIR &= ~(BIT4+BIT5); // P3.4,5 = USCI_A0 TXD/RXD
}



void UartPowerON(void)
{
    TimerB0Init();             //ͨѶ�ö�ʱ����ʼ��
    UpChannel0_Init();         //GPRS DTUͨ�� 115200 0 8 1
    UpChannel1_Init();         //RS232Cͨ��   115200 0 8 1
    DownChanne0_Init();        //M-Busͨ��      2400 2 8 1
}

/******************************************************************************
  ����(ģ��)����:void PowerManage(void)
  ����:��Դ�������	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:��⵱ǰ��Դ״̬�������磬�������Ӧ���������͹��ģ������жϼ���Ƿ�
           �Ѿ��ϵ磬���ϵ磬���˳��͹���.
           �ϵ�����:����10�β�쵽�ϵ�״̬������˳��͹���
*******************************************************************************/
void PowerManage(void)
{
    unsigned char PowerOffTime[3];
    unsigned char PowerOnTime[3];
    
    unsigned char PowerOffData[3];
    unsigned char PowerOnData[3];
    
    unsigned char DataBuffer;

    if( (ReadRTCFlag()&BIT1) && (0 == (P2IN & BIT0)) )
    {
        ReadTime(PowerOffTime);  //��ȡ����ʱ��
        ReadDate(PowerOffData);
      
        //IOControl_Init(); //����IO�������
        FlashPowerOff();
        UartPowerOff();
        
        LPM3;
        
        FlashPowerON();            //��FLASHģ��
        UartPowerON();
          
        ReadTime( PowerOnTime );   //��ȡ�ϵ�ʱ��
        ReadDate(PowerOnData);
        
        printf("\nRePowerUp Now!\n");
        
        WriteRTCFlag( ReadRTCFlag()&(~BIT1) );    //������־λ
        
        
        if( ReadDownChanne0_TaskStatus() & BIT0 ) //�жϵ���ǰ����״̬
        {
            switch( ReadDownChanne0_TaskID() )
            {
            case 0x00:
              if( ReadDateMeterInfo() )
              {
                  if( MeterReadDataRead( &DataBuffer) )
                  {
                      if( DataBuffer > PowerOnData[0] )
                      {
                          break;
                      }
                      else
                      {
                          ReadMeasuData_TaskOFF();
                      }
                  }
                  else
                  {
                      ReadMeasuData_TaskOFF();
                  }
              }
              else
              {
                  ReadMeasuData_TaskOFF();
              }
              //����ͨ����������������
              break;
            case 0x01:
              SeareDevcie_TaskOFF();//�������ͨ���ѱ�����
              
              break;
            default:
              break;
            }
        }   
    }
}