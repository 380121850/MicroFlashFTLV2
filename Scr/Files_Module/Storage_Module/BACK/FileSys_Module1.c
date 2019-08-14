  
//************************************************************************
//  Lierda, Inc       �洢��ʽ
//************************************************************************
//                 MSP430F5438
#include "msp430x54x.h"
#include "stdlib.h"
#include "FlashStorage.H"
#include "FileSys_Module.H"
#include "Storage_Module.H"

unsigned char const FFBuffer[sizeof(struct DeviceDataStruct)]={
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0xff
};

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
  ����(ģ��)����:unsigned char ReadBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
  ����:	��ָ����ַ����һ��������Ϣͷ
  �������:������Ϣͷ�ṹ��ָ��        	              		
  �������:1�ɹ���0ʧ�� ���ػ�����Ϣͷ�Ľṹ��          	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char ReadBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
{
    if( Storage_Module_Read( BasicHeadAddr,(unsigned char *)FileBasicTable,sizeof(*FileBasicTable),SPIFlashMod ) )
    {
        if( FileBasicTable->CheckSum == SumCheck( (unsigned char *)FileBasicTable,
                                                 (unsigned char *)( &(FileBasicTable->CheckSum) ) - (unsigned char *)FileBasicTable ) )//����У��
        {
            return 1;
        }
        else return 0;
    }
    else return 0;    
    
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
  ����:	��ָ����ַ����һ��������Ϣͷ
  �������:������Ϣͷ�ṹ��ָ��        	              		
  �������:1�ɹ���0ʧ�� ���ػ�����Ϣͷ�Ľṹ��          	 		   		 
  ����˵��: 
*******************************************************************************/
static unsigned char WriteBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
{
    if( FileBasicTable->CheckSum == SumCheck( (unsigned char *)FileBasicTable,
                                             (unsigned char *)( &(FileBasicTable->CheckSum) ) - (unsigned char *)FileBasicTable ) )//����У��
    {
        if( Storage_Module_Write( BasicHeadAddr,(unsigned char *)FileBasicTable,sizeof(*FileBasicTable),SPIFlashMod ) )
        {
            return 1;
        }
        else return 0;   
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  ����:	�������ļ�ϵͳ����Ϣͷ�������˲ɼ���һЩ��Ϣ
  �������: �ṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
{
    if( ReadBasicHead( CollectorInfoTable_Addr,FileBasicTable ) )
    {
        if( CollectorType == FileBasicTable->DeviceType )
          return 1;
        else return 0;
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  ����:	�������ļ�ϵͳ����Ϣͷ�������˲ɼ���һЩ��Ϣ
  �������: �ṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
{
    return WriteBasicHead( CollectorInfoTable_Addr,FileBasicTable );
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorID(unsigned char *CollectorIDBuff)
  ����:	���ɼ�����ID��
  �������:7�ֽڳ��ȵĻ������׵�ַ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadCollectorID(unsigned char *CollectorIDBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        StringCopy(FileBasicTable.DeviceIDBuff,CollectorIDBuff,7);
        return 1;
    }
    else return 0;
}



/******************************************************************************
  ����(ģ��)����:unsigned char ReadCollectorID(unsigned char *CollectorIDBuff)
  ����:	д�ɼ�����ID��
  �������:7�ֽڳ��ȵĻ������׵�ַ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteCollectorID(unsigned char *CollectorIDBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        StringCopy( CollectorIDBuff,FileBasicTable.DeviceIDBuff,7 );
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //У���
        return WriteCollectorInfo( &FileBasicTable );
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  ����:	���ɼ����������� 3���ֽ�
  �������:3�ֽڳ��ȵĻ������׵�ַ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        StringCopy(FileBasicTable.ReadMeterData,MeterReadDateBuff,3);//ȡ�ó�������
        return 1;
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  ����:	д�ɼ�����������
  �������:1�ֽڳ��ȵĻ������׵�ַ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteMeterReadDate(unsigned char *MeterReadDateBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        StringCopy(MeterReadDateBuff,FileBasicTable.ReadMeterData,3);//��������
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //У���
        return WriteCollectorInfo( &FileBasicTable );
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  ����:	���ɼ����������� 1���ֽ�
  �������:1�ֽڳ��ȵĻ������׵�ַ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadUSCIA0Info(struct CommuniPortInfoStruct *USCIA0Info)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        *USCIA0Info = FileBasicTable.CommuniPortInfo[0];
        return 1;
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  ����:	���ɼ����������� 1���ֽ�
  �������:1�ֽڳ��ȵĻ������׵�ַ        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteUSCIA0Info(struct CommuniPortInfoStruct *USCIA0Info)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
         FileBasicTable.CommuniPortInfo[0] = *USCIA0Info;
         FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //У���
         return WriteCollectorInfo( &FileBasicTable );
    }
    else return 0;
}


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
               && ( FileBasicTable.StorageInfo.MaxDeviceType == DeviceTypeCntMax )                     //��ǰ����֧�ֵ����������
               && ( FileBasicTable.StorageInfo.DeviceTypeItemSize == sizeof(struct DeviceTypeStruct) ) //�豸������Ŀ��С �ֽ�
               && ( FileBasicTable.StorageInfo.MaxDevice == DeviceCntMax )                             //ÿ���豸���Ͷ�Ӧ������豸��
               && ( FileBasicTable.StorageInfo.DeviceListItemSize == sizeof(struct DeviceListStruct) ) //�豸�б���Ŀ��С
               && ( FileBasicTable.StorageInfo.DeviceDataSize == sizeof( struct DeviceDataStruct ) )   //�豸���ݴ�С
              )  
            {        //��ǰ������Ϣ
                return 1;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
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
        
        FileBasicTable.StorageInfo.MaxDeviceType = DeviceTypeCntMax;                     //��ǰ����֧�ֵ����������
        FileBasicTable.StorageInfo.DeviceTypeItemSize = sizeof(struct DeviceTypeStruct); //�豸������Ŀ��С �ֽ�
        FileBasicTable.StorageInfo.MaxDevice = DeviceCntMax;                             //ÿ���豸���Ͷ�Ӧ������豸��
        FileBasicTable.StorageInfo.DeviceListItemSize = sizeof(struct DeviceListStruct); //�豸�б���Ŀ��С
        FileBasicTable.StorageInfo.DeviceDataSize = sizeof( struct DeviceDataStruct );   //�豸���ݴ�С

        FileBasicTable.CommuniPortInfo[0].Band = 2400;//Ĭ��ʹ��2400�����ʣ�żУ�飬8λ����λ1λֹͣλ
        FileBasicTable.CommuniPortInfo[0].Pariy = 2;
        FileBasicTable.CommuniPortInfo[0].Data = 8;
        FileBasicTable.CommuniPortInfo[0].Stop = 1;
        
        FileBasicTable.CommuniPortInfo[1].Band = 9600;//Ĭ��ʹ��9600�����ʣ�żУ�飬8λ����λ1λֹͣλ
        FileBasicTable.CommuniPortInfo[1].Pariy = 2;
        FileBasicTable.CommuniPortInfo[1].Data = 8;
        FileBasicTable.CommuniPortInfo[1].Stop = 1;
        
        FileBasicTable.CommuniPortInfo[2].Band = 9600;//Ĭ��ʹ��9600�����ʣ�żУ�飬8λ����λ1λֹͣλ
        FileBasicTable.CommuniPortInfo[2].Pariy = 2;
        FileBasicTable.CommuniPortInfo[2].Data = 8;
        FileBasicTable.CommuniPortInfo[2].Stop = 1;
        
        FileBasicTable.CommuniPortInfo[3].Band = 115200;//Ĭ��ʹ��115200�����ʣ���У�飬8λ����λ1λֹͣλ
        FileBasicTable.CommuniPortInfo[3].Pariy = 0;
        FileBasicTable.CommuniPortInfo[3].Data = 8;
        FileBasicTable.CommuniPortInfo[3].Stop = 1;
        
        FileBasicTable.ReadMeterData[0] = 0X00;                 //�����ճ�ʼ��Ϊ ÿ���µ�XX�ţ�0ʱ0��
        FileBasicTable.ReadMeterData[1] = 0X00;                 //������Ĭ�ϲ��趨
        FileBasicTable.ReadMeterData[2] = 0XFF;
        
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //У���
        FlashModule_ChipErase();
        DeviceStructInit(); //�洢�ṹ��ʼ��
        return WriteCollectorInfo(&FileBasicTable);
    }
    else return 1;
}



/******************************************************************************/
/*                           �豸�����б�                                     */
/******************************************************************************/

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceTypeItem(unsigned char DeviceType,struct DeviceTypeStruct *DeviceTypeItem)
  ����:	���豸������Ŀ
  �������:�豸���ͣ��豸���ͽṹ��ָ��       	              		
  �������:          	 		   		 
  ����˵��: ����ƥ�䡢��Ŀ��Ч״̬���ŷ���
*******************************************************************************/
unsigned char ReadDeviceTypeItem(unsigned char DeviceType,struct DeviceTypeStruct *DeviceTypeItem)
{
    unsigned long StartAddr = DeviceTypeTable_Addr;
    for( unsigned int i=0;i<DeviceTypeCntMax;i++ )
    {
        if( Storage_Module_Read( StartAddr,(unsigned char *)( DeviceTypeItem ),sizeof(*DeviceTypeItem),SPIFlashMod ) )
        {
            if( (DeviceTypeItem->DeviceType == DeviceType) && ( DeviceTypeItem->Status & EnBit ) ) 
            {
                return 1;
            }
        }
         StartAddr = StartAddr + sizeof(*DeviceTypeItem);//ָ����һ����Ŀ
    }
    return 0;
}



/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceTypeExt(unsigned char Num,unsigned char *DeviceType)
  ����:	���豸������Ŀ�е��豸����
  �������:       	              		
  �������:          	 		   		 
  ����˵��: ����ƥ�䡢��Ŀ��Ч״̬���ŷ���
*******************************************************************************/
unsigned char ReadDeviceTypeExt(unsigned char Num,unsigned char *DeviceType)
{
    unsigned long StartAddr = DeviceTypeTable_Addr;
    struct DeviceTypeStruct DeviceTypeItem;
    
    StartAddr = StartAddr + (unsigned long)sizeof(&DeviceTypeItem)*Num;    //ָ����һ����Ŀ
    if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod ) )
    {
        if(  DeviceTypeItem.Status & EnBit  ) 
        {
            *DeviceType = DeviceTypeItem.DeviceType;
            return 1;
        }
        else return 0;
    }
    else return 0;
}



/******************************************************************************
  ����(ģ��)����:unsigned char Add_DeviceType(unsigned char DeviceType)
  ����:	����һ���豸����
  �������:��Ҫ���ӵ��豸����        	              		
  �������:          	 		   		 
  ����˵��:���Ѿ��б����Ѿ������������ͣ��򷵻�ʧ�ܡ� 
*******************************************************************************/
unsigned char Add_DeviceTypeItem(unsigned char DeviceType)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )
    {
        return 0;   //����ʧ�ܣ��豸�����Ѿ����ڲ�����Ч
    }
    else
    {//�豸���Ͳ����ڻ�����Ч
        for( unsigned int i=0;i<DeviceTypeCntMax;i++ )
        {
             if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod ) )
             {
                 if( (DeviceTypeItem.Status & EnBit) == 0)//��ǰ��Ŀ��û�б�����
                 {
                     DeviceTypeItem.Status |= EnBit;      //�ñ�־λ������
                     DeviceTypeItem.DeviceType = DeviceType;
                     DeviceTypeItem.CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );  //У���
                     return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod );
                 }
             }
             StartAddr = StartAddr + sizeof(DeviceTypeItem);//ָ����һ����Ŀ
        }
        return 0;   //�豸�����Ѿ���     
    }
}

