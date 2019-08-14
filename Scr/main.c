
#include "msp430x54x.h"
#include "stdio.h"
#include ".\Sys_Module\Sys_Module.H"             //单片机基本模块
#include ".\Files_Module\FileSys_Module.H"
#include ".\RTC_Module\RTC_Module.H"             //RTC模块


//#include ".\Communicate_Module\TimerB0_Module.H"                          //通讯定时器模块
//#include ".\Communicate_Module\DownChannel0_Module\DownChannel0_Module.H" //下行M-Bus通道模块
//#include ".\Communicate_Module\UpChannel0_Module\UpChannel0_Module.H"     //上行GPRS DTU通讯模块
//#include ".\Communicate_Module\UpChannel1_Module\UpChannel1_Module.H"     //上行RS232C通讯模块
//#include ".\Communicate_Module\CommunicatiTest_Module.H"
//#include ".\Communicate_Module\UpChannel0_Module\USCIA3_UART_Module\USCIA3_UART_Module.H"
#include ".\Communicate_Module\UpChannel1_Module\USCIA1_UART_Module\USCIA1_UART_Module.H"

void main( void )
{
    //unsigned int EmptyPhySectNum=1;
    WDTCTL = WDTPW + WDTHOLD;  // Stop watchdog timer to prevent time out reset   
    Init_Clk();                //时钟初始化为8M
    USCIA1_UARTSet(115200,0,8,1);
    RTC_Init(24);
    TimerA0Init();
    FileSysInit();
    
    _EINT();                  //打开总中断
    
    while(1);
}