#ifndef  __DatasSruct_H
#define  __DatasSruct_H

#include "DataMapping.H"


//****************************************************************************//
//********                                                          **********//
//********               采集器基本信息操作API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  功能:	读整个文件系统的信息头，包含了采集器一些信息
  输入参数: 结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable);


/******************************************************************************
  函数(模块)名称:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  功能:	读整个文件系统的信息头，包含了采集器一些信息
  输入参数: 结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteCollectorInfo(struct FileBasicTableStruct *FileBasicTable);






//****************************************************************************//
//********                                                          **********//
//********               采集器从机类型操作API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  功能:	读设备类型条目
  输入参数:设备类型，设备类型结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 类型匹配、条目有效状态、才返回
*******************************************************************************/
unsigned char ReadDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem );


/******************************************************************************
  函数(模块)名称:unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem )
  功能:	写设备类型条目
  输入参数:     	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char WriteDeviceInfo( struct DeviceTypeStruct *DeviceTypeItem );







//****************************************************************************//
//********                                                          **********//
//********               采集器从机列表操作API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	读设备列表条目
  输入参数:设备类型，设备ID号及结构体指针        	              		
  输出参数:          	 		   		 
  其它说明:根据设备类型、设备ID号及条目有效位 
*******************************************************************************/
unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem);


/******************************************************************************
  函数(模块)名称:unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	读设备列表条目
  输入参数:设备类型，设备ID号及结构体指针        	              		
  输出参数:          	 		   		 
  其它说明:根据设备类型、设备ID号及条目有效位 
*******************************************************************************/
unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem);








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
                                 struct DeviceListStruct *DeviceListItem);

/******************************************************************************
  函数(模块)名称:unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	指定序号写入列表条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem);



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
                                 struct DeviceCurrentData *CurrenData);

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData);


/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData);


/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurreData( unsigned long DeviceDataAddr,
                                 struct DeviceCurrentData *CurrenData)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteHistoryData( unsigned long DeviceDataAddr,
                                 struct DeviceHistoryData *HistoryData);



#endif