/******************************************************************************
  ����(ģ��)����:unsigned char Dell_DeviceType(unsigned char DeviceType)
  ����:	ɾ��һ���豸����
  �������:��Ҫɾ�����豸����        	              		
  �������:          	 		   		 
  ����˵��:�������ڻ��Ѿ�ɾ��״̬���򷵻�ʧ�� 
*******************************************************************************/
unsigned char Dell_DeviceTypeItem(unsigned char DeviceType)
{
    struct DeviceTypeStruct DeviceTypeItem; 
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    for( unsigned int i=0;i<DeviceTypeCntMax;i++ )
    {
         if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod ) )
         {
             if( DeviceTypeItem.DeviceType == DeviceType) 
             {
                 if( (DeviceTypeItem.Status & EnBit) == 1)//��Ŀ����״̬
                 {
                      DeviceTypeItem.Status &= ~EnBit;      //���־λ������Ч
                      DeviceTypeItem.CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );  //У���
                      return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod );
                 }
                 else return 0;//ɾ������ʧ�ܣ���Ŀ�Ѿ���Ч״̬
             }
         }
         StartAddr = StartAddr + sizeof(DeviceTypeItem);//ָ����һ����Ŀ
    }
    return 0;  //�Ҳ�������豸����
}


/******************************************************************************/
/*                             �豸�б�                                       */
/******************************************************************************/

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff, 
                                                  struct DeviceListStruct *DeviceListItem)
  ����:	���豸�б���Ŀ
  �������:�豸���ͣ��豸ID�ż��ṹ��ָ��        	              		
  �������:          	 		   		 
  ����˵��:�����豸���͡��豸ID�ż���Ŀ��Чλ 
