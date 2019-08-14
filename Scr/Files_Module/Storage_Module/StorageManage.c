//************************************************************************
//
//  �����ṩͳһ�ӿ�API
//            
//************************************************************************
#include".\FlashManage\FlashManage.h"
#include "StorageManage.H"

#define StorageManage_Select  1

/******************************************************************************
  ����(ģ��)����:void Storage_Module_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void Storage_Module_Init(void)
{
#if StorageManage_Select == 0
    ;
#elif StorageManage_Select == 1
    FlashManage_Init();
#endif
}


/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer)
  ����:	
  �������: ��         	              		
  �������: ��         	 		   		 
  ����˵��: ������ӳ��FLASH�洢ĳ��������ַ����
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned int DataLent,
                                  unsigned char *DataBuffer)
{
#if StorageManage_Select == 0
    ;
#elif StorageManage_Select == 1
    return LogicFlashRead( ReadAddr, DataLent, DataBuffer);
#endif
    
}


/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer)
  ����:	
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WriteAddr,unsigned int DataLent,
                                   unsigned char *DataBuffer)
{
#if StorageManage_Select == 0
    ;
#elif StorageManage_Select == 1
    return LogicFlashWrite( WriteAddr, DataLent, DataBuffer);
#endif
   
}

