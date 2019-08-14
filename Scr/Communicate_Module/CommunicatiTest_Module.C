  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include ".\DownChannel0_Module\USCIA0_UART_Module\USCIA0_UART_Module.H"//下行通道M-Bus
#include ".\UpChannel1_Module\USCIA1_UART_Module\USCIA1_UART_Module.H"  //上行通道RS232C
#include ".\UpChannel0_Module\USCIA3_UART_Module\USCIA3_UART_Module.H"  //上行通道GPRS DTU
#include "CommunicatiTest_Module.H"

void UARTPortInit(unsigned char UartX,unsigned long BandRate)
{
  switch( UartX )
  {
  case 0:
    USCIA0_UARTSet(BandRate,2,8,1);
    break;
  case 1:
    USCIA1_UARTSet(BandRate,2,8,1);
    break;
  case 2:
    //USCIA2_UARTSet(BandRate,2,8,1);
    break;
  case 3:
    USCIA3_UARTSet(BandRate,2,8,1);
    break;
  default:
    break;
  }
}

void UARTPortTX(unsigned char UartX,unsigned char Data)
{
  switch( UartX )
  {
  case 0:
    USCIA0_ByteSend(Data);
    break;
  case 1:
    USCIA1_ByteSend(Data);
    break;
  case 2:
    //USCIA2_ByteSend(Data);
    break;
  case 3:
    USCIA3_ByteSend(Data);
    break;
  default:
    break;
  }
}

unsigned char UARTPortRX(unsigned char UartX)
{
  switch( UartX )
  {
  case 0:
    while (!(UCA0IFG&UCRXIFG));
    return UCA0RXBUF;

  case 1:
    while (!(UCA1IFG&UCRXIFG));
    return UCA1RXBUF;
   return 0;

  case 2:
    return 0;

  case 3:
    while (!(UCA3IFG&UCRXIFG));
    return UCA3RXBUF;

  default:
    return 0;
  }
}

unsigned char UartPortTest(unsigned char UartX,unsigned long BandRate)
{
   unsigned char UARTTestFail[]="UARTTest Test is Fail!";
   unsigned char UARTTestOK[]="UARTTest Test is Ok!";
   unsigned char UARTOK[]="  is Ok!";
   
   UARTPortInit(UartX,BandRate);
   for(unsigned int i=0;i<256;i++)
   {
     for(unsigned int j=0;j<256;j++)
     {
         UARTPortTX(UartX,(unsigned char) i);
         if( UARTPortRX(UartX) != i )
         {
           //USCIA1_MultiByteTX(UARTTestFail,sizeof(UARTTestFail));
           return 0;
         }

     }
    //USCIA1_ByteSend(i);
    //USCIA1_MultiByteTX(UARTOK,sizeof(UARTOK));
    
   }
   //USCIA1_MultiByteTX(UARTTestOK,sizeof(UARTTestOK));
   return 1;
}