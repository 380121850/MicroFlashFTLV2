#ifndef  __StorageManage_H
#define  __StorageManage_H


/******************************************************************************
  ����(ģ��)����:void Storage_Module_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void Storage_Module_Init(void);

/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer)
  ����:	
  �������: ��         	              		
  �������: ��         	 		   		 
  ����˵��: ������ӳ��FLASH�洢ĳ��������ַ����
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer)
  ����:	
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer);
#endif