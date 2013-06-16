  
//************************************************************************
//  Lierda, Inc      
//************************************************************************


#include "msp430x54x.h"
#include "stdlib.h"
#include ".\USCIA0_UART_Module\USCIA0_UART_Module.H"
#include "..\DownChannel1_Module\USCIA2_UART_Module\USCIA2_UART_Module.H"
#include "..\..\Storage_Module\Storage_Module.H"
#include "Down0CJ188_Module.H"


/******************************************************************************
  ����(ģ��)����:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  ����:��һϵ���ֽڽ����ۼ����	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: �ֽڵ��ۼӺ�         	 		   		 
  ����˵��:�����ڲ�����
*******************************************************************************/
unsigned char Down0CJ188_SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    unsigned char SumCheck = 0X00;
    for(unsigned int i=0;i<SumCheckLent;i++) //�ۼ����
      SumCheck = SumCheck + *SumCheckBuff++;
    return SumCheck;                         //�����ۼ�ֵ
}

/******************************************************************************
  ����(ģ��)����:static unsigned char FrameCheckSum(struct Up0FrameStruct *FrameTemp)
  ����:���֡У���	
  �������:         	              		
  �������:         	 		   		 
  ����˵��:�����ڲ�����
*******************************************************************************/
unsigned char Down0CJ188_FrameCheckSum(struct Down0FrameStruct *FrameTemp)
{
    if(FrameTemp->FrameBuffer[FrameTemp->FrameLent-1-1] == Down0CJ188_SumCheck( FrameTemp->FrameBuffer,FrameTemp->FrameLent-1-1 ) )
      return 1;
    else
      return 0;
}


/******************************************************************************
  ����(ģ��)����:CJ188_CommendPack(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
  ����:	188Э��������
  �������: �豸���͡��豸ID�������룬���ݳ����Լ����ݻ�����
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ڴ治��򴮿�æ���ᵼ��ʧ�ܡ�
*******************************************************************************/
unsigned char CJ188_CommendPackSend_USCIA0(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
{
    unsigned int i;
    unsigned char *FrameBuffer;
    FrameBuffer = (unsigned char *)malloc(15+DataLent); //����֡���ݿռ�
    if( NULL == FrameBuffer )//û�����뵽���򷵻�
        return 0;
    else
    {
        FrameBuffer[0] = 0xFE;
        FrameBuffer[1] = 0xFE;
        FrameBuffer[2] = 0x68;         //֡��ʼ�ַ�
        FrameBuffer[3] = MeterType;    //�豸����
        for(i=4;i<11;i++)              //�豸ID
          FrameBuffer[i] = *MeterIDbuff++;
        FrameBuffer[11] = CTL_Code;    //������
        FrameBuffer[12] = DataLent;    //���ݳ���
        for(i=13;i<DataLent+13;i++)    //������
          FrameBuffer[i] = *DataBuffer++;
        FrameBuffer[i++] = Down0CJ188_SumCheck( &FrameBuffer[2],11+DataLent );         //У����
        FrameBuffer[i] = 0x16;         //ֹ֡ͣ�ַ�
        
        i = USCIA0_MultiByteTX(FrameBuffer,15+DataLent);
        
        free(FrameBuffer);            //�ͷ����뵽�Ŀռ�
        return i;
    }
}


/******************************************************************************
  ����(ģ��)����:unsigned char GetFrame(struct FrameStruct *FrameTemp)
  ����:	�ӽ��ջ������м��һ֡����
  �������:֡���ݽṹ��ָ�룬�����洢�����֡����         	              		
  �������: 1�ɹ���0��֡�������         	 		   		 
  ����˵��:�����֡���ݣ�����ȷ���������ģ�����֡ͷ��֡β�������򳤶��Լ�����֡
           ���ݵĺϷ����ȣ�����׼��188Э����.
ע:�����֡�ڵ�У��͡��豸�����Լ��豸ID��������
*******************************************************************************/
unsigned char Down0CJ188_GetFrame(struct Down0FrameStruct *FrameTemp)
{
    unsigned int  BufferLent;
    unsigned char ReadData;
    BufferLent = USCIA0_ReadRXBuffSize();          //ȡ�õ�ǰ��������С
    USCIA0_FrameDec();
    if(BufferLent >= 13)     //����68��һ֡��������13���ֽ�
    {
        while(BufferLent--)
        { 
          if( USCIA0_ReadRXBuffer(&ReadData) )
          {
            if( 0x68 == ReadData)   //ȡ�û�����һ���ֽ�����//֡ͷ�ж�
            {
                if(BufferLent > 12) 
                {
                    ReadData = USCIA0_ReadBufferOffset(9);
                    if( BufferLent >= (10+ReadData+2) )
                    {
                      ReadData = USCIA0_ReadBufferOffset( 9+ReadData+1+1 );
                        if(ReadData == 0x16)
                        {
                            ReadData = USCIA0_ReadBufferOffset(9);
                            FrameTemp->FrameBuffer[0]=0x68;
                            FrameTemp->FrameLent = 13+ ReadData;
                            for(unsigned int i=0;i<FrameTemp->FrameLent-1;i++)
                            {
                                USCIA0_ReadRXBuffer( &FrameTemp->FrameBuffer[i+1] );
                            }
                            return 1;
                        }
                        continue;
                    }
                    continue;
                }
                else return 0;//��ǰ����һ֡����
            }
          }
          else return 0;
        }
        return 0;     //�Ѿ�ȡ������
    }
    return 0;        //���������ݳ��Ȳ���
}