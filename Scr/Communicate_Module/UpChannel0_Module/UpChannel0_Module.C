  
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
  ����(ģ��)����:void UpChannel0_Init(void)
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void UpChannel0_Init(void)
{
    USCIA3_UARTSet(115200,0,8,1);
}


/******************************************************************************
  ����(ģ��)����:unsigned char Up0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
  ����:	����ͨ�����ֽڷ��ͺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Up0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
{
    return USCIA3_MultiByteTX(SendBuffer,BufferLent);
}

/******************************************************************************
  ����(ģ��)����:void Up0RecievProcess(void)
  ����:	����ͨ����������
  �������: ��
  �������: ��         	 		   		 
  ����˵��: ���ڴӻ���λ���������������������������������
*******************************************************************************/
void Up0RecievProcess(void)
{
    struct Up0FrameStruct FrameTemp;
    Up0CJ188_ReceivProcess(&FrameTemp);
}