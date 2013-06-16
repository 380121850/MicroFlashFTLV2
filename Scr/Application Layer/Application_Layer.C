  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include "..\RTC_Module\RTC_Module.H"   //RTCģ��
#include "..\Sys_Module\Sys_Module.H"   //��Ƭ������ģ��
#include "..\Storage_Module\Storage_Module.H" //FLASHģ��
#include "..\Storage_Module\FileSys_Module.H" //�洢ģ��
#include "Application_Layer.H"                //Ӧ�ò����


/******************************************************************************
  ����(ģ��)����:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  ����:�ж������ֽڴ��Ƿ����	
  �������:�ַ���ָ��1���ַ���ָ��2������         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned int Lent)
{
    for(unsigned int i=0;i<Lent;i++ )
    {
        if( *Buff1++ == *Buff2++ )
          continue;
        else return 0;
    }
    return 1;
}

/******************************************************************************
  ����(ģ��)����:void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
  ����:�ַ�������	
  �������:�ַ���ָ��1���ַ���ָ��2������         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
{
    while(Lent--)
      *DstAddr++ = *SourceBuff++;
}


/******************************************************************************
  ����(ģ��)����:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  ����:��һϵ���ֽڽ����ۼ����	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: �ֽڵ��ۼӺ�         	 		   		 
  ����˵��:�����ڲ�����static 
*******************************************************************************/
unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    unsigned char SumCheck = 0X00;
    for(unsigned int i=0;i<SumCheckLent;i++) //�ۼ����
      SumCheck = SumCheck + *SumCheckBuff++;
    return SumCheck;                         //�����ۼ�ֵ
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff)
  ����:	���ɼ���������Ϣ
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ����48�ֽڵĻ������׵�ַ
*******************************************************************************/
unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo( &FileBasicTable ) )//���ɼ�����Ϣ
    {
        CollectorInfoBuff[0] = FileBasicTable.DeviceType;                  //0X02 ��ʾ�豸�����ǲɼ��� 
        StringCopy( FileBasicTable.DeviceIDBuff,&CollectorInfoBuff[1],7 ); //���豸ID
        StringCopy( (unsigned char *)(&FileBasicTable.FileSYSVer),&CollectorInfoBuff[8],2 ); //�ļ�ϵͳ�汾�� Ĭ��Ϊ 0X0100
        CollectorInfoBuff[10] = FileBasicTable.ManufacInfo ;//����ID,Ĭ��Ϊ0X01
        
        CollectorInfoBuff[11] = FileBasicTable.StorageInfo.MaxDeviceType;      //��ǰ����֧�ֵ����������
        CollectorInfoBuff[12] = FileBasicTable.StorageInfo.DeviceTypeItemSize; //�豸������Ŀ��С �ֽ�
        StringCopy( (unsigned char *)(&FileBasicTable.StorageInfo.MaxDevice),&CollectorInfoBuff[13],2 );//ÿ���豸���Ͷ�Ӧ������豸��
        CollectorInfoBuff[15] = FileBasicTable.StorageInfo.DeviceListItemSize; //�豸�б���Ŀ��С
        StringCopy( (unsigned char *)(&FileBasicTable.StorageInfo.DeviceDataSize),&CollectorInfoBuff[16],2 );//�豸���ݴ�С    
        
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[0]),&CollectorInfoBuff[18],7 );
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[1]),&CollectorInfoBuff[25],7 );
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[2]),&CollectorInfoBuff[32],7 );
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[3]),&CollectorInfoBuff[39],7 );
        
        StringCopy( FileBasicTable.ReadMeterData,&CollectorInfoBuff[46],3 );//�����ճ�ʼ��Ϊ ÿ���µ�XX�ţ�0ʱ0��
        return 1;
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff)
  ����:	�Ƚϲɼ���ID������������ID�Ƿ�һ��
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
 ����˵��: 
