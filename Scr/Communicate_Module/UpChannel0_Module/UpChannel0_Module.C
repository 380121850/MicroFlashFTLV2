  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P3.4/UCA0TXD|------------>
//            |                 | 115200 - 8N1
//            |     P3.5/UCA0RXD|<------------

#include "msp430x54x.h"
#include ".\USCIA3_UART_Module\USCIA3_UART_Module.H"
#include "Up0CJ188_Module.H"
#include "UpChannel0_Module.H"


/******************************************************************************
  函数(模块)名称:void UpChannel0_Init(void)
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
void UpChannel0_Init(void)
{
    USCIA3_UARTSet(115200,0,8,1);
}


/******************************************************************************
  函数(模块)名称:unsigned char Up0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
  功能:	上行通道多字节发送函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Up0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
{
    return USCIA3_MultiByteTX(SendBuffer,BufferLent);
}

/******************************************************************************
  函数(模块)名称:void Up0RecievProcess(void)
  功能:	上行通道处理任务
  输入参数: 无
  输出参数: 无         	 		   		 
  其它说明: 处于从机地位，被动接收由主机发送来的命令，并处理
*******************************************************************************/
void Up0RecievProcess(void)
{
    struct Up0FrameStruct FrameTemp;
    Up0CJ188_ReceivProcess(&FrameTemp);
}
