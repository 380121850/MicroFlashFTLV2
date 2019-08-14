#ifndef  __DataMapping_H
#define  __DataMapping_H


//*****************采集器基本头信息*****************************//
#define CollectorInfoTable_Addr    0x00000000   //采集器基本信息数据区域起始地址 
                                                //大小为0XFF
#define CollectorType              0X02         //采集器类型号
#define CurrenFileSYSVer           0X0100       //当前存储版本号 V01.00
#define CurrenManuFacInfo          0X01         // LSD==0X01
struct FileBasicTableStruct
{
    unsigned char DeviceType;       //设备类型
    unsigned char DeviceIDBuff[7];  //采集器ID号
    unsigned int  FileSYSVer;       //存储版本号
    unsigned char ManufacInfo;      //设备厂商信息
    unsigned char ReadMeterData[3]; //抄表日期
    unsigned char CheckSum;         //校验和
};




//***************************设备类型结构体**********************//
#define DeviceTypeTable_Addr           (CollectorInfoTable_Addr + sizeof(struct FileBasicTableStruct) + 256)  //类型列表起始地址
#define EnBit   0x01                   //有效位  0有效，1无效
struct DeviceTypeStruct
{
    unsigned char Num;                 //列表序号
    unsigned char Status;              //设备状态寄存器
    unsigned char DeviceType;          //设备类型
    unsigned long DeviceListPoint;     //设备列表指针 指设备列表在FLASH中的起始地址
    unsigned int  DeviceCnt;           //设备个数 表示有列表包含几个设备
    unsigned int  Reaserv;             //保留
    unsigned char CheckSum;            //表校验和
};




//********************************设备列表结构体*******************************//
#define  DeviceListTable_Addr  ( DeviceTypeTable_Addr + sizeof(struct DeviceTypeStruct) + 256)  //设备列表起始地址
#define  DeviceCntMax    128    //设备列表最大容量
struct DeviceListStruct
{
    unsigned char Num;                 //列表序号
    unsigned char Status;              //设备状态寄存器
    unsigned char DeviceIDBuff[7];     //设备ID号
    unsigned long DeviceDataPoint;     //设备数据存储块指针
    unsigned char CheckSum;            //表校验和    
};
#define  DeviceTypeDataMax   (unsigned long)sizeof(struct DeviceListStruct)*DeviceCntMax //一种设备类型需要的最大存储空间






//*******************************设备数据结构体********************************//
struct DeviceCurrentData
{
  unsigned char DeviceType;
  unsigned char DeviceID[7];
  unsigned char CurrentData[19];
  unsigned char CurrentDate[7];
  unsigned char CheckSum;
};


struct DeviceHistoryData
{
  unsigned char CurrentData[5];
  unsigned char CurrentDate[7];
  unsigned char CheckSum;
};

#define  DeviceDataTable_Addr    ( DeviceListTable_Addr + DeviceTypeDataMax )  //设备数据起始地址
struct DeviceDataStruct
{
    struct DeviceCurrentData CurrenData;       //当前数据 
    struct DeviceHistoryData HistoryData[12];  //历史数据
};



#endif