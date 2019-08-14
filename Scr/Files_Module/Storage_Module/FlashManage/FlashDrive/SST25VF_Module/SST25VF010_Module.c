//************************************************************************
//
//  Lierda, Inc                      SST25VF010 
//            
//************************************************************************
#include "msp430x54x.h"
#include "SPI_Module.h"
#include "SST25VF010_Module.H"

/******************************************************************************
  ����(ģ��)����:unsigned char ReadStatus(void)
  ����:	��״̬�Ĵ���
  �������:��         	              		
  �������: ״̬�Ĵ���         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadStatus(void)
{
    unsigned char StatusTemp;
    SPI_CE_L;
    SPI_Send_Byte(RDSR_Command);//��������
    //__delay_cycles(10);                 // Delay;
    StatusTemp = SPI_Recei_Byte();
    SPI_CE_H;
    return StatusTemp;
}


/******************************************************************************
  ����(ģ��)����:void StatusWrite(unsigned char StatusData)
  ����:	״̬�Ĵ���д
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: �޸ĵ�ǰ�Ĵ���
*******************************************************************************/
void StatusWrite(unsigned char StatusData)
{
    //�Ĵ���дʹ��
    SPI_CE_L;
    SPI_Send_Byte(EWSR_Command);//��������
    SPI_CE_H;
    //__delay_cycles(50);                 // Delay;
    SPI_CE_L;
    SPI_Send_Byte(WRSR_Command);//��������
    SPI_Send_Byte(StatusData);  //��������
    SPI_CE_H;
}

/******************************************************************************
  ����(ģ��)����:void WriteEnable(void)
  ����:	дʹ�ܺ���
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�к�FLASH��д
*******************************************************************************/
void WriteEnable(void)
{
    SPI_CE_L;
    SPI_Send_Byte(WREN_Command);//��������
    SPI_CE_H;
}
/******************************************************************************
  ����(ģ��)����:void WriteDisable(void)
  ����:	д��ֹ����
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: ִ�к�FLASH��ֹд
*******************************************************************************/
void WriteDisable(void)
{
    SPI_CE_L;
    SPI_Send_Byte(WRDI_Command);//��������
    SPI_CE_H;
}

