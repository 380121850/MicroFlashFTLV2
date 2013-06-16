//************************************************************************
//        Ϊ�ϲ��ṩ�������ݽṹ�Ķ�дAPI
//            
//************************************************************************
#include "stdio.h"
#include "DatasSruct.H"
#include "FileSys_Module.h"



/******************************************************************************
  ����(ģ��)����:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  ����:��һϵ���ֽڽ����ۼ����	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: �ֽڵ��ۼӺ�         	 		   		 
  ����˵��:�����ڲ�����static 
*******************************************************************************/
static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    unsigned char SumCheck = 0X00;
    for(unsigned int i=0;i<SumCheckLent;i++) //�ۼ����
      SumCheck = SumCheck + *SumCheckBuff++;
    return SumCheck;                         //�����ۼ�ֵ
}


/******************************************************************************
  ����(ģ��)����:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  ����:�ж������ֽڴ��Ƿ����	
  �������:�ַ���ָ��1���ַ���ָ��2������         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned int Lent)
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
static void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
{
    while(Lent--)
      *DstAddr++ = *SourceBuff++;
}

/******************************************************************************
  ����(ģ��)����:static void SetBuffZero(unsigned char *DataBuff,unsigned int Lent)
  ����:�ѻ�����������Ϊ0	
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
static void SetBuffZero(unsigned char *DataBuff,unsigned int Lent)
{
    while(Lent--)
      *DataBuff++ = 0;
}





//****************************************************************************//
//********                                                          **********//
//********               �ɼ���������Ϣ����API                      **********//
//********                                                          **********//
//˵������Ҫ�ǲɼ���һЩ������Ϣ�ṹ��Ķ�дAPI����
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char CheckFileSysInfo(void)
  ����:	�����ļ�ϵͳ��Ϣͷ�ĳ�ʼ��
  �������: ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char CheckFileSysInfo(void)
{
    struct FileBasicTableStruct FileBasicTable;
    
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        if( FileBasicTable.CheckSum == SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) ) )  //У���
        {
            if( 
                  ( CollectorType == FileBasicTable.DeviceType ) 
               && ( CurrenFileSYSVer == FileBasicTable.FileSYSVer )
               && ( CurrenManuFacInfo == FileBasicTable.ManufacInfo )
              )  
            {
                return 1;
            }
            else return 0;//�ɼ�����Ϣ����
        }
        else return 0;//У��ͳ���
    }
    else return 0;//����Ϣ����
}



/******************************************************************************
  ����(ģ��)����:unsigned char FileSysInfoInit(void)
  ����:	�����ļ�ϵͳ��Ϣͷ�ĳ�ʼ��
  �������: ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char FileSysInfoInit(void)
{
    struct FileBasicTableStruct FileBasicTable; 
    if( 0 == CheckFileSysInfo() )
    {
        FileBasicTable.DeviceType = CollectorType;               //�������� 0X02 ��ʾ�豸�����ǲɼ���
        SetBuffZero(FileBasicTable.DeviceIDBuff,7);             //�豸ID/�����Ϊ0
        FileBasicTable.FileSYSVer = 0X0100;                     //�ļ�ϵͳ�汾�� Ĭ��Ϊ 0X0100
        FileBasicTable.ManufacInfo = 0X01;                      //����ID,Ĭ��Ϊ0X01
        
        FileBasicTable.ReadMeterData[0] = 0X00;                 //�����ճ�ʼ��Ϊ ÿ���µ�XX�ţ�0ʱ0��
        FileBasicTable.ReadMeterData[1] = 0X00;                 //������Ĭ�ϲ��趨
        FileBasicTable.ReadMeterData[2] = 0XFF;
        
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //У���
        return WriteCollectorInfo(&FileBasicTable);
    }
    else return 1;
}



//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ����Ͳ���API                      **********//
//********                                                          **********//
//˵������Ҫ�ǲɼ����洢�Ĵӻ�������Ϣ�Ķ�дAPI����
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char SetDeviceType( unsigned char DeviceType )
  ����:	���豸����
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDeviceType( unsigned char *DeviceType )
{
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceInfo( &DeviceTypeItem ) )
    {
        if( (DeviceTypeItem.Status & EnBit)&&   //���ñ�־λ
            ( DeviceTypeItem.CheckSum == SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) ) )//У���) 
           )
        {
            *DeviceType = DeviceTypeItem.DeviceType;
            return 1;
        }
        else return 0;
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char SetDeviceType( unsigned char DeviceType )
  ����:	�����豸����
  �������:��Ҫ���õ��豸����        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SetDeviceType( unsigned char DeviceType )
{
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceInfo( &DeviceTypeItem ) )
    {
        DeviceTypeItem.Status |= EnBit;      //�ñ�־λ������
        DeviceTypeItem.DeviceListPoint = DeviceListTable_Addr;
        DeviceTypeItem.DeviceType = DeviceType;
        DeviceTypeItem.CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );  //У���
        return WriteDeviceInfo( &DeviceTypeItem );
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char CheckDeviceType( void )
  ����:	
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char CheckDeviceType( void )
{
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceInfo( &DeviceTypeItem ) )
    {
        if( (DeviceTypeItem.Status & EnBit) //���ñ�־λ
           && ( DeviceTypeItem.CheckSum == SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) ) )//У���) 
           && (DeviceTypeItem.DeviceListPoint == DeviceListTable_Addr) 
          )
        {
            return 1;
        }
        else return 0;
    }
    else return 0;
}



