#ifndef  __FlashStorage_H
#define  __FlashStorage_H

#include ".\SST25VF_Module\SST25VF010_Module.H"
#include ".\SST29VF_Module\SST29VF_Module.H"


//#define  FlashModule_ture   1

/******************************************************************************
  ����(ģ��)����:void FlashModule_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_Init  SST29VF_Init
#else
#define FlashModule_Init  SST25VF010_Init
#endif


/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_ReadInfo(unsigned char *ReadBuffer)
  ����:	FLASH��оƬ��Ϣ����
  �������:�����ݻ�����ָ��        	              		
  �������: �������        	 		   		 
  ����˵��: Ĭ��2�ֽڣ���һ�ֽ�ΪоƬ���̱�ʶ���ڶ��ֽ�ΪоƬID
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_ReadInfo  ReadID_SST29
#else
#define FlashModule_ReadInfo  ReadID_SST25
#endif


/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_ReadByte(unsigned long ReadAddr)
  ����:	FLASH���ֽڶ�����
  �������:����ַ        	              		
  �������:������         	 		   		 
  ����˵��: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_ReadByte  ReadByte_SST29
#else
#define FlashModule_ReadByte  ReadByte_SST25
#endif


/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
  ����:	FLASH���ֽ�д����
  �������:д��ַ��д����        	              		
  �������:         	 		   		 
  ����˵��: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_WriteByte  ByteWrite_SST29
#else
#define FlashModule_WriteByte  ByteWrite_SST25
#endif



/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
  ����:	FLASH���ֽڶ�����
  �������:����ַ�����ݻ�����ָ�룬���ݳ���        	              		
  �������: �������        	 		   		 
  ����˵��: ���4095���ֽ�
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_MultiByteRead  MultiByteRead_SST29
#else
#define FlashModule_MultiByteRead  MultiByteRead_SST25
#endif



/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
  ����:	FLASH���ֽ�д����
  �������:д��ַ��д���ݻ�����ָ�룬���ݳ���        	              		
  �������: �������        	 		   		 
  ����˵��: ���4095���ֽ�
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_MultiByteWrite  MultiByteWrite_SST29
#else
#define FlashModule_MultiByteWrite  MultiByteWrite_SST25
#endif



/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_SectorErase(unsigned int SectorAddr)
  ����:	FLASH������������
  �������:������ַ        	              		
  �������: �������        	 		   		 
  ����˵��: ���65536������
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_SectorErase  SectorErase_SST29
#else
#define FlashModule_SectorErase  SectorErase_SST25
#endif

/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_BlockErase(unsigned int BlockAddr)
  ����:	FLASHоƬ���������
  �������:���ַ        	              		
  �������: �������        	 		   		 
  ����˵��: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_BlockErase 
#else
#define FlashModule_BlockErase  BlockErase_SST25
#endif


/******************************************************************************
  ����(ģ��)����:unsigned char FlashModule_ChipErase(void)
  ����:	FLASHоƬ��������
  �������:��        	              		
  �������: �������        	 		   		 
  ����˵��: 
*******************************************************************************/
#ifdef  FlashModule_ture
#define FlashModule_ChipErase ChipErase_SST29 
#else
#define FlashModule_ChipErase  ChipErase_SST25
#endif

unsigned char FlashTest(unsigned long FlashMaxSize);

#endif