/******************************************************************************
  ����(ģ��)����:void SST25VF010_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void SST25VF010_Init(void)
{
    SPI_CE_H;
    SPI_HOLD_H;
    SPI_WP_H;
    
    SPI_CE_OUT;
    SPI_HOLD_OUT;
    SPI_WP_OUT;
    SPI_PowerOut;
    SPI_PowerOn;
    __delay_cycles(1000);                 // Delay;
    SPI_Init();
    StatusWrite(0X00);
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadID(unsigned char Manu_or_Device)
  ����:	���豸��ʶ��
  �������:��Ҫ��ȡ�ı�ʶ��         	              		
  �������: �豸��ʶ��        	 		   		 
  ����˵��: 0����ȡ�豸���̱�־ BF (FOR SST)��1����ȡ�豸��ʶ�� 13 ��FOR SST29SF040��
*******************************************************************************/
unsigned char ReadID_SST25(unsigned char *ReadBuffer)
{
    SPI_CE_L;
    SPI_Send_Byte(ReadID_Command);//��������
    SPI_Send_Byte(0X00);
    SPI_Send_Byte(0X00);
    SPI_Send_Byte(0X01);
    SPI_Recei_Mass(ReadBuffer,2);
    SPI_CE_H;
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadByte_SST25(unsigned long ReadAddr)
  ����:	FLASH������
  �������:         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadByte_SST25(unsigned long ReadAddr)
{
    unsigned char Addr0,Addr1,Addr2,ReadData;
    Addr0=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr1=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr2=(unsigned char)ReadAddr;
    
    SPI_CE_L;
    SPI_Send_Byte(Read_Command);//���Ͷ�����
    SPI_Send_Byte(Addr2);       //���Ͷ���ַ
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    ReadData = SPI_Recei_Byte(); //��������
    SPI_CE_H;
    return ReadData;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadData(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
  ����:	FLASH������
  �������:����ʼ��ַ����������ַ�����ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char MultiByteRead_SST25(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
{
    unsigned char Addr0,Addr1,Addr2;
    Addr0=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr1=(unsigned char)ReadAddr;
    ReadAddr=ReadAddr>>8;
    
    Addr2=(unsigned char)ReadAddr;
    
    SPI_CE_L;
    SPI_Send_Byte(Read_Command);//���Ͷ�����
    SPI_Send_Byte(Addr2);       //���Ͷ���ַ
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_Recei_Mass(ReadBuffer,ReadLent); //��������
    SPI_CE_H;
    return 1;
}


/******************************************************************************
  ����(ģ��)����:void ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData)
  ����:	FLASH�ֽ�д����
  �������:д��ַ��д����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData)
{
    unsigned char Addr0,Addr1,Addr2;
    Addr0=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr1=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr2=(unsigned char)WritedAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(ByteProgram_Command);//��������
    SPI_Send_Byte(Addr2);              //���͵�ַ
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_Send_Byte(WriteData);          //��������
    SPI_CE_H;
    //WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
    
}


/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteWrite(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent)
  ����:	FLASH�ֽ�д����
  �������:д��ַ��д����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char MultiByteWrite_SST25(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent)
{
    unsigned char Addr0,Addr1,Addr2;
    Addr0=(unsigned char)WriteAddr;
    WriteAddr=WriteAddr>>8;
    
    Addr1=(unsigned char)WriteAddr;
    WriteAddr=WriteAddr>>8;
    
    Addr2=(unsigned char)WriteAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(AAIProgram_Command);  //��������
    SPI_Send_Byte(Addr2);               //���͵�ַ
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_Send_Byte(*WriteData++);        //��������
    SPI_CE_H;
    //__delay_cycles(50);                 // Delay;
    while(ReadStatus()&Busy_BIT);
    for(unsigned int i=0;i<WriteLent-1;i++)
    {
        SPI_CE_L;
        SPI_Send_Byte(AAIProgram_Command);    //��������
        SPI_Send_Byte(*WriteData++);          //��������
        SPI_CE_H;
        while(ReadStatus()&Busy_BIT);
    }
    WriteDisable();
    while(ReadStatus()&WEL_BIT);
    return 1;
}


/******************************************************************************
  ����(ģ��)����:unsigned char SectorErase_SST25(unsigned int SectorAddr)
  ����:	FLASH������������
  �������:������ַ����0��ʼ         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SectorErase_SST25(unsigned int SectorAddr)
{
    unsigned char Addr0,Addr1,Addr2;
    unsigned long WritedAddr;
    WritedAddr = (unsigned long)SectorAddr;
    WritedAddr=WritedAddr<<12;
    
    Addr0=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr1=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr2=(unsigned char)WritedAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(SectorErase_Command);//��������
    SPI_Send_Byte(Addr2);              //���͵�ַ
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_CE_H;
    WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
    
}


/******************************************************************************
  ����(ģ��)����:unsigned char SectorErase(unsigned int SectorAddr)
  ����:	FLASH���������
  �������:FLASH���ַ����0��ʼ         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char BlockErase_SST25(unsigned int BlockAddr)
{
    unsigned char Addr0,Addr1,Addr2;
    unsigned long WritedAddr;
    WritedAddr = (unsigned long)BlockAddr;
    WritedAddr=WritedAddr<<15;
    
    Addr0=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr1=(unsigned char)WritedAddr;
    WritedAddr=WritedAddr>>8;
    
    Addr2=(unsigned char)WritedAddr;
    
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(BlorkErase_Command);//��������
    SPI_Send_Byte(Addr2);              //���͵�ַ
    SPI_Send_Byte(Addr1);
    SPI_Send_Byte(Addr0);
    SPI_CE_H;
    WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ChipErase(void)
  ����:	FLASHоƬȫ����������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ChipErase_SST25(void)
{
    WriteEnable();
    SPI_CE_L;
    SPI_Send_Byte(ChipErase_Command);//��������
    SPI_CE_H;
    WriteDisable();
    while(ReadStatus()&Busy_BIT);
    return 1;
}