//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ��б����API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char SearchDevice(unsigned char *DeviceIDBuff,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	�����豸�б���Ŀ
  �������:�豸����ID���ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��:���漰�����ַ���� 
*******************************************************************************/
unsigned char SearchDevice(unsigned char *DeviceIDBuff,
                                 struct DeviceListStruct *DeviceListItem)
{
    for( unsigned int i=0;i<DeviceCntMax;i++ )       //�����豸�б��ڵ��豸
    {
        if( ReadDeviceListItem(i,DeviceListItem) )
        {
            if( IsEqual( DeviceListItem->DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem->Status & EnBit) == 0) )//�ж��豸ID�ż���Чλ
            {   
                return 1;
            }
        }
    }
    return 0;                                       //�����豸ʧ��
}


/******************************************************************************
  ����(ģ��)����:unsigned char Add_Device(unsigned char *DeviceIDBuff)
  ����:	����һ���豸���͵��豸
  �������:��Ҫ���ӵ��豸���ͣ��豸ID��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Add_Device( unsigned char *DeviceIDBuff )
{
    struct DeviceListStruct DeviceListItem;//�豸�б���Ŀ
    if( SearchDevice( DeviceIDBuff,&DeviceListItem) )
    {
        return 0;//�Ѿ��������ӵ��豸
    }
    else
    {
        for( unsigned int i=0;i<DeviceCntMax;i++ )       //�����豸�б��ڵ��豸
        {
            if( ReadDeviceListItem(i,&DeviceListItem)&&( ( DeviceListItem.Status & EnBit) == 1 ) )
            {
                DeviceListItem.Status &= ~EnBit;                           //�ñ�־λ������
                DeviceListItem.Num  = i;
                DeviceListItem.DeviceDataPoint = DeviceDataTable_Addr + sizeof(struct DeviceDataStruct)*i;
                StringCopy(DeviceIDBuff,DeviceListItem.DeviceIDBuff,7);   //�豸ID�Ÿ��ƹ���
                DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //У���
                return WriteDeviceListItem( i,&DeviceListItem);
            }
        } 
        return 0;
    }
}


/******************************************************************************
  ����(ģ��)����:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	����һ���豸���͵��豸
  �������:��Ҫ���ӵ��豸���ͣ��豸ID��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Del_Device( unsigned char *DeviceIDBuff )
{
    struct DeviceListStruct DeviceListItem;//�豸�б���Ŀ
    if( SearchDevice( DeviceIDBuff,&DeviceListItem) )
    {
        if( IsEqual( DeviceListItem.DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem.Status & EnBit) == 0) )
        {   //�ж��豸ID�ż���Чλ
            DeviceListItem.Status |= EnBit;       //���־λ������Ч
            DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //У���
            return WriteDeviceListItem( DeviceListItem.Num,&DeviceListItem);
        }
        else return 0;
    }
    else return 0;//������
}


/******************************************************************************
  ����(ģ��)����:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	����һ���豸���͵��豸
  �������:��Ҫ���ӵ��豸���ͣ��豸ID��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Del_allDevice(void)
{
    struct DeviceListStruct DeviceListItem;//�豸�б���Ŀ
    for( unsigned int i=0;i<DeviceCntMax;i++ )       //�����豸�б��ڵ��豸
    {
        if( ReadDeviceListItem(i,&DeviceListItem)&&( ( DeviceListItem.Status & EnBit) == 0 ) )
        {
            DeviceListItem.Status |= EnBit;       //���־λ������Ч
            DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //У���
            if( WriteDeviceListItem( i,&DeviceListItem) == 0)
              return 0;
        }
    }
    return 1;       
}


/******************************************************************************
  ����(ģ��)����:unsigned char SearchDevice(unsigned char *DeviceIDBuff,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	�����豸�б���Ŀ
  �������:�豸����ID���ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��:���漰�����ַ���� 
*******************************************************************************/
void CheckDeviceList(void)
{ 
    struct DeviceListStruct DeviceListItem;
    
    printf("\n CheckDeviceList: ");
    for( unsigned int i=0;i<DeviceCntMax;i++ )       //�����豸�б��ڵ��豸
    {
        if( ReadDeviceListItem(i,&DeviceListItem)
           && ( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )
           )
        {
            printf("\n List %d is OK!",i);
        }
        else
        {
            printf("\n List %d is Fail!",i);
        }
    }
}

