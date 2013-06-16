
//************************************************************************
//
//  Lierda, Inc                      SPI模拟 
//            
//************************************************************************

#include "msp430x54x.h"
#include "SoftWareSPI_Module.H"

/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Init(void)
  功能:	SPI总线初始化
  输入参数:         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
void SoftWareSPI_Init(void) 
{ 
    SoftWareSPI_SI_OUT;
    SoftWareSPI_SO_IN;
    SoftWareSPI_SCK_OUT;
    SoftWareSPI_SCK_L;
}

/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Send_Byte(unsigned char Send_Data)
  功能:	SPI总线发送一个字节数据
  输入参数:发送的数据         	              		
  输出参数: 无         	 		   		 
  其它说明: 
*******************************************************************************/ 
void SoftWareSPI_Send_Byte(unsigned char SendData)
{
    for(unsigned int i=0;i<8;i++)
    {
        if(SendData & BIT7)
        {
            SoftWareSPI_SI_H;
        }
        else
        {
            SoftWareSPI_SI_L;
        }
        SoftWareSPI_SCK_H;              /* toggle clock high */  
        SendData = (SendData << 1);     /* shift 1 place for next bit */  
        SoftWareSPI_SCK_L;              /* toggle clock low */  
    }
}


/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节发送函数
  输入参数:需要发送的数据的地址指针，数据长度         	              		
  输出参数: 无         	 		   		 
  其它说明: 只负责把数据发送出去
*******************************************************************************/
void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        SoftWareSPI_Send_Byte(*Data_point++);
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char SoftWareSPI_Recei_Byte(void)
  功能:	SPI总线接收一个字节数据
  输入参数:  无        	              		
  输出参数:  接收到的数据       	 		   		 
  其它说明: 
*******************************************************************************/ 
unsigned char SoftWareSPI_Recei_Byte(void)  
{  
    unsigned char RecivData = 0;  
    for (unsigned int i = 0; i < 8; i++)  
    {  
        RecivData = (RecivData << 1);       /* shift 1 place to the left or shift in 0 */  
        SoftWareSPI_SCK_H;                      /* toggle clock high */ 
        if (SoftWareSPI_SO_Read)                /* check to see if bit is high */  
          RecivData = RecivData | BIT0;     /* if high, make bit high */  
        SoftWareSPI_SCK_L;                      /* toggle clock low */ 
    }  
    return RecivData;  
} 


/******************************************************************************
  函数(模块)名称:void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  功能:	SPI多字节接收函数
  输入参数:存放的地址指针，以及需要接收的数据长度         	              		
  输出参数: 接收到的字节数据         	 		   		 
  其它说明: 只负责读取数据
*******************************************************************************/
void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        *Data_point++ = SoftWareSPI_Recei_Byte();
    }
}