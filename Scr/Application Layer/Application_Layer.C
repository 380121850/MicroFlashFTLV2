  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
#include "msp430x54x.h"
#include "..\RTC_Module\RTC_Module.H"   //RTC模块
#include "..\Sys_Module\Sys_Module.H"   //单片机基本模块
#include "..\Storage_Module\Storage_Module.H" //FLASH模块
#include "..\Storage_Module\FileSys_Module.H" //存储模块
#include "Application_Layer.H"                //应用层服务


/******************************************************************************
  函数(模块)名称:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  功能:判断两串字节串是否相等	
  输入参数:字符串指针1，字符串指针2，长度         	              		
  输出参数:          	 		   		 
  其它说明: 
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
  函数(模块)名称:void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
  功能:字符串复制	
  输入参数:字符串指针1，字符串指针2，长度         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
{
    while(Lent--)
      *DstAddr++ = *SourceBuff++;
}


/******************************************************************************
  函数(模块)名称:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  功能:对一系列字节进行累加求和	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 字节的累加和         	 		   		 
  其它说明:属于内部函数static 
*******************************************************************************/
unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    unsigned char SumCheck = 0X00;
    for(unsigned int i=0;i<SumCheckLent;i++) //累加求和
      SumCheck = SumCheck + *SumCheckBuff++;
    return SumCheck;                         //返回累加值
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff)
  功能:	读采集器基本信息
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 至少48字节的缓冲区首地址
*******************************************************************************/
unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo( &FileBasicTable ) )//读采集器信息
    {
        CollectorInfoBuff[0] = FileBasicTable.DeviceType;                  //0X02 表示设备类型是采集器 
        StringCopy( FileBasicTable.DeviceIDBuff,&CollectorInfoBuff[1],7 ); //读设备ID
        StringCopy( (unsigned char *)(&FileBasicTable.FileSYSVer),&CollectorInfoBuff[8],2 ); //文件系统版本号 默认为 0X0100
        CollectorInfoBuff[10] = FileBasicTable.ManufacInfo ;//厂商ID,默认为0X01
        
        CollectorInfoBuff[11] = FileBasicTable.StorageInfo.MaxDeviceType;      //当前可以支持的最大类型数
        CollectorInfoBuff[12] = FileBasicTable.StorageInfo.DeviceTypeItemSize; //设备类型条目大小 字节
        StringCopy( (unsigned char *)(&FileBasicTable.StorageInfo.MaxDevice),&CollectorInfoBuff[13],2 );//每个设备类型对应的最大设备数
        CollectorInfoBuff[15] = FileBasicTable.StorageInfo.DeviceListItemSize; //设备列表条目大小
        StringCopy( (unsigned char *)(&FileBasicTable.StorageInfo.DeviceDataSize),&CollectorInfoBuff[16],2 );//设备数据大小    
        
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[0]),&CollectorInfoBuff[18],7 );
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[1]),&CollectorInfoBuff[25],7 );
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[2]),&CollectorInfoBuff[32],7 );
        StringCopy( (unsigned char *)(&FileBasicTable.CommuniPortInfo[3]),&CollectorInfoBuff[39],7 );
        
        StringCopy( FileBasicTable.ReadMeterData,&CollectorInfoBuff[46],3 );//抄表日初始化为 每个月的XX号，0时0分
        return 1;
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff)
  功能:	比较采集器ID与命令下来的ID是否一致
  输入参数: 
  输出参数: 成功或失败         	 		   		 
 其它说明: 
