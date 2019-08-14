//************************************************************************
//
//  Lierda, Inc                      SST25VF010 
//            
//************************************************************************
#include "msp430x54x.h"
//#include "FlashStorage.H"
//#include ".\SST25VF_Module\SST25VF010_Module.H"
#include "Storage_Module.H"

__no_init struct StorageStruct FlashRAM; //��д������


/******************************************************************************
  ����(ģ��)����:void Storage_Module_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void Storage_Module_Init(void)
{
    FlashRAM.Status = 0X00;
    FlashRAM.StorageMod = SPIFlashMod;
    FlashRAM.Status = 0X00;
    FlashRAM.Status = 0X00;
    
    FlashModule_Init();
    FlashRAM_UPdate(0X00,FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);
    __delay_cycles(100);                 // Delay;
}


/******************************************************************************
  ����(ģ��)����:void Storage_Module_Init(void)
  ����:	FLASH����������
  �������:��Ҫ������FLASH��ַ�����ݻ�������ַ�����ݳ��ȣ���дѡ���Լ��洢ѡ��         	              		
  �������: ��         	 		   		 
  ����˵��:�ִ洢���ͣ�SPI���ǲ��е�FLASH�����ֶ�д�����࣬�� 
*******************************************************************************/
void FlashRAM_UPdate(unsigned long ProceAddr,unsigned char *Buffer,
                              unsigned long DataLent,unsigned char PrecessMod,
                              unsigned char StorageMod)
{
    if(StorageMod == SPIFlashMod)       //SPI FLASH����
    {
        if(ReadMOD == PrecessMod) //������
        {
            if(DataLent<128)    //�����µıȽ��٣���ѵ�ǰ����ַǰ��64���ֽڸ���
            {
                DataLent = 128;
                if(ProceAddr>63)
                {
                    ProceAddr = ProceAddr-63;
                }
                else
                {
                    ProceAddr = 0;
                }
            }
            
            FlashModule_MultiByteRead(ProceAddr,FlashRAM.RAMBuffer,DataLent);//���»�����
            FlashRAM.StorageMod = SPIFlashMod;
            FlashRAM.CurrenAddr = ProceAddr;
            FlashRAM.DataLent = DataLent;
        }
        else if(WriteMod == PrecessMod) //д����
        {
            FlashModule_SectorErase((unsigned int)(ProceAddr>>12));
            FlashModule_MultiByteWrite(ProceAddr,FlashRAM.RAMBuffer,DataLent);
        }
        else
        {
          //�޲���
        }
        
    }
    else if(FlashRAM.StorageMod == ParalFlashMod) //���� FLASH����
    {
      
    }
    else
    {
      
    }
}

/******************************************************************************
  ����(ģ��)����:void CopyFlashRAM(unsigned long ReadAddr,unsigned char *CopyBuffer,
                                   unsigned long CopyLent)
  ����:	�ӻ��������������ֽڵ�ָ��������
  �������:���Ƶĵ�ַ(��ָFLASH��ַ)��ָ����������ַ(MCU��RAM��ַ)�����ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: �ڲ����������û�������ʹ�á�ȷ��Ҫ���ĵ�ַ�ڻ�������ַ��Χ��
*******************************************************************************/
void CopyFlashRAM(unsigned long ReadAddr,unsigned char *CopyBuffer,unsigned long CopyLent)
{
    unsigned long OffSetAddr;                  //��ַƫ��
    OffSetAddr = ReadAddr-FlashRAM.CurrenAddr;
    for(unsigned long i=0;i<CopyLent;i++)
    {
      *CopyBuffer++ = FlashRAM.RAMBuffer[OffSetAddr+i];
    }
}

