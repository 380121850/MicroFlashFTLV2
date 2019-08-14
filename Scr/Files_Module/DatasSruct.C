//************************************************************************
//        根据存储map，对各种数据提供读写API
//            
//************************************************************************
#include ".\Storage_Module\StorageManage.H"
#include "DatasSruct.H"


//****************************************************************************//
//********                                                          **********//
//********               采集器基本信息操作API                      **********//
//********                                                          **********//
//说明：主要是采集器一些基本信息结构体的读写API函数
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  功能:	读整个文件系统的信息头，包含了采集器一些信息
  输入参数: 结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
{
    return Storage_Module_Read( CollectorInfoTable_Addr,sizeof(struct FileBasicTableStruct),(unsigned char *)FileBasicTable ); 
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  功能:	读整个文件系统的信息头，包含了采集器一些信息
  输入参数: 结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteCollectorInfo( struct FileBasicTableStruct *FileBasicTable)
{
    return Storage_Module_Write( CollectorInfoTable_Addr,sizeof(struct FileBasicTableStruct),(unsigned char *)FileBasicTable );
}





//****************************************************************************//
//********                                                          **********//
//********               采集器从机类型操作API                      **********//
//********                                                          **********//
//说明：主要是采集器存储的从机类型信息的读写API函数
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  功能:	读设备类型条目
  输入参数:设备类型，设备类型结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 类型匹配、条目有效状态、才返回
*******************************************************************************/
unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
{
    return Storage_Module_Read( DeviceTypeTable_Addr,sizeof(struct DeviceTypeStruct),(unsigned char *)( DeviceTypeItem ) );
}

/******************************************************************************
  函数(模块)名称:unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  功能:	写设备类型条目
  输入参数:     	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
{
    return Storage_Module_Write( DeviceTypeTable_Addr,sizeof(struct DeviceTypeStruct),(unsigned char *)( DeviceTypeItem ) );
}




//****************************************************************************//
//********                                                          **********//
//********               采集器从机列表操作API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
{
    unsigned long StartAddr;
    if( DeviceListNum < DeviceCntMax )
    {
        StartAddr = DeviceListTable_Addr + sizeof(struct DeviceListStruct)*DeviceListNum;  //取得设备列表地址
        return Storage_Module_Read( StartAddr,sizeof( struct DeviceListStruct ),(unsigned char *)( DeviceListItem ) );
    }
    return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	指定序号写入列表条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
{
    unsigned long StartAddr;
    if( DeviceListNum < DeviceCntMax )
    {
        StartAddr = DeviceListTable_Addr + sizeof(struct DeviceListStruct)*DeviceListNum;  //取得设备列表地址
        return Storage_Module_Write( StartAddr,sizeof( struct DeviceListStruct ),(unsigned char *)( DeviceListItem ) );
    }
    return 0;
}



//****************************************************************************//
//********                                                          **********//
//********               采集器从机数据操作API                      **********//
//********                                                          **********//
//****************************************************************************//
/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
{
    return Storage_Module_Read( DeviceDataAddr,sizeof( struct DeviceCurrentData ),(unsigned char *)( CurrenData ) );
}



/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
{
    return Storage_Module_Write( DeviceDataAddr,sizeof( struct DeviceCurrentData ),(unsigned char *)( CurrenData ) );
}




/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData)
{
    return Storage_Module_Read( DeviceDataAddr,sizeof( struct DeviceHistoryData ),(unsigned char *)( HistoryData ) );
}



/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData)
{
    return Storage_Module_Write( DeviceDataAddr,sizeof( struct DeviceHistoryData ),(unsigned char *)( HistoryData ) );
}