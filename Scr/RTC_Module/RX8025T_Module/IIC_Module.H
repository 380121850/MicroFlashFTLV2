#ifndef  __IIC_Module_H
#define  __IIC_Module_H

//#define  SoftWareIIC_Disable   1

#include ".\HardWareIIC_Module\HardWareIIC_Module.H"
#include ".\SoftWareIIC_Module\SoftWareIIC_Module.H"

/******************************************************************************
  ����(ģ��)����:void IIC_Init(void)
  ����:	
  �������:         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/ 
#ifdef  SoftWareIIC_Disable
#define IIC_Init  SoftWareIIC_Init
#else
#define IIC_Init  HardWareIIC_Init
#endif

/******************************************************************************
  ����(ģ��)����:void IIC_Send_Byte(unsigned char Send_Data)
  ����:	SPI���߷���һ���ֽ�����
  �������:���͵�����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/ 
#ifdef  SoftWareIIC_Disable
#define IIC_SingleWrite  SoftWareIIC_Single_Write
#else
#define IIC_SingleWrite  HardWareIIC_SingleWrite
#endif

/******************************************************************************
  ����(ģ��)����:void IIC_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڷ��ͺ���
  �������:��Ҫ���͵����ݵĵ�ַָ�룬���ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
#ifdef  SoftWareIIC_Disable
#define IIC_SingleRead  SoftWareIIC_Single_Read
#else
#define IIC_SingleRead  HardWareIIC_SingleRead
#endif

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Recei_Byte(void)
  ����:	SPI���߽���һ���ֽ�����
  �������:  ��        	              		
  �������:  ���յ�������       	 		   		 
  ����˵��: 
*******************************************************************************/ 
#ifdef  SoftWareIIC_Disable
#define IIC_MultiRead  SoftWareIIC_Multi_read
#else
#define IIC_MultiRead  HardWareIIC_MultiRead
#endif


/******************************************************************************
  ����(ģ��)����:void IIC_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڽ��պ���
  �������:��ŵĵ�ַָ�룬�Լ���Ҫ���յ����ݳ���         	              		
  �������: ���յ����ֽ�����         	 		   		 
  ����˵��: ֻ�����ȡ����
*******************************************************************************/
#ifdef  SoftWareIIC_Disable
#define IIC_MultiWrite  SoftWareIIC_Page_Write
#else
#define IIC_MultiWrite  HardWareIIC_MultiWrite
#endif

#endif