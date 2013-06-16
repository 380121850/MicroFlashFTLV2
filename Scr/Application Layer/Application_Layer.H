#ifndef  __Application_Layer_H
#define  __Application_Layer_H

/******************************************************************************
  ����(ģ��)����:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  ����:�ж������ֽڴ��Ƿ����	
  �������:�ַ���ָ��1���ַ���ָ��2������         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned int Lent);

/******************************************************************************
  ����(ģ��)����:void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
  ����:�ַ�������	
  �������:�ַ���ָ��1���ַ���ָ��2������         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent);

/******************************************************************************
  ����(ģ��)����:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  ����:��һϵ���ֽڽ����ۼ����	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: �ֽڵ��ۼӺ�         	 		   		 
  ����˵��:�����ڲ�����static 
*******************************************************************************/
unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent);

/******************************************************************************
  ����(ģ��)����:unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff)
  ����:	�Ƚϲɼ���ID������������ID�Ƿ�һ��
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
 ����˵��: 
*******************************************************************************/
unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff)
  ����:	���ɼ���������Ϣ
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����48�ֽڵĻ������׵�ַ
*******************************************************************************/
unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceID(unsigned char *IDBuffer)
  ����:	�ṩ�ɼ���ID��
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDeviceID(unsigned char *IDBuffer);


/******************************************************************************
  ����(ģ��)����:unsigned char WriteDeviceID(unsigned char *IDBuffer)
  ����:	�޸Ĳɼ�����ַ
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char WriteDeviceID(unsigned char *IDBuffer);



/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	�������������ʵ�ֺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �����ṩ�����������ݣ�һ���ֽ�
*******************************************************************************/
unsigned char MeterReadDataRead( unsigned char *DataBuffer );

/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataWrite( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	�������մ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
����˵��: 
*******************************************************************************/
unsigned char MeterReadDataWrite(unsigned char *MeterReadDateBuff );


/******************************************************************************
  ����(ģ��)����:unsigned char CheckReadMeterData(void)
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �ڲ��洢�� �� ʱ �յĸ�ʽ��Э��ֻ�ṩ �� ���� 
*******************************************************************************/
unsigned char CheckReadMeterData(void);

/******************************************************************************
  ����(ģ��)����:unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff )
  ����:	��չ�ı��Ŷ�
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �������Ϊ �豸���� ��� ����
*******************************************************************************/
unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff );

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceItemNum(unsigned char DeviceType,unsigned char *DeviceIDBuff,unsigned char *Num)
  ����:	�����豸���͡����Ŷ�ȡ������ڲɼ����洢���б��е����
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ReadDeviceItemNum(unsigned char DeviceType,unsigned char *DeviceIDBuff,unsigned char *Num);

/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ExtWriteDeviceItem( unsigned char DeviceType,unsigned char *DeviceIDBuff );


/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ExtWriteDeviceTypeItem( unsigned char DeviceType );


/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ExtDelDeviceTypeItem( unsigned char DeviceType );


/******************************************************************************
  ����(ģ��)����:unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �������� ���NUM,�豸���ͣ��豸���� 4���ֽ�
            Ĭ��ȡ��1���豸����
*******************************************************************************/
unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer );

/******************************************************************************
  ����(ģ��)����:unsigned char CTL_MBUSON(void)
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char CTL_MBUSON(void);

/******************************************************************************
  ����(ģ��)����:unsigned char CTL_MBUSON(void)
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char CTL_MBUSOFF(void);


/******************************************************************************
  ����(ģ��)����:unsigned char ClearDeviceList(unsigned char DeviceTypeNum)
  ����:	�豸�б����
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ClearDeviceList(unsigned char DeviceTypeNum);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurrenData(unsigned char DeviceListNum,unsigned char *DataBuffer)
  ����:	��ĳ���豸������
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDeviceCurrenData(unsigned char DeviceListNum,unsigned char *DataBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	д��׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetRTCFun(unsigned char *DateBuffer);

/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	����׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadRTCFun(unsigned char *DateBuffer);


/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	д��׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadUSCIA0(unsigned char *DataBuffer);


/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	д��׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteUSCIA0(unsigned char *DataBuffer);


/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	����׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadMeterAllDataFun(unsigned char DeviceListNum, unsigned char *DateBuffer);
/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	�������������ʵ�ֺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �����ṩ�����������ݣ�һ���ֽ�
*******************************************************************************/
unsigned char MeterReadDataRead( unsigned char *DataBuffer );


#endif