//****************************************************************************//
//********                                                          **********//
//********               �ɼ����ӻ����ݲ���API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б�ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadDeviceCurrentData(unsigned char *DeviceIDBuffer,
                                 struct DeviceCurrentData *CurrentData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //У���
        {
            return ReadCurreData( DeviceListItem.DeviceDataPoint,CurrentData);
        }
        else return 0;
    }
    else return 0;
}



/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  ����:	ָ����Ŷ����б�����Ŀ
  �������:�洢��ţ��б�ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteDeviceCurrentData(unsigned char *DeviceIDBuffer,
                                 struct DeviceCurrentData *CurrentData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //У���
        {
            return WriteCurreData( DeviceListItem.DeviceDataPoint,CurrentData);
        }
        else return 0;
    }
    else return 0;
}




/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
  ����:	����ָ��ID�ŵĴӱ��ָ���·ݵ���ʷ����
  �������:�ӱ��ID�ţ��·ݣ������ݴ洢�ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: �·ݣ�0~11���ֱ��ʾ1~12�·�
*******************************************************************************/
unsigned char ReadDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //У���
        {
            return ReadHistoryData( ( DeviceListItem.DeviceDataPoint 
                                     + sizeof( struct DeviceCurrentData ) //��ǰ����ƫ��
                                       + sizeof( struct DeviceHistoryData )*Month ),HistoryData);
        }
        else return 0;
    }
    else return 0;
}



/******************************************************************************
  ����(ģ��)����:unsigned char  WriteDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
  ����:	д��ָ��ID�ŵĴӱ��ָ���·ݵ���ʷ����
  �������:�ӱ��ID�ţ��·ݣ������ݴ洢�ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��: �·ݣ�0~11���ֱ��ʾ1~12�·�
*******************************************************************************/
unsigned char WriteDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //У���
        {
            return  WriteHistoryData( ( DeviceListItem.DeviceDataPoint 
                                     + sizeof( struct DeviceCurrentData ) //��ǰ����ƫ��
                                       + sizeof( struct DeviceHistoryData )*Month ),HistoryData);
        }
        else return 0;
    }
    else return 0;
}






/******************************************************************************
  ����(ģ��)����:unsigned char FileSysInit(void)
  ����:	
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char FileSysInit(void)
{
    Storage_Module_Init();
    printf("\n Storage_Module_Init is OK!");
    
    if( 0 == CheckFileSysInfo() )
        printf("\n FileSysInfo Init is Fail!");
    else 
        printf("\n FileSysInfo Init is OK!");
    
    if( 0 == CheckDeviceType() )
        printf("\n CheckDeviceType is Fail!");
    else 
        printf("\n CheckDeviceType is OK!");
    
    CheckDeviceList();
    
    return 1;
}