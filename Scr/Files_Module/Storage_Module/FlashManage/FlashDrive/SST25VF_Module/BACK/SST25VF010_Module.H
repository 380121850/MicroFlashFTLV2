#ifndef  __SST25VF010_Module_H
#define  __SST25VF010_Module_H

//оƬ���ƽӿ�
#define SPI_CE_BIT     BIT0    //Ҫ��  1��CE����Ϊ���IO��
#define SPI_HOLD_BIT   BIT6    //      2��HOLDΪ���IO��
#define SPI_WP_BIT     BIT7    //      2��WPΪ���IO��

#define SPI_DIR_Port  P3DIR  //      3����ʹ�õ�Դ���ƣ���VCC����Ϊ���IO��
#define SPI_OUT_Port  P3OUT
#define SPI_IN_Port   P3IN

//��Դ
#define SPI_PowerOut   P10DIR |=BIT7;
#define SPI_PowerOn    P10OUT &=~BIT7;
#define SPI_PowerOff   P10OUT |=BIT7;

#define SPI_CE_OUT   SPI_DIR_Port |=SPI_CE_BIT;
#define SPI_CE_H     SPI_OUT_Port |= SPI_CE_BIT;
#define SPI_CE_L     SPI_OUT_Port &=~SPI_CE_BIT;

#define SPI_HOLD_OUT   SPI_DIR_Port |=SPI_HOLD_BIT;
#define SPI_HOLD_H     SPI_OUT_Port |= SPI_HOLD_BIT;
#define SPI_HOLD_L     SPI_OUT_Port &=~SPI_HOLD_BIT;

#define SPI_WP_OUT   SPI_DIR_Port |=SPI_WP_BIT;
#define SPI_WP_H     SPI_OUT_Port |= SPI_WP_BIT;
#define SPI_WP_L     SPI_OUT_Port &=~SPI_WP_BIT;

#define Read_Command            0x03
#define SectorErase_Command     0x20
#define BlorkErase_Command      0x52
#define ChipErase_Command       0x60
#define ByteProgram_Command     0x02
#define AAIProgram_Command      0xAF
#define RDSR_Command            0x05
#define EWSR_Command            0x50
#define WRSR_Command            0x01
#define WREN_Command            0x06
#define WRDI_Command            0x04
#define ReadID_Command          0x90

#define Busy_BIT  0X01
#define WEL_BIT   0X02
#define BP0_BIT   0X04
#define BP1_BIT   0X08

#define AAI_BIT   0X40
#define BPL_BIT   0X80


/******************************************************************************
  ����(ģ��)����:void SST25VF010_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void SST25VF010_Init(void);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadID(unsigned char Manu_or_Device)
  ����:	���豸��ʶ��
  �������:��Ҫ��ȡ�ı�ʶ��         	              		
  �������: �豸��ʶ��        	 		   		 
  ����˵��: 0����ȡ�豸���̱�־ BF (FOR SST)��1����ȡ�豸��ʶ�� 13 ��FOR SST29SF040��
*******************************************************************************/
unsigned char ReadID_SST25(unsigned char *ReadBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadByte_SST25(unsigned long ReadAddr)
  ����:	FLASH������
  �������:         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadByte_SST25(unsigned long ReadAddr);

/******************************************************************************
  ����(ģ��)����:void ReadData(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
  ����:	FLASH������
  �������:����ʼ��ַ����������ַ�����ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void MultiByteRead_SST25(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent);

/******************************************************************************
  ����(ģ��)����:void ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData)
  ����:	FLASH�ֽ�д����
  �������:д��ַ��д����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ByteWrite_SST25(unsigned long WritedAddr,unsigned char WriteData);

/******************************************************************************
  ����(ģ��)����:void MultiByteWrite(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent)
  ����:	FLASH�ֽ�д����
  �������:д��ַ��д����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void MultiByteWrite_SST25(unsigned long WriteAddr,unsigned char *WriteData,unsigned int WriteLent);

/******************************************************************************
  ����(ģ��)����:void SectorErase_SST25(unsigned int SectorAddr)
  ����:	FLASH������������
  �������:������ַ����0��ʼ         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void SectorErase_SST25(unsigned int SectorAddr);

/******************************************************************************
  ����(ģ��)����:void SectorErase(unsigned int SectorAddr)
  ����:	FLASH���������
  �������:FLASH���ַ����0��ʼ         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void BlockErase_SST25(unsigned int BlockAddr);

/******************************************************************************
  ����(ģ��)����:void ChipErase(void)
  ����:	FLASHоƬȫ����������
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void ChipErase_SST25(void);

#endif