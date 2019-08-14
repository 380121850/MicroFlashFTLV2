
#include "stdio.h"
#include "msp430x54x.h"
#include "..\RTC_Module\RTC_Module.H"             //RTC模块
#include "..\Application Layer\Application_Layer.H"                //应用层服务

#include "..\Communicate_Module\TimerB0_Module.H"
#include "..\Communicate_Module\DownChannel0_Module\DownChannel0_Module.H" //下行M-Bus通道模块
#include "..\Communicate_Module\UpChannel0_Module\UpChannel0_Module.H"     //上行GPRS DTU通讯模块
#include "..\Communicate_Module\UpChannel1_Module\UpChannel1_Module.H"     //上行RS232C通讯模块
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
    
    P10OUT |=BIT7;    //关掉FLASH模块电源
}


void FlashPowerON(void)
{
    P10OUT &=~BIT7;    //打开FLASH模块电源
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
    TimerB0Init();             //通讯用定时器初始化
    UpChannel0_Init();         //GPRS DTU通道 115200 0 8 1
    UpChannel1_Init();         //RS232C通道   115200 0 8 1
    DownChanne0_Init();        //M-Bus通道      2400 2 8 1
}

/******************************************************************************
  函数(模块)名称:void PowerManage(void)
  功能:电源监测任务	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:监测当前电源状态，若掉电，则进行相应处理后进入低功耗，再用中断检测是否
           已经上电，若上电，则退出低功耗.
           上电条件:连续10次查检到上电状态，则才退出低功耗
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
        ReadTime(PowerOffTime);  //读取掉电时间
        ReadDate(PowerOffData);
      
        //IOControl_Init(); //空闲IO口输出低
        FlashPowerOff();
        UartPowerOff();
        
        LPM3;
        
        FlashPowerON();            //打开FLASH模块
        UartPowerON();
          
        ReadTime( PowerOnTime );   //读取上电时间
        ReadDate(PowerOnData);
        
        printf("\nRePowerUp Now!\n");
        
        WriteRTCFlag( ReadRTCFlag()&(~BIT1) );    //清掉电标志位
        
        
        if( ReadDownChanne0_TaskStatus() & BIT0 ) //判断掉电前任务状态
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
              //下行通道读计量数据任务
              break;
            case 0x01:
              SeareDevcie_TaskOFF();//清掉下行通道搜表任务
              
              break;
            default:
              break;
            }
        }   
    }
}