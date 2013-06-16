#ifndef  __DownChannel0_Module_H
#define  __DownChannel0_Module_H


/******************************************************************************
  ����(ģ��)����:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  ����:��һϵ���ֽڽ����ۼ����	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: �ֽڵ��ۼӺ�         	 		   		 
  ����˵��:�����ڲ�����static 
*******************************************************************************/
unsigned char ReadMeterList(void);
void EnMeterReadTask(void);

/******************************************************************************
  ����(ģ��)����:void DownChanne0_Init(void)
  ����:	����ͨ��ͨѶ���������ú���
  �������:��         	              		
  �������: ��        	 		   		 
  ����˵��:Ĭ��ʹ��2400�����ʣ�żУ�飬8λ����λ��1λֹͣλ
*******************************************************************************/
void DownChanne0_Init(void);

unsigned char DownChanne0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent);

/******************************************************************************
  ����(ģ��)����:void DownChanne0_CountTaskTime(void)
  ����:	����ͨ�������ʱ����
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:
******************************************************************************/
void DownChanne0_CountTaskTime(void);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOn(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
*******************************************************************************/
unsigned char ReadMeasuData_TaskOn(void);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOFF(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
*******************************************************************************/
void ReadMeasuData_TaskOFF(void);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  ����:	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
*******************************************************************************/
extern unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  ����:	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
*******************************************************************************/
unsigned char SeareDevcie_TaskRequ(unsigned char DeviceType,unsigned char *DeviceIDBuff,
                                     unsigned char SeareLent);

unsigned char DownChanne0_ReadStatus(void);

/******************************************************************************
  ����(ģ��)����:void DownChanne0_TaskProcess(void)
  ����:	����ͨ��0����������
  �������:��         	              		
  �������:         	 		   		 
  ����˵��:����ͨ��������������ڴ��������ڴ���
*******************************************************************************/
void DownChanne0_TaskProcess(void);

unsigned char ReadDownChanne0_TaskStatus(void);
unsigned char ReadDownChanne0_TaskID(void);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOFF(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
*******************************************************************************/
void SeareDevcie_TaskOFF(void);

unsigned char ReadDateMeterInfo(void);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOFF(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
*******************************************************************************/
void ReadMeasuData_TaskOFF(void);

#endif