*******************************************************************************/
unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff)
{
    unsigned char ReadIDBuff[7];
    if( ReadCollectorID(ReadIDBuff) )//读采集器ID
    {
        return IsEqual( ReadIDBuff,CollectorIDBuff,7 );//比较设备ID号
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceID(unsigned char *IDBuffer)
  功能:	提供采集器ID号
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadDeviceID(unsigned char *IDBuffer)
{
    return ReadCollectorID( IDBuffer );  //读采集器的ID号
}

/******************************************************************************
  函数(模块)名称:unsigned char WriteDeviceID(unsigned char *IDBuffer)
  功能:	修改采集器地址
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明:
*******************************************************************************/
unsigned char WriteDeviceID(unsigned char *IDBuffer)
{        
    return WriteCollectorID(IDBuffer);              //写采集器的ID号
}


/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	读抄表日命令功能实现函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 对外提供抄表日期数据，一个字节
*******************************************************************************/
unsigned char MeterReadDataRead( unsigned char *DataBuffer )
{
    unsigned char DateBuff[3];
    if( ReadMeterReadDate(DateBuff) )  //取得抄表日期
    {
        *DataBuffer = DateBuff[2];
        return 1;
    }
    else
      return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataWrite( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	读抄表日处理
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 内部存储是 分 时 日的格式，协议只提供 日 数据 
*******************************************************************************/
unsigned char MeterReadDataWrite(unsigned char *MeterReadDateBuff )
{
    unsigned char DataBuffer[3];
    DataBuffer[0] = 0X00;
    DataBuffer[1] = 0X00;
    DataBuffer[2] = *MeterReadDateBuff;  
    SetAlarm(0x00,0x00,DataBuffer[2] );
    return WriteMeterReadDate( DataBuffer );//写抄表日期  
}

/******************************************************************************
  函数(模块)名称:unsigned char CheckReadMeterData(void)
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 内部存储是 分 时 日的格式，协议只提供 日 数据 
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
  函数(模块)名称:unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff )
  功能:	扩展的表号读
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 输出数据为 设备类型 序号 表号
*******************************************************************************/
unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff )
{ 
    struct DeviceListStruct DeviceListItem;
    unsigned char DeviceType;
    
    if( ReadDeviceTypeExt(0,&DeviceType) )//先读出当前设备类型号
    {
        DataBuff[0] = DeviceType;//设备类型
        DataBuff[1] = ListNum;   //序号
        if( ReadDeviceListItem_Ext( DeviceType,ListNum,&DeviceListItem) )//再读出设备列表指定的条目
        {
            StringCopy(DeviceListItem.DeviceIDBuff,&DataBuff[2],7);  //取得表号
            return 1;
        }
        else return 0;//取设备表号失败
    }
    else return 0; //取设备类型号失败
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceItemNum(unsigned char DeviceType,unsigned char *DeviceIDBuff,unsigned char *Num)
  功能:	根据设备类型、表号读取这个表在采集器存储的列表中的序号
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
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
  函数(模块)名称:unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 返回数据 序号NUM,设备类型，设备个数 4个字节
            默认取第1个设备类型
*******************************************************************************/
unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer )
{
    unsigned char DeviceType;
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceTypeExt( 0,&DeviceType ) )//取得第一种设备类型
    {
        if( ReadDeviceTypeItem( DeviceType,&DeviceTypeItem) )//取得设备类型对应的结构体信息
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
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char ExtWriteDeviceTypeItem( unsigned char DeviceType )
{ 
    return Add_DeviceTypeItem(DeviceType); //根据设备类型及表号增加到列表中去.
}

/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char ExtDelDeviceTypeItem( unsigned char DeviceType )
{ 
    return Dell_DeviceTypeItem( DeviceType );
}

/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char ExtWriteDeviceItem( unsigned char DeviceType,unsigned char *DeviceIDBuff )
{ 
    if( Add_DeviceListItem(DeviceType,DeviceIDBuff) )//根据设备类型及表号增加到列表中去.
    {
        return 1;
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char CTL_MBUSON(void)
  功能:	打开总线
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char CTL_MBUSON(void)
{
    MBUSOn();
    return 1;
}


/******************************************************************************
  函数(模块)名称:unsigned char CTL_MBUSON(void)
  功能:	关闭总线
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char CTL_MBUSOFF(void)
{
    MBUSOFF();
    return 1;
}


/******************************************************************************
  函数(模块)名称:unsigned char ClearDeviceList(unsigned char DeviceTypeNum)
  功能:	设备列表清除
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ClearDeviceList(unsigned char DeviceTypeNum)
{
    unsigned char DeviceType;
    if( ReadDeviceTypeExt(DeviceTypeNum,&DeviceType) )//先读出当前第一种设备类型号
    {     
        return Dell_AllDeviceList(DeviceType);
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurrenData(unsigned char DeviceListNum,unsigned char *DataBuffer)
  功能:	读某个设备的数据
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	写标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetRTCFun(unsigned char *DateBuffer)
{
    return SetRTC(DateBuffer);
}

/******************************************************************************
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	写标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	写标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	读标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadRTCFun(unsigned char *DateBuffer)
{
    return ReadRTC(DateBuffer);
}


/******************************************************************************
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	读标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
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