*******************************************************************************/
unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff)
{
    unsigned char ReadIDBuff[7];
    if( ReadCollectorID(ReadIDBuff) )//���ɼ���ID
    {
        return IsEqual( ReadIDBuff,CollectorIDBuff,7 );//�Ƚ��豸ID��
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceID(unsigned char *IDBuffer)
  ����:	�ṩ�ɼ���ID��
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDeviceID(unsigned char *IDBuffer)
{
    return ReadCollectorID( IDBuffer );  //���ɼ�����ID��
}

/******************************************************************************
  ����(ģ��)����:unsigned char WriteDeviceID(unsigned char *IDBuffer)
  ����:	�޸Ĳɼ�����ַ
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char WriteDeviceID(unsigned char *IDBuffer)
{        
    return WriteCollectorID(IDBuffer);              //д�ɼ�����ID��
}


/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	�������������ʵ�ֺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �����ṩ�����������ݣ�һ���ֽ�
*******************************************************************************/
unsigned char MeterReadDataRead( unsigned char *DataBuffer )
{
    unsigned char DateBuff[3];
    if( ReadMeterReadDate(DateBuff) )  //ȡ�ó�������
    {
        *DataBuffer = DateBuff[2];
        return 1;
    }
    else
      return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataWrite( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	�������մ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �ڲ��洢�� �� ʱ �յĸ�ʽ��Э��ֻ�ṩ �� ���� 
*******************************************************************************/
unsigned char MeterReadDataWrite(unsigned char *MeterReadDateBuff )
{
    unsigned char DataBuffer[3];
    DataBuffer[0] = 0X00;
    DataBuffer[1] = 0X00;
    DataBuffer[2] = *MeterReadDateBuff;  
    SetAlarm(0x00,0x00,DataBuffer[2] );
    return WriteMeterReadDate( DataBuffer );//д��������  
}

/******************************************************************************
  ����(ģ��)����:unsigned char CheckReadMeterData(void)
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �ڲ��洢�� �� ʱ �յĸ�ʽ��Э��ֻ�ṩ �� ���� 
*******************************************************************************/
unsigned char CheckReadMeterData(void)
{
    unsigned char DateBuffer[3];
    unsigned char RTCBuffer[7];
    if( ReadMeterReadDate(DateBuffer) )
    {
        if( ReadRTC(RTCBuffer) )
        {
            if( (DateBuffer[0] == RTCBuffer[1])
               &&(DateBuffer[1] == RTCBuffer[2])
               &&(DateBuffer[2] == RTCBuffer[3])
              )
              return 1;
            else return 0;
        }
        else return 0;
    }
    else return 0;
  
}

/******************************************************************************
  ����(ģ��)����:unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff )
  ����:	��չ�ı��Ŷ�
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �������Ϊ �豸���� ��� ����
*******************************************************************************/
unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff )
{ 
    struct DeviceListStruct DeviceListItem;
    unsigned char DeviceType;
    
    if( ReadDeviceTypeExt(0,&DeviceType) )//�ȶ�����ǰ�豸���ͺ�
    {
        DataBuff[0] = DeviceType;//�豸����
        DataBuff[1] = ListNum;   //���
        if( ReadDeviceListItem_Ext( DeviceType,ListNum,&DeviceListItem) )//�ٶ����豸�б�ָ������Ŀ
        {
            StringCopy(DeviceListItem.DeviceIDBuff,&DataBuff[2],7);  //ȡ�ñ���
            return 1;
        }
        else return 0;//ȡ�豸����ʧ��
    }
    else return 0; //ȡ�豸���ͺ�ʧ��
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceItemNum(unsigned char DeviceType,unsigned char *DeviceIDBuff,unsigned char *Num)
  ����:	�����豸���͡����Ŷ�ȡ������ڲɼ����洢���б��е����
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ReadDeviceItemNum(unsigned char DeviceType,unsigned char *DeviceIDBuff,unsigned char *Num)
{
    struct DeviceListStruct DeviceListItem;
    if( ReadDeviceListItem( DeviceType,DeviceIDBuff,&DeviceListItem ) )
    {
        *Num = DeviceListItem.Num;
        return 1;
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: �������� ���NUM,�豸���ͣ��豸���� 4���ֽ�
            Ĭ��ȡ��1���豸����
*******************************************************************************/
unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer )
{
    unsigned char DeviceType;
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceTypeExt( 0,&DeviceType ) )//ȡ�õ�һ���豸����
    {
        if( ReadDeviceTypeItem( DeviceType,&DeviceTypeItem) )//ȡ���豸���Ͷ�Ӧ�Ľṹ����Ϣ
        {
            DataBuffer[0] = DeviceTypeItem.Num;
            DataBuffer[1] = DeviceTypeItem.DeviceType;
            CountDeviceNum(DeviceType,&DeviceTypeItem.DeviceCnt);
            DataBuffer[2] = (unsigned char )DeviceTypeItem.DeviceCnt;
            DataBuffer[3] = (unsigned char )(DeviceTypeItem.DeviceCnt>>8);
            return 1;
        }
        else return 0;
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ExtWriteDeviceTypeItem( unsigned char DeviceType )
{ 
    return Add_DeviceTypeItem(DeviceType); //�����豸���ͼ��������ӵ��б���ȥ.
}

/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ExtDelDeviceTypeItem( unsigned char DeviceType )
{ 
    return Dell_DeviceTypeItem( DeviceType );
}

/******************************************************************************
  ����(ģ��)����:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  ����:	
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: ���ݱ����ͼ����ţ����ӵ��б���ȥ
*******************************************************************************/
unsigned char ExtWriteDeviceItem( unsigned char DeviceType,unsigned char *DeviceIDBuff )
{ 
    if( Add_DeviceListItem(DeviceType,DeviceIDBuff) )//�����豸���ͼ��������ӵ��б���ȥ.
    {
        return 1;
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char CTL_MBUSON(void)
  ����:	������
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char CTL_MBUSON(void)
{
    MBUSOn();
    return 1;
}


/******************************************************************************
  ����(ģ��)����:unsigned char CTL_MBUSON(void)
  ����:	�ر�����
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char CTL_MBUSOFF(void)
{
    MBUSOFF();
    return 1;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ClearDeviceList(unsigned char DeviceTypeNum)
  ����:	�豸�б����
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ClearDeviceList(unsigned char DeviceTypeNum)
{
    unsigned char DeviceType;
    if( ReadDeviceTypeExt(DeviceTypeNum,&DeviceType) )//�ȶ�����ǰ��һ���豸���ͺ�
    {     
        return Dell_AllDeviceList(DeviceType);
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceCurrenData(unsigned char DeviceListNum,unsigned char *DataBuffer)
  ����:	��ĳ���豸������
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDeviceCurrenData(unsigned char DeviceListNum,unsigned char *DataBuffer)
{
    unsigned char DeviceType;
    struct DeviceListStruct DeviceListItem;
    
    if( ReadDeviceTypeExt(0,&DeviceType) )
    {    
        if( ReadDeviceListItem_Ext(DeviceType,DeviceListNum, &DeviceListItem) )
        {
            return Storage_Module_Read(DeviceListItem.DeviceDataPoint,DataBuffer,35, SPIFlashMod);
        }
        else return 0;
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	д��׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetRTCFun(unsigned char *DateBuffer)
{
    return SetRTC(DateBuffer);
}

/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	д��׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadUSCIA0(unsigned char *DataBuffer)
{
    struct CommuniPortInfoStruct USCIA0Info;
    if( ReadUSCIA0Info(&USCIA0Info) )
    {
        StringCopy( (unsigned char *)(&USCIA0Info.Band),DataBuffer,4 );
        DataBuffer[4] = USCIA0Info.Pariy;
        DataBuffer[5] = USCIA0Info.Data;
        DataBuffer[6] = USCIA0Info.Stop;
        return 1;
    }
    else
      return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	д��׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteUSCIA0(unsigned char *DataBuffer)
{
    struct CommuniPortInfoStruct USCIA0Info;
    
    StringCopy( DataBuffer,(unsigned char *)(&USCIA0Info.Band),4 );
    USCIA0Info.Pariy = 2;//DataBuffer[4];
    USCIA0Info.Data = 8;//DataBuffer[5];
    USCIA0Info.Stop = 1;//DataBuffer[6];    
    return WriteUSCIA0Info(&USCIA0Info);
}

/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	����׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadRTCFun(unsigned char *DateBuffer)
{
    return ReadRTC(DateBuffer);
}


/******************************************************************************
  ����(ģ��)����:unsigned char SetRTCFun(unsigned char *DateBuffer)
  ����:	����׼ʱ�书�ܺ���
  �������: 
  �������: �ɹ���ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadMeterAllDataFun(unsigned char DeviceListNum, unsigned char *DateBuffer)
{
    struct DeviceListStruct DeviceListItem;
    unsigned char DeviceType;
    if( ReadDeviceTypeExt(0,&DeviceType) )
    {    
        if( ReadDeviceListItem_Ext(DeviceType,DeviceListNum, &DeviceListItem) )
        {
            return ReadMeterAllData(DeviceType,DeviceListItem.DeviceIDBuff,DateBuffer);
        }
        else return 0;
    }
    else return 0;
}