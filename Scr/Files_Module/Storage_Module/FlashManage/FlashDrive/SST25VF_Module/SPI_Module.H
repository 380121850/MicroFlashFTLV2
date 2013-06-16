#ifndef  __SPI_Module_H
#define  __SPI_Module_H

//#define  SoftWareSPI_Disable   1
#include ".\SoftWareSPI_Module\SoftWareSPI_Module.H"
#include ".\HardWareSPI_Module\HardWareSPI_Module.H"

/******************************************************************************
  ����(ģ��)����:void SPI_Init(void)
  ����:	
  �������:         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/ 
#ifdef  SoftWareSPI_Disable
#define SPI_Init  SoftWareSPI_Init
#else
#define SPI_Init  HardWareSPI_Init
#endif

/******************************************************************************
  ����(ģ��)����:void SPI_Send_Byte(unsigned char Send_Data)
  ����:	SPI���߷���һ���ֽ�����
  �������:���͵�����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/ 
#ifdef  SoftWareSPI_Disable
#define SPI_Send_Byte  SoftWareSPI_Send_Byte
#else
#define SPI_Send_Byte  HardWareSPI_Send_Byte
#endif

/******************************************************************************
  ����(ģ��)����:void SPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڷ��ͺ���
  �������:��Ҫ���͵����ݵĵ�ַָ�룬���ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
#ifdef  SoftWareSPI_Disable
#define SPI_Send_Mass  SoftWareSPI_Send_Mass
#else
#define SPI_Send_Mass  HardWareSPI_Send_Mass
#endif

/******************************************************************************
  ����(ģ��)����:unsigned char SPI_Recei_Byte(void)
  ����:	SPI���߽���һ���ֽ�����
  �������:  ��        	              		
  �������:  ���յ�������       	 		   		 
  ����˵��: 
*******************************************************************************/ 
#ifdef  SoftWareSPI_Disable
#define SPI_Recei_Byte  SoftWareSPI_Recei_Byte
#else
#define SPI_Recei_Byte  HardWareSPI_Recei_Byte
#endif


/******************************************************************************
  ����(ģ��)����:void SPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڽ��պ���
  �������:��ŵĵ�ַָ�룬�Լ���Ҫ���յ����ݳ���         	              		
  �������: ���յ����ֽ�����         	 		   		 
  ����˵��: ֻ�����ȡ����
*******************************************************************************/
#ifdef  SoftWareSPI_Disable
#define SPI_Recei_Mass  SoftWareSPI_Recei_Mass
#else
#define SPI_Recei_Mass  HardWareSPI_Recei_Mass
#endif

#endif