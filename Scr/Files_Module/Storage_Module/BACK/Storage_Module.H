#ifndef  __Storage_Module_H
#define  __Storage_Module_H


#define SectorSize     0XFFF  //������С
#define RAMBufferMax   SectorSize
#define SPIFlashMod    0X00
#define ParalFlashMod  0X01
#define ReadMOD        0X00
#define WriteMod       0X01


struct StorageStruct
{
    unsigned char Status;
    unsigned char StorageMod;
    unsigned long CurrenAddr;
    unsigned int  DataLent;
    unsigned char RAMBuffer[RAMBufferMax+1];
};
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
//#define             BIT7
#define  RAMBufferBusy_BIT           BIT0


/******************************************************************************
  ����(ģ��)����:void Storage_Module_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void Storage_Module_Init(void);

/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  ����:	
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned int ReadLent, unsigned char StorageMod);

/******************************************************************************
  ����(ģ��)����:void Storage_Module_Init(void)
  ����:	FLASH����������
  �������:��Ҫ������FLASH��ַ�����ݻ�������ַ�����ݳ��ȣ���дѡ���Լ��洢ѡ��         	              		
  �������: ��         	 		   		 
  ����˵��:�ִ洢���ͣ�SPI���ǲ��е�FLASH�����ֶ�д�����࣬�� 
*******************************************************************************/
void FlashRAM_UPdate(unsigned long ProceAddr,unsigned char *Buffer,
                              unsigned long DataLent,unsigned char PrecessMod,
                              unsigned char StorageMod);

/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  ����:	
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WritedAddr,unsigned char *WritedBuffer,
                                  unsigned int WriteLent, unsigned char StorageMod);
#endif