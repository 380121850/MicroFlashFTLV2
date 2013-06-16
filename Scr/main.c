
#include "msp430x54x.h"
#include "stdio.h"
#include ".\Sys_Module\Sys_Module.H"             //��Ƭ������ģ��
#include ".\Files_Module\FileSys_Module.H"
#include ".\RTC_Module\RTC_Module.H"             //RTCģ��


//#include ".\Communicate_Module\TimerB0_Module.H"                          //ͨѶ��ʱ��ģ��
//#include ".\Communicate_Module\DownChannel0_Module\DownChannel0_Module.H" //����M-Busͨ��ģ��
//#include ".\Communicate_Module\UpChannel0_Module\UpChannel0_Module.H"     //����GPRS DTUͨѶģ��
//#include ".\Communicate_Module\UpChannel1_Module\UpChannel1_Module.H"     //����RS232CͨѶģ��
//#include ".\Communicate_Module\CommunicatiTest_Module.H"
//#include ".\Communicate_Module\UpChannel0_Module\USCIA3_UART_Module\USCIA3_UART_Module.H"
#include ".\Communicate_Module\UpChannel1_Module\USCIA1_UART_Module\USCIA1_UART_Module.H"

void main( void )
{
    //unsigned int EmptyPhySectNum=1;
    WDTCTL = WDTPW + WDTHOLD;  // Stop watchdog timer to prevent time out reset   
    Init_Clk();                //ʱ�ӳ�ʼ��Ϊ8M
    USCIA1_UARTSet(115200,0,8,1);
    RTC_Init(24);
    TimerA0Init();
    FileSysInit();
    
    _EINT();                  //�����ж�
    
    while(1);
}