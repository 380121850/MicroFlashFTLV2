//************************************************************************
//        ���ݴ洢map���Ը��������ṩ��дAPI
//            
//************************************************************************
#include ".\Storage_Module\StorageManage.H"
#include "DatasSruct.H"


//****************************************************************************//
//********                                                          **********//
//********               �ɼ���������Ϣ����API                      **********//
//********                                                          **********//
//˵������Ҫ�ǲɼ���һЩ������Ϣ�ṹ��Ķ�дAPI����
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  ����:	�������ļ�ϵͳ����Ϣͷ�������˲ɼ���һЩ��Ϣ
  �������: �ṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
{
    return Storage_Module_Read( CollectorInfoTable_Addr,sizeof(struct FileBasicTableStruct),(unsigned char *)FileBasicTable ); 
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  ����:	�������ļ�ϵͳ����Ϣͷ�������˲ɼ���һЩ��Ϣ
  �������: �ṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteCollectorInfo( struct FileBasicTableStruct *FileBasicTable)
{
    return Storage_Module_Write( CollectorInfoTable_Addr,sizeof(struct FileBasicTableStruct),(unsigned char *)FileBasicTable );
}





//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ����Ͳ���API                      **********//
//********                                                          **********//
//˵������Ҫ�ǲɼ����洢�Ĵӻ�������Ϣ�Ķ�дAPI����
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  ����:	���豸������Ŀ
  �������:�豸���ͣ��豸���ͽṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: ����ƥ�䡢��Ŀ��Ч״̬���ŷ���
*******************************************************************************/
unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
{
    return Storage_Module_Read( DeviceTypeTable_Addr,sizeof(struct DeviceTypeStruct),(unsigned char *)( DeviceTypeItem ) );
}

/******************************************************************************
  ����(ģ��)����:unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  ����:	д�豸������Ŀ
  �������:     	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
{
    return Storage_Module_Write( DeviceTypeTable_Addr,sizeof(struct DeviceTypeStruct),(unsigned char *)( DeviceTypeItem ) );
}




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
                                 struct DeviceListStruct *DeviceListItem)
{
    unsigned long StartAddr;
    if( DeviceListNum < DeviceCntMax )
    {
        StartAddr = DeviceListTable_Addr + sizeof(struct DeviceListStruct)*DeviceListNum;  //ȡ���豸�б���ַ
        return Storage_Module_Read( StartAddr,sizeof( struct DeviceListStruct ),(unsigned char *)( DeviceListItem ) );
    }
    return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	ָ�����д���б���Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
{
    unsigned long StartAddr;
    if( DeviceListNum < DeviceCntMax )
    {
        StartAddr = DeviceListTable_Addr + sizeof(struct DeviceListStruct)*DeviceListNum;  //ȡ���豸�б���ַ
        return Storage_Module_Write( StartAddr,sizeof( struct DeviceListStruct ),(unsigned char *)( DeviceListItem ) );
    }
    return 0;
}



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
                                 struct DeviceCurrentData *CurrenData)
{
    return Storage_Module_Read( DeviceDataAddr,sizeof( struct DeviceCurrentData ),(unsigned char *)( CurrenData ) );
}



/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
{
    return Storage_Module_Write( DeviceDataAddr,sizeof( struct DeviceCurrentData ),(unsigned char *)( CurrenData ) );
}




/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData)
{
    return Storage_Module_Read( DeviceDataAddr,sizeof( struct DeviceHistoryData ),(unsigned char *)( HistoryData ) );
}



/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData)
{
    return Storage_Module_Write( DeviceDataAddr,sizeof( struct DeviceHistoryData ),(unsigned char *)( HistoryData ) );
}