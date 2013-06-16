
//************************************************************************
//
//  Lierda, Inc                      SPIģ�� 
//            
//************************************************************************

#include "msp430x54x.h"
#include "SoftWareSPI_Module.H"

/******************************************************************************
  ����(ģ��)����:void SoftWareSPI_Init(void)
  ����:	SPI���߳�ʼ��
  �������:         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/ 
void SoftWareSPI_Init(void) 
{ 
    SoftWareSPI_SI_OUT;
    SoftWareSPI_SO_IN;
    SoftWareSPI_SCK_OUT;
    SoftWareSPI_SCK_L;
}

/******************************************************************************
  ����(ģ��)����:void SoftWareSPI_Send_Byte(unsigned char Send_Data)
  ����:	SPI���߷���һ���ֽ�����
  �������:���͵�����         	              		
  �������: ��         	 		   		 
  ����˵��: 
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
  ����(ģ��)����:void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڷ��ͺ���
  �������:��Ҫ���͵����ݵĵ�ַָ�룬���ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        SoftWareSPI_Send_Byte(*Data_point++);
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char SoftWareSPI_Recei_Byte(void)
  ����:	SPI���߽���һ���ֽ�����
  �������:  ��        	              		
  �������:  ���յ�������       	 		   		 
  ����˵��: 
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
  ����(ģ��)����:void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڽ��պ���
  �������:��ŵĵ�ַָ�룬�Լ���Ҫ���յ����ݳ���         	              		
  �������: ���յ����ֽ�����         	 		   		 
  ����˵��: ֻ�����ȡ����
*******************************************************************************/
void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned int Data_Lent)
{
    for(unsigned int i=0;i<Data_Lent;i++)
    {
        *Data_point++ = SoftWareSPI_Recei_Byte();
    }
}