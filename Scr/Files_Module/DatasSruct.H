#ifndef  __DatasSruct_H
#define  __DatasSruct_H

#include "DataMapping.H"


//****************************************************************************//
//********                                                          **********//
//********               �ɼ���������Ϣ����API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  ����:	�������ļ�ϵͳ����Ϣͷ�������˲ɼ���һЩ��Ϣ
  �������: �ṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  ����:	�������ļ�ϵͳ����Ϣͷ�������˲ɼ���һЩ��Ϣ
  �������: �ṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteCollectorInfo(struct FileBasicTableStruct *FileBasicTable);






//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ����Ͳ���API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  ����:	���豸������Ŀ
  �������:�豸���ͣ��豸���ͽṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: ����ƥ�䡢��Ŀ��Ч״̬���ŷ���
*******************************************************************************/
unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem );


/******************************************************************************
  ����(ģ��)����:unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  ����:	д�豸������Ŀ
  �������:     	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem );







//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ��б�����API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	���豸�б���Ŀ
  �������:�豸���ͣ��豸ID�ż��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��:�����豸���͡��豸ID�ż���Ŀ��Чλ 
*******************************************************************************/
unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem);


/******************************************************************************
  ����(ģ��)����:unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	���豸�б���Ŀ
  �������:�豸���ͣ��豸ID�ż��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��:�����豸���͡��豸ID�ż���Ŀ��Чλ 
*******************************************************************************/
unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem);








//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ��б�����API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem);

/******************************************************************************
  ����(ģ��)����:unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	ָ�����д���б���Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem);



//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ����ݲ���API                      **********//
//********                                                          **********//
//****************************************************************************//
/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData);

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData);


/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData);



#endif