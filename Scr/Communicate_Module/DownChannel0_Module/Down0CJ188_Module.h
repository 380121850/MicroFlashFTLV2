#ifndef  __Down0CJ188_Module_H
#define  __Down0CJ188_Module_H

#define Down0FrameMaxSize  64                    //֡��С
struct Down0FrameStruct
{
    unsigned int FrameLent;                   //֡����
    unsigned char FrameBuffer[Down0FrameMaxSize];  //֡������
};


/******************************************************************************
  ����(ģ��)����:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  ����:��һϵ���ֽڽ����ۼ����	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: �ֽڵ��ۼӺ�         	 		   		 
  ����˵��:�����ڲ�����
*******************************************************************************/
unsigned char Down0CJ188_SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent);

/******************************************************************************
  ����(ģ��)����:static unsigned char FrameCheckSum(struct Up0FrameStruct *FrameTemp)
  ����:���֡У���	
  �������:         	              		
  �������:         	 		   		 
  ����˵��:�����ڲ�����
*******************************************************************************/
unsigned char Down0CJ188_FrameCheckSum(struct Down0FrameStruct *FrameTemp);


/******************************************************************************
  ����(ģ��)����:CJ188_CommendPack(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
  ����:	188Э��������
  �������: �豸���͡��豸ID�������룬���ݳ����Լ����ݻ�����
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ڴ治��򴮿�æ���ᵼ��ʧ�ܡ�
*******************************************************************************/
unsigned char CJ188_CommendPackSend_USCIA0(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char GetFrame(struct FrameStruct *FrameTemp)
  ����:	�ӽ��ջ������м��һ֡����
  �������:֡���ݽṹ��ָ�룬�����洢�����֡����         	              		
  �������: 1�ɹ���0��֡�������         	 		   		 
  ����˵��:�����֡���ݣ�����ȷ���������ģ�����֡ͷ��֡β�������򳤶��Լ�����֡
           ���ݵĺϷ����ȣ�����׼��188Э����.
ע:�����֡�ڵ�У��͡��豸�����Լ��豸ID��������
*******************************************************************************/
unsigned char Down0CJ188_GetFrame(struct Down0FrameStruct *FrameTemp);


#endif