*******************************************************************************/
unsigned char ReadDeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff, 
                                 struct DeviceListStruct *DeviceListItem)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //�����豸���� ����ָ�����豸���ͣ����豸���ͱ��в���
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;      //ȡ���豸�б��ַ
        for( unsigned int i=0;i<DeviceCntMax;i++ )       //�����豸�б��ڵ��豸
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( DeviceListItem ),sizeof(*DeviceListItem),SPIFlashMod ) )
            {
                if( IsEqual( DeviceListItem->DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem->Status & EnBit) == 1) )
                {            //�ж��豸ID�ż���Чλ
                    return 1;
                }
            }
            StartAddr = StartAddr + sizeof(*DeviceListItem);    //ȡ����һ���豸��Ŀ
        }
        return 0;  //�����豸ʧ��
    }
    else return 0; //�����豸����ʧ��
}

/******************************************************************************
  ����(ģ��)����:unsigned char CountDeviceNum(unsigned char DeviceType)
  ����:	������Ŷ��豸�б���Ŀ
  �������:       	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char CountDeviceNum(unsigned char DeviceType,unsigned int *DeviceNum)
{
    struct DeviceTypeStruct DeviceTypeItem;
    struct DeviceListStruct DeviceListItem;
    unsigned long StartAddr;
    unsigned int Cnt=0;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //�����豸���� ����ָ�����豸���ͣ����豸���ͱ��в���
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;      //ȡ���豸�б��ַ
        for( unsigned int i=0;i<DeviceCntMax;i++ )       //�����豸�б��ڵ��豸
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
            {
                if( ( DeviceListItem.Status & EnBit) == 1 )
                {            //�ж��豸��Чλ
                    Cnt++;
                }
            }
            StartAddr = StartAddr + sizeof(DeviceListItem);    //ȡ����һ���豸��Ŀ
        }
        *DeviceNum = Cnt;
        return 1;
    }
    else return 0; //�����豸����ʧ��
}

/******************************************************************************
  ����(ģ��)����:unsigned char CountDeviceNum(unsigned char DeviceType)
  ����:	������Ŷ��豸�б���Ŀ
  �������:       	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char WriteDeviceNum(unsigned char DeviceType,unsigned int DeviceNum)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //�����豸���� ����ָ�����豸���ͣ����豸���ͱ��в���
    {
        DeviceTypeItem.DeviceCnt = DeviceNum;  //�豸����
        DeviceTypeItem.CheckSum  = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );
        StartAddr = StartAddr + (unsigned long)sizeof(&DeviceTypeItem)*DeviceTypeItem.Num;    //ָ����һ����Ŀ
        return  Storage_Module_Write( StartAddr,(unsigned char *)(&DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod );
    }
    else return 0; //�����豸����ʧ��
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadDeviceListItem_Ext(unsigned char DeviceType,unsigned char Num, 
                                 struct DeviceListStruct *DeviceListItem)
  ����:	������Ŷ��豸�б���Ŀ
  �������:       	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char ReadDeviceListItem_Ext(unsigned char DeviceType,unsigned char Num, 
                                 struct DeviceListStruct *DeviceListItem)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //�����豸���� ����ָ�����豸���ͣ����豸���ͱ��в���
    {
        StartAddr = DeviceTypeItem.DeviceListPoint +  sizeof(*DeviceListItem)*Num;      //ȡ���豸�б��ַ
        if( Storage_Module_Read( StartAddr,(unsigned char *)( DeviceListItem ),sizeof(*DeviceListItem),SPIFlashMod ) )
        {
            if( ( DeviceListItem->Status & EnBit) == 1 )
            {            //�ж���Чλ
                return 1;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0; //�����豸����ʧ��
}


/******************************************************************************
  ����(ģ��)����:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	����һ���豸���͵��豸
  �������:��Ҫ���ӵ��豸���ͣ��豸ID��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Add_DeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff)
{
    struct DeviceTypeStruct DeviceTypeItem;//�豸������Ŀ
    struct DeviceListStruct DeviceListItem;//�豸�б���Ŀ
    
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    if( ReadDeviceListItem(DeviceType,DeviceIDBuff,&DeviceListItem) )
    {
        return 0;//�Ѿ��������ӵ��豸
    }
    else
    {
        if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//ȡ�豸���ͱ�
        {
            StartAddr = DeviceTypeItem.DeviceListPoint;  //ȡ���豸�б��ַ
            for(unsigned int j=0;j<DeviceCntMax;j++)     //���豸�б��в��ҿ���Ŀ
            {
                if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
                {
                   if( (DeviceListItem.Status & EnBit) == 0 )//��ǰ��Ŀ�ǿ�/��Ч
                   {
                       DeviceListItem.Status |= EnBit;                           //�ñ�־λ������
                       StringCopy(DeviceIDBuff,DeviceListItem.DeviceIDBuff,7);   //�豸ID�Ÿ��ƹ���
                       DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //У���
                       return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
                   }
               }
               StartAddr = StartAddr + sizeof(DeviceListItem);//ָ����һ����Ŀ
           }
           return 0;         
        }
        else return 0; //�������豸���ͣ�����0
    }
}


/******************************************************************************
  ����(ģ��)����:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	����һ���豸���͵��豸
  �������:��Ҫ���ӵ��豸���ͣ��豸ID��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Dell_DeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff)
{
    struct DeviceTypeStruct DeviceTypeItem;//�豸������Ŀ
    struct DeviceListStruct DeviceListItem;//�豸�б���Ŀ
    
    unsigned long StartAddr;

    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//ȡ�豸���ͱ�
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;  //ȡ���豸�б��ַ
        for(unsigned int j=0;j<DeviceCntMax;j++)     //���豸�б��в��ҿ���Ŀ
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
            {
                if( IsEqual( DeviceListItem.DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem.Status & EnBit) == 1) )
                { //�ж��豸ID�ż���Чλ
                    DeviceListItem.Status &= ~EnBit;       //���־λ������Ч
                    DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //У���
                    return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
                }
           }
           StartAddr = StartAddr + sizeof(DeviceListItem);//ָ����һ����Ŀ
       }
       return 0;   //û�д��豸��ɾ������ʧ��        
    }
    else return 0; //�������豸���ͣ�����0
}


/******************************************************************************
  ����(ģ��)����:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	����һ���豸���͵��豸
  �������:��Ҫ���ӵ��豸���ͣ��豸ID��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
/*unsigned char Dell_AllDeviceList(unsigned char DeviceType)
{
    struct DeviceTypeStruct DeviceTypeItem;//�豸������Ŀ
    struct DeviceListStruct DeviceListItem;//�豸�б���Ŀ
    unsigned long StartAddr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//ȡ�豸���ͱ�
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;  //ȡ���豸�б��ַ
        for(unsigned int j=0;j<DeviceCntMax;j++)     //���豸�б��в��ҿ���Ŀ
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
            {
                if( ( DeviceListItem.Status & EnBit) == 1 )
                { //�ж��豸ID�ż���Чλ
                    Storage_Module_Write( DeviceListItem.DeviceDataPoint,(unsigned char *)(FFBuffer ),sizeof(struct DeviceDataStruct),SPIFlashMod );
                    DeviceListItem.Status &= ~EnBit;       //���־λ������Ч
                    DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //У���
                    Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
                }
                
           }
           StartAddr = StartAddr + sizeof(DeviceListItem);//ָ����һ����Ŀ
        }
       return 1;        
    }
    else return 0; //�������豸���ͣ�����0
}
*/

