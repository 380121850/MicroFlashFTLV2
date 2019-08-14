//************************************************************************
//
//                     FlashDrive 
//            
//************************************************************************
//************************************************************************
//  1���ṩ
//                    
//            
//************************************************************************
#include ".\SST25VF_Module\SST25VF010_Module.H"
#include ".\SST29VF_Module\SST29VF_Module.H"
#include "FlashDrive.H"

#define FlashDrive_Select  1

/******************************************************************************
  ����(ģ��)����:void FlashDrive_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void FlashDrive_Init(void)
{
#if FlashDrive_Select == 0
    SST29VF_Init();
#elif FlashDrive_Select == 1
    SST25VF010_Init();
#endif
}

/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer)
  ����:	FLASH��оƬ��Ϣ����
  �������:�����ݻ�����ָ��        	              		
  �������: �������        	 		   		 
  ����˵��: Ĭ��2�ֽڣ���һ�ֽ�ΪоƬ���̱�ʶ���ڶ��ֽ�ΪоƬID
*******************************************************************************/
unsigned char FlashDrive_ReadInfo(unsigned char *ReadBuffer)
{   
#if FlashDrive_Select == 0
    return ReadID_SST29(ReadBuffer);
#elif FlashDrive_Select == 1
    return ReadID_SST25(ReadBuffer);
#endif
}


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_ReadByte(unsigned long ReadAddr)
  ����:	FLASH���ֽڶ�����
  �������:����ַ        	              		
  �������:������         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char FlashDrive_ReadByte(unsigned long ReadAddr)
{ 
#if FlashDrive_Select == 0
    return ReadByte_SST29(ReadAddr);
#elif FlashDrive_Select == 1
    return ReadByte_SST25(ReadAddr);
#endif    
}


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
  ����:	FLASH���ֽ�д����
  �������:д��ַ��д����        	              		
  �������:         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char FlashDrive_WriteByte(unsigned long WriteAddr,
                                                      unsigned char WriteByte)
{
#if FlashDrive_Select == 0
    return ByteWrite_SST29( WriteAddr,WriteByte);
#elif FlashDrive_Select == 1
    return ByteWrite_SST25( WriteAddr,WriteByte);
#endif
}



/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
  ����:	FLASH���ֽڶ�����
  �������:����ַ�����ݻ�����ָ�룬���ݳ���        	              		
  �������: �������        	 		   		 
  ����˵��: ���4095���ֽ�
*******************************************************************************/
unsigned char FlashDrive_MultiByteRead(unsigned long ReadAddr,
                                                          unsigned char *ReadBuffer,
                                                          unsigned int ReadLent)
{  
#if FlashDrive_Select == 0
    return MultiByteRead_SST29( ReadAddr, ReadBuffer, ReadLent);
#elif FlashDrive_Select == 1
    return MultiByteRead_SST25( ReadAddr, ReadBuffer, ReadLent);
#endif
}


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
  ����:	FLASH���ֽ�д����
  �������:д��ַ��д���ݻ�����ָ�룬���ݳ���        	              		
  �������: �������        	 		   		 
  ����˵��: ���4095���ֽ�
*******************************************************************************/
unsigned char FlashDrive_MultiByteWrite(unsigned long WriteAddr,
                                                           unsigned char *WriteBuffer,
                                                           unsigned int Lent)
{   
#if FlashDrive_Select == 0
    return MultiByteWrite_SST29( WriteAddr, WriteBuffer, Lent);
#elif FlashDrive_Select == 1
    return MultiByteWrite_SST25( WriteAddr, WriteBuffer, Lent);
#endif
}



/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_SectorErase(unsigned int SectorAddr)
  ����:	FLASH������������
  �������:������ַ        	              		
  �������: �������        	 		   		 
  ����˵��: ���65536������
*******************************************************************************/
unsigned char FlashDrive_SectorErase(unsigned int SectorAddr)
{
#if FlashDrive_Select == 0
    return SectorErase_SST29( SectorAddr);
#elif FlashDrive_Select == 1
    return SectorErase_SST25( SectorAddr);
#endif
}
  


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_BlockErase(unsigned int BlockAddr)
  ����:	FLASHоƬ���������
  �������:���ַ        	              		
  �������: �������        	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char FlashDrive_BlockErase(unsigned int BlockAddr)
{
#if FlashDrive_Select == 0
     return BlockErase_SST29( BlockAddr);
#elif FlashDrive_Select == 1
     return BlockErase_SST25( BlockAddr);
#endif
}


/******************************************************************************
  ����(ģ��)����:unsigned char FlashDrive_ChipErase(void)
  ����:	FLASHоƬ��������
  �������:��        	              		
  �������: �������        	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char FlashDrive_ChipErase(void)
{
#if FlashDrive_Select == 0
    return ChipErase_SST29();
#elif FlashDrive_Select == 1
    return ChipErase_SST25();
#endif
}