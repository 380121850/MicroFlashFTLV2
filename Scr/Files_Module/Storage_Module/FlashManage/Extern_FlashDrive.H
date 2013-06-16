#ifndef  __Extern_FlashDrive_H
#define  __Extern_FlashDrive_H


/******************************************************************************
  ����(ģ��)����:void FlashDrive_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
extern void FlashDrive_Init(void);


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer)
  ����:	FLASH��оƬ��Ϣ����
  �������:�����ݻ�����ָ��        	              		
  �������: �������        	 		   		 
  ����˵��: Ĭ��2�ֽڣ���һ�ֽ�ΪоƬ���̱�ʶ���ڶ��ֽ�ΪоƬID
*******************************************************************************/
extern unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer);


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_ReadByte(unsigned long ReadAddr)
  ����:	FLASH���ֽڶ�����
  �������:����ַ        	              		
  �������:������         	 		   		 
  ����˵��: 
*******************************************************************************/
extern unsigned char FlashDrive_ReadByte(unsigned long ReadAddr);


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
  ����:	FLASH���ֽ�д����
  �������:д��ַ��д����        	              		
  �������:         	 		   		 
  ����˵��: 
*******************************************************************************/
extern unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte);



/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
  ����:	FLASH���ֽڶ�����
  �������:����ַ�����ݻ�����ָ�룬���ݳ���        	              		
  �������: �������        	 		   		 
  ����˵��: ���4095���ֽ�
*******************************************************************************/
extern unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent);


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
  ����:	FLASH���ֽ�д����
  �������:д��ַ��д���ݻ�����ָ�룬���ݳ���        	              		
  �������: �������        	 		   		 
  ����˵��: ���4095���ֽ�
*******************************************************************************/
extern unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent);



/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_SectorErase(unsigned int SectorAddr)
  ����:	FLASH������������
  �������:������ַ        	              		
  �������: �������        	 		   		 
  ����˵��: ���65536������
*******************************************************************************/
extern unsigned char FlashDrive_SectorErase(unsigned int SectorAddr);


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_BlockErase(unsigned int BlockAddr)
  ����:	FLASHоƬ���������
  �������:���ַ        	              		
  �������: �������        	 		   		 
  ����˵��: 
*******************************************************************************/
extern unsigned char FlashDrive_BlockErase(unsigned int BlockAddr);


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_ChipErase(void)
  ����:	FLASHоƬ��������
  �������:��        	              		
  �������: �������        	 		   		 
  ����˵��: 
*******************************************************************************/
extern unsigned char FlashDrive_ChipErase(void);

#endif