/******************************************************************************
  ����(ģ��)����:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	����һ���豸���͵��豸
  �������:��Ҫ���ӵ��豸���ͣ��豸ID��        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char Dell_AllDeviceList(unsigned char DeviceType)
{
    struct DeviceTypeStruct DeviceTypeItem;//�豸������Ŀ
    struct DeviceListStruct *DeviceListItem;//�豸�б���Ŀ
    unsigned char *DeviceListItemBuffer;
    unsigned char Flag=0;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//ȡ�豸���ͱ�
    {
        DeviceListItemBuffer = (unsigned char *)malloc( sizeof(struct DeviceListStruct)*DeviceCntMax ); //����֡���ݿռ�
        if( NULL != DeviceListItemBuffer )//û�����뵽���򷵻�
        {
            if( Storage_Module_Read( DeviceTypeItem.DeviceListPoint,DeviceListItemBuffer,sizeof(struct DeviceListStruct)*DeviceCntMax,SPIFlashMod ) )
            {
                DeviceListItem = (struct DeviceListStruct *)DeviceListItemBuffer;
                for(unsigned int j=0;j<DeviceCntMax;j++)     //���豸�б��в��ҷǿ���Ŀ
                {
                    if( ( DeviceListItem->Status & EnBit) == 1 )
                    { //�ж���Чλ
                        Storage_Module_Write( DeviceListItem->DeviceDataPoint,(unsigned char *)(FFBuffer ),sizeof(struct DeviceDataStruct),SPIFlashMod );
                        DeviceListItem->Status &= ~EnBit;       //���־λ������Ч
                        DeviceListItem->CheckSum = SumCheck( (unsigned char *)(DeviceListItem),(unsigned char *)( &(DeviceListItem->CheckSum) ) - (unsigned char *)(DeviceListItem) );  //У���
                        Flag = 1;
                    }
                    DeviceListItem++;  //ָ����һ����Ŀ 
               }
               if(Flag)
                 Flag = Storage_Module_Write( DeviceTypeItem.DeviceListPoint,DeviceListItemBuffer,sizeof(struct DeviceListStruct)*DeviceCntMax,SPIFlashMod );
               free(DeviceListItemBuffer);            //�ͷ����뵽�Ŀռ�
               return Flag;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
}
/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeterAllData(unsigned char MeterType,unsigned char *MeterIDBuff,
                               unsigned char *ReadBuffer)
  ����:	���ӱ���������
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadMeterAllData(unsigned char MeterType,unsigned char *MeterIDBuff,
                               unsigned char *ReadBuffer)
{
    struct DeviceListStruct DeviceListItem;
    if( ReadDeviceListItem(MeterType,MeterIDBuff,&DeviceListItem) )
    {
        return Storage_Module_Read( DeviceListItem.DeviceDataPoint,ReadBuffer,sizeof(struct DeviceDataStruct),SPIFlashMod );
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char CheckDeviceTypeTable(void)
  ����:	����豸�����б��Ƿ�����
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char CheckDeviceTypeTable(void)
{
    struct DeviceTypeStruct  DeviceTypeItem;  //�豸�����б�
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    for( unsigned int i=0;i<DeviceTypeCntMax;i++ )
    {
        if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod ) )
        {
            if( DeviceTypeItem.CheckSum != SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) ) )
            {
                return 0;
            }
        }
        else return 0;
        StartAddr = StartAddr + sizeof(DeviceTypeItem);//ָ����һ����Ŀ
    }
    return 1;
}


/******************************************************************************
  ����(ģ��)����:unsigned char DeviceTypeStructInit(void)
  ����:	�豸�����б��ʼ��
  �������:        	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char DeviceStructInit(void)
{
    struct DeviceTypeStruct  DeviceTypeItem[DeviceTypeCntMax];  //�豸�����б�
    struct DeviceListStruct  DeviceListItem[DeviceCntMax];      //�豸�б�
    
    if( 0 == CheckDeviceTypeTable() )
    {
        for(unsigned int i=0;i<DeviceTypeCntMax;i++)
        {
           DeviceTypeItem[i].Num = i;
           DeviceTypeItem[i].Status = 0x00;
           DeviceTypeItem[i].DeviceType = 0xFF;
           DeviceTypeItem[i].DeviceListPoint = DeviceListTable_Addr + (unsigned long)i*DeviceCntMax*(sizeof(struct DeviceListStruct) + sizeof(struct DeviceDataStruct));
           for(unsigned int j=0;j<DeviceCntMax;j++)
           {
               DeviceListItem[j].Num = j;
               DeviceListItem[j].Status = 0x00;
               for(unsigned int k=0;k<7;k++)
                 DeviceListItem[j].DeviceIDBuff[k] = 0x00;
               DeviceListItem[j].DeviceDataPoint = DeviceDataTable_Addr + (unsigned long)j*sizeof(struct DeviceDataStruct);
               DeviceListItem[j].CheckSum = SumCheck( (unsigned char *)(&DeviceListItem[j]),(unsigned char *)( &(DeviceListItem[j].CheckSum) ) - (unsigned char *)(&DeviceListItem[j]) );  //У���;;
           }
           DeviceTypeItem[i].DeviceCnt = 0x00;
           DeviceTypeItem[i].Reaserv = 0xFF;
           DeviceTypeItem[i].CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem[i]),(unsigned char *)( &(DeviceTypeItem[i].CheckSum) ) - (unsigned char *)(&DeviceTypeItem[i]) );  //У���;
           
           Storage_Module_Write( DeviceTypeItem[i].DeviceListPoint,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
        }
        return Storage_Module_Write( DeviceTypeTable_Addr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod ); 
    }
    else return 1;
}