/******************************************************************************
  ����(ģ��)����:void WriteFlashRAM(unsigned long WriteAddr,unsigned char *WriteBuffer,
                                    unsigned long WriteLent)
  ����:	�򻺳�������ָ���ֽ�
  �������:         	              		
  �������: ��         	 		   		 
  ����˵��: �ڲ����������û�������ʹ��
*******************************************************************************/
void WriteFlashRAM(unsigned long WriteAddr,unsigned char *WriteBuffer,unsigned long WriteLent)
{
    unsigned long OffSetAddr;                  //��ַƫ��
    OffSetAddr = WriteAddr-FlashRAM.CurrenAddr;
    for(unsigned long i=0;i<WriteLent;i++)
    {
      FlashRAM.RAMBuffer[OffSetAddr+i] = *WriteBuffer++;
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  ����:	
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ������ӳ��FLASH�洢ĳ��������ַ����
*******************************************************************************/
unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned int ReadLent, unsigned char StorageMod)
{
    unsigned char ReadFlag=0;
    unsigned long FlashRAM_EndaAddr = FlashRAM.CurrenAddr + FlashRAM.DataLent;
    unsigned long ReadEndAddr = ReadAddr + ReadLent;
    
    if((FlashRAM.Status & RAMBufferBusy_BIT)==0)
    {
        FlashRAM.Status |= RAMBufferBusy_BIT;  //��æ��־λ���Է�ֹ�����������������
        if(StorageMod==FlashRAM.StorageMod)    //�жϵ�ǰ���������� SPI��FLASH����FLASH
        {
            if( ReadAddr > FlashRAM_EndaAddr )            //1 ���ݿ��ڻ��������ұ� �����ݿ��ַ����ڻ������飩
            {
                //����,��Ҫ��ȡ�����ݿ���ȫ���ڵ�ǰ������
                FlashModule_MultiByteRead(ReadAddr,ReadBuffer,ReadLent);
                ReadAddr += ReadLent>>1;   //����м��ַ
                FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                SectorSize+1,ReadMOD,SPIFlashMod);//�����м��ַ���ڵ��������ݵ�������
                ReadFlag = 1;
            }
            else if( ReadAddr < FlashRAM.CurrenAddr) //2
            {   
                if( ReadEndAddr > FlashRAM_EndaAddr )      //2.1
                {                                       //Ҫ�����ݿ������������
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,FlashRAM.CurrenAddr-ReadAddr);
                    ReadBuffer += FlashRAM.CurrenAddr-ReadAddr+1;
                    
                    CopyFlashRAM(FlashRAM.CurrenAddr,ReadBuffer,FlashRAM.DataLent);
                    ReadBuffer += FlashRAM.DataLent;
                    
                    FlashModule_MultiByteRead(FlashRAM_EndaAddr,ReadBuffer,ReadEndAddr-FlashRAM_EndaAddr);
                    ReadAddr += ReadLent>>1;   //����м��ַ  
                    FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                    SectorSize+1,ReadMOD,SPIFlashMod);//�����м��ַ���ڵ��������ݵ�������                    
                    ReadFlag = 1;
                }
                else if( ReadEndAddr < FlashRAM.CurrenAddr)//2.2
                {
                                      //����,��Ҫ��ȡ�����ݿ���ȫ���ڵ�ǰ������
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,ReadLent);
                    ReadAddr += ReadLent>>1;   //����м��ַ
                    FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                    SectorSize+1,ReadMOD,SPIFlashMod);//�����м��ַ���ڵ��������ݵ�������
                    ReadFlag = 1;
                }
                else                                       //2.3
                {   
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,FlashRAM.CurrenAddr-ReadAddr);
                    CopyFlashRAM(FlashRAM.CurrenAddr,ReadBuffer+(FlashRAM.CurrenAddr-ReadAddr)+1,ReadEndAddr-FlashRAM.CurrenAddr+1);
                    ReadAddr += ReadLent>>1;   //����м��ַ
                    FlashRAM_UPdate(ReadAddr & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,
                                    SectorSize+1,ReadMOD,SPIFlashMod);//�����м��ַ���ڵ��������ݵ�������
                    ReadFlag = 1;
                }
            }
            else                                        //3
            {
                if( ReadEndAddr > FlashRAM_EndaAddr )      //3.1
                {
                    FlashModule_MultiByteRead(ReadAddr,ReadBuffer,ReadLent);
                    ReadFlag = 1;
                }
                else                                     //3.3  ����������
                {   
                    CopyFlashRAM(ReadAddr,ReadBuffer,ReadLent);
                    ReadFlag = 1;
                }
                
            }
        } 
        FlashRAM.Status &= ~RAMBufferBusy_BIT;  //��æ��־λ�����������������������
    }
    return ReadFlag;
}


/******************************************************************************
  ����(ģ��)����:unsigned char Storage_Module_Read(unsigned long ReadAddr,unsigned char *ReadBuffer,
                                  unsigned long ReadLent, unsigned char StorageMod)
  ����:	
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Storage_Module_Write(unsigned long WritedAddr,unsigned char *WritedBuffer,
                                  unsigned int WriteLent, unsigned char StorageMod)
{
    unsigned long MasDataAddr_End = WritedAddr + (unsigned long)WriteLent;    //Ҫд�����ݿ�Ľ�����ַ
    
    unsigned long CurrenSectorWriteAddr_Start; //��ǰ����д��ʼ��ַ
    unsigned long CurrenSectorWriteAddr_End;   //��ǰ����д������ַ
    unsigned int  CurrenSectorWriteCnt;        //��ǰ����дҪд���ֽ���
    unsigned long CurrenSectorAddr_End;        //��ǰҳ������ַ
    
    if(0 == WriteLent)
    {
        return 0;
    }
    
    if((FlashRAM.Status & RAMBufferBusy_BIT)==0)
    {
        FlashRAM.Status |= RAMBufferBusy_BIT;  //��æ��־λ���Է�ֹ�����������������
        if(StorageMod==FlashRAM.StorageMod)    //�жϵ�ǰ���������� SPI��FLASH����FLASH
        {
            CurrenSectorWriteAddr_Start =  WritedAddr;            //��ʼ����ǰд��ʼ��ַ
            CurrenSectorAddr_End = CurrenSectorWriteAddr_Start| (unsigned long)SectorSize; //ȡ�õ�ǰ����������ַ
         
            while(1)
            {
                if(MasDataAddr_End > CurrenSectorAddr_End)
                {//���ݿ������ַ�ڵ�ǰҳ֮��
                    CurrenSectorWriteAddr_End = CurrenSectorAddr_End;           //ҳд������ַʹ�õ�ǰҳ������ַ
                    CurrenSectorWriteCnt = CurrenSectorWriteAddr_End - CurrenSectorWriteAddr_Start+1;  //���㵱ǰҳҪд���ֽ���
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);//���������µ�������      
                    WriteFlashRAM(CurrenSectorWriteAddr_Start,WritedBuffer,CurrenSectorWriteCnt);  //����д�뵽������
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,WriteMod,SPIFlashMod); //�ѻ��������µ�����
                    
                    CurrenSectorWriteAddr_Start = CurrenSectorWriteAddr_Start + CurrenSectorWriteCnt;//������ҳ��ʼ��ַ
                    CurrenSectorAddr_End = CurrenSectorWriteAddr_Start| (unsigned long)SectorSize;   //ȡ�õ�ǰ����������ַ
                    WritedBuffer = WritedBuffer + CurrenSectorWriteCnt;                              //�����µ����ݿ�ָ��
                }
                else  
                {//���ݿ������ַ�ڵ�ǰҳ��
                    CurrenSectorWriteAddr_End = MasDataAddr_End;           //ҳд������ַʹ�����ݿ������ַ
                    CurrenSectorWriteCnt = CurrenSectorWriteAddr_End - CurrenSectorWriteAddr_Start;  //���㵱ǰҳҪд���ֽ���
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);//���������µ�������
                    WriteFlashRAM(CurrenSectorWriteAddr_Start,WritedBuffer,CurrenSectorWriteCnt);                               //����д�뵽������
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,WriteMod,SPIFlashMod); //�ѻ�����д������
                    FlashRAM_UPdate(CurrenSectorAddr_End & (~(unsigned long)SectorSize),FlashRAM.RAMBuffer,SectorSize+1,ReadMOD,SPIFlashMod);  //����������������
                    break;
                }
            }
        }
        FlashRAM.Status &= ~RAMBufferBusy_BIT;  //��æ��־λ�����������������������
        return 1;
    }
    return 0;
}

