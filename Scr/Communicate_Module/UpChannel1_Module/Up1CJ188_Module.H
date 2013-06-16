#ifndef  __Up0CJ188_Module_H
#define  __Up0CJ188_Module_H


#define Up1FrameMaxSize  64                    //֡��С
struct Up1FrameStruct
{
    unsigned int FrameLent;                      //֡����
    unsigned char FrameBuffer[Up1FrameMaxSize];  //֡������
};

/******************************************************************************
  ����(ģ��)����:void CJ188_ReceivProcess(void)
  ����:188Э����մ���������
  �������:��         	              		
  �������:��         	 		   		 
  ����˵��:���ú󣬻��ѯһ�ν��ջ�����������֡��־λ�򻺳�����������������������
*******************************************************************************/
unsigned char Up0CJ188_ReceivProcess(struct Up1FrameStruct *FrameTemp);

/******************************************************************************
  ����(ģ��)����:CJ188_CommendPack(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
  ����:	188Э��������
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
����˵��: 
*******************************************************************************/
unsigned char CJ188_CommendPackSend(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer);


/******************************************************************************
  ����(ģ��)����:unsigned char Up1CJ188_ReceivProcess(struct Up1FrameStruct *FrameTemp)
  ����:188Э����մ���������
  �������:��         	              		
  �������:��         	 		   		 
  ����˵��:���ú󣬻��ѯһ�ν��ջ�����������֡��־λ�򻺳�����������������������
*******************************************************************************/
unsigned char Up1CJ188_ReceivProcess(struct Up1FrameStruct *FrameTemp);

#endif