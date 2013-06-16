//************************************************************************
//
//  Lierda, Inc                      SST25VF010 
//            
//************************************************************************
#include "msp430x54x.h"
#include "Parallel_Module.h"
#include "SST29VF_Module.H"

/******************************************************************************
  ����(ģ��)����:void SST29VF_Init(void)
  ����:	FLASHоƬ��ʼ��
  �������:��         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/
void SST29VF_Init(void)
{
    Parallel_CTL_Port; //���ƿ�����Ϊ��ͨIO��
    
    Parallel_CE_H;
    Parallel_CE_OUT;
    
    Parallel_OE_H;
    Parallel_OE_OUT;
    
    Parallel_WE_H;
    Parallel_WE_OUT;
    
    Parallel_Init();
}

/******************************************************************************
  ����(ģ��)����:void ParallWritData(unsigned long WriteAddr,unsigned char WriteByte)
  ����:	���ַд��һ���ֽ�
  �������:Ҫд��ĵ�ַ��Ҫд������         	              		
  �������: ��         	 		   		 
  ����˵��: �Բ������ߵķ�ʽ�������дʹ��λ��
*******************************************************************************/
void ParallWritData(unsigned long WriteAddr,unsigned char WriteByte)
{
    WrteAddrPort(WriteAddr);  
    Parallel_WE_L;
    WrteDataPort(WriteByte);    
    __delay_cycles(3);                 // Delay;
    Parallel_WE_H;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ParallReadData(unsigned long ReadAddr)
  ����:	�ӵ�ַ����һ���ֽ�
  �������:Ҫ��ȡ�ĵ�ַ         	              		
  �������: ������������        	 		   		 
  ����˵��: �Բ������ߵķ�ʽ��
*******************************************************************************/
unsigned char ParallReadData(unsigned long ReadAddr)
{
    unsigned char ReadData=0;
    WrteAddrPort(ReadAddr);
    __delay_cycles(1);                 // Delay;
    Parallel_OE_L;
    __delay_cycles(1);                 // Delay;
    ReadData = ReadDataPort();
    Parallel_OE_H;
    return ReadData;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadID(unsigned char Manu_or_Device)
  ����:	���豸��ʶ��
  �������:��Ҫ��ȡ�ı�ʶ��         	              		
  �������: �豸��ʶ��        	 		   		 
  ����˵��: 0����ȡ�豸���̱�־ BF (FOR SST)��1����ȡ�豸��ʶ�� 13 ��FOR SST29SF040��
*******************************************************************************/
unsigned char ReadID_SST29(unsigned char *ReadBuffer)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //����IDģʽ
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X90);
    __delay_cycles(5);                 // Delay;
    
    *ReadBuffer++ = ParallReadData(0X0000);       //��ȡ�豸���̱�ʶ��ID��
    *ReadBuffer = ParallReadData(0X0001);
    
    ParallWritData(0X555,0XAA);             //�˳�IDģʽ
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0XF0);
    __delay_cycles(5);                 // Delay;
    
    Parallel_CE_H;
    return 1;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ParallReadData(unsigned long ReadAddr)
  ����:	�ӵ�ַ����һ���ֽ�
  �������:Ҫ��ȡ�ĵ�ַ         	              		
  �������: ������������        	 		   		 
  ����˵��: �Բ������ߵķ�ʽ��
*******************************************************************************/
unsigned char ReadByte_SST29(unsigned long ReadAddr)
{
    unsigned char ReadData;
    Parallel_CE_L;
    WrteAddrPort(ReadAddr);
    Parallel_OE_L;
    __delay_cycles(2);                 // Delay;
    ReadData = ReadDataPort();
    Parallel_OE_H;
    Parallel_CE_H;
    return ReadData;
}


/******************************************************************************
  ����(ģ��)����:unsigned char MultiByteRead_SST29(unsigned long ReadAddr,
                                 unsigned char *ReadBuffer,unsigned int ReadLent)
  ����:	�ӵ�ַ����һ���ֽ�
  �������:Ҫ��ȡ�ĵ�ַ         	              		
  �������: ������������        	 		   		 
  ����˵��: �Բ������ߵķ�ʽ��
*******************************************************************************/
unsigned char MultiByteRead_SST29(unsigned long ReadAddr,unsigned char *ReadBuffer,unsigned int ReadLent)
{
    DataPort_DIR_IN;
    Parallel_CE_L;
    Parallel_OE_L;    
    for(unsigned int i=0;i<ReadLent;i++)
    {
        WrteAddrPort(ReadAddr++);
         __delay_cycles(2);                 // Delay;
        *ReadBuffer++ = DataPort_IN;
    }
    Parallel_OE_H;
    Parallel_CE_H;
    return 1;
}


unsigned char CheckToggleBit(unsigned long ReadAddr)
{
    unsigned char PreData=0;
    unsigned char CurrData=0xff;
    unsigned int PollTime=1;
    while(PollTime)
    {
        PreData = ReadByte_SST29(ReadAddr)&BIT6;
        __delay_cycles(20);                 // Delay;
        CurrData = ReadByte_SST29(ReadAddr)&BIT6;
        if(CurrData==PreData)
        {
            return 1;
        }
    }
    return 0; 
}

unsigned char CheckDQ7Poling(unsigned long ReadAddr)
{
    unsigned char PreData=0;
    unsigned int PollTime=1;
    while(PollTime)
    {
        PreData = ReadByte_SST29(ReadAddr)&BIT7;
        __delay_cycles(20);                 // Delay;
        if(PreData)
        {
            return 1;
        }
    }
    return 0; 
}


/******************************************************************************
  ����(ģ��)����:unsigned char ByteWrite_SST29(unsigned long WriteAddr,unsigned char WriteByte)
  ����:	���豸��ʶ��
  �������:��Ҫ��ȡ�ı�ʶ��         	              		
  �������: �豸��ʶ��        	 		   		 
  ����˵��: 0����ȡ�豸���̱�־ BF (FOR SST)��1����ȡ�豸��ʶ�� 13 ��FOR SST29SF040��
*******************************************************************************/
unsigned char ByteWrite_SST29(unsigned long WriteAddr,unsigned char WriteByte)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //����IDģʽ
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0XA0);
    ParallWritData(WriteAddr,WriteByte);
    
    __delay_cycles(100);                 // Delay;
    CheckToggleBit(0);
    
    Parallel_CE_H;
    return 1;
  
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadID(unsigned char Manu_or_Device)
  ����:	���豸��ʶ��
  �������:��Ҫ��ȡ�ı�ʶ��         	              		
  �������: �豸��ʶ��        	 		   		 
  ����˵��: 0����ȡ�豸���̱�־ BF (FOR SST)��1����ȡ�豸��ʶ�� 13 ��FOR SST29SF040��
*******************************************************************************/
unsigned char MultiByteWrite_SST29(unsigned long WriteAddr,unsigned char *WriteBuffer,unsigned int Lent)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //����IDģʽ
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0XA0);
    while(Lent--)
    {
        ParallWritData(WriteAddr++,*WriteBuffer++);
        __delay_cycles(20);                 // Delay;
        CheckToggleBit(0);
        
    }
    
    __delay_cycles(100);                 // Delay;
    CheckToggleBit(0);
    
    Parallel_CE_H;  
    return 1;
}


/******************************************************************************
  ����(ģ��)����:void SectorErase_SST29(unsigned int SectorAddr)
  ����:	
  �������:         	              		
  �������:         	 		   		 
  ����˵��: ������ַ�����16λ����������4Mλʵ��ʹ��12λ��ַ��4096������.
*******************************************************************************/
unsigned char SectorErase_SST29(unsigned int SectorAddr)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //6�ֽڲ�������
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X80);
    ParallWritData(0X555,0XAA);
    ParallWritData(0X2AA,0X55);
    
    ParallWritData((unsigned long)SectorAddr,0X20);
    
    __delay_cycles(100);                 // Delay;
    CheckToggleBit(0);
    
    Parallel_CE_H;
    return 1;
}

unsigned char BlockErase_SST29( unsigned int BlockAddr)
{
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadID(unsigned char Manu_or_Device)
  ����:	���豸��ʶ��
  �������:��Ҫ��ȡ�ı�ʶ��         	              		
  �������: �豸��ʶ��        	 		   		 
  ����˵��: 0����ȡ�豸���̱�־ BF (FOR SST)��1����ȡ�豸��ʶ�� 13 ��FOR SST29SF040��
*******************************************************************************/
unsigned char ChipErase_SST29(void)
{
    Parallel_CE_L;
    
    ParallWritData(0X555,0XAA);             //����IDģʽ
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X80);
    
    ParallWritData(0X555,0XAA);             //����IDģʽ
    ParallWritData(0X2AA,0X55);
    ParallWritData(0X555,0X10);
    
    Parallel_CE_H;
    
    __delay_cycles(1000);                 // Delay;
    
    CheckToggleBit(0);    
    //CheckDQ7Poling(0);
    return 1;
    
}