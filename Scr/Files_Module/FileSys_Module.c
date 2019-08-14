//************************************************************************
//        为上层提供各种数据结构的读写API
//            
//************************************************************************
#include "stdio.h"
#include "DatasSruct.H"
#include "FileSys_Module.h"



/******************************************************************************
  函数(模块)名称:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  功能:对一系列字节进行累加求和	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 字节的累加和         	 		   		 
  其它说明:属于内部函数static 
*******************************************************************************/
static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    unsigned char SumCheck = 0X00;
    for(unsigned int i=0;i<SumCheckLent;i++) //累加求和
      SumCheck = SumCheck + *SumCheckBuff++;
    return SumCheck;                         //返回累加值
}


/******************************************************************************
  函数(模块)名称:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  功能:判断两串字节串是否相等	
  输入参数:字符串指针1，字符串指针2，长度         	              		
  输出参数:          	 		   		 
  其它说明: 
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
  函数(模块)名称:void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
  功能:字符串复制	
  输入参数:字符串指针1，字符串指针2，长度         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
static void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
{
    while(Lent--)
      *DstAddr++ = *SourceBuff++;
}

/******************************************************************************
  函数(模块)名称:static void SetBuffZero(unsigned char *DataBuff,unsigned int Lent)
  功能:把缓冲区数据清为0	
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
static void SetBuffZero(unsigned char *DataBuff,unsigned int Lent)
{
    while(Lent--)
      *DataBuff++ = 0;
}





//****************************************************************************//
//********                                                          **********//
//********               采集器基本信息操作API                      **********//
//********                                                          **********//
//说明：主要是采集器一些基本信息结构体的读写API函数
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char CheckFileSysInfo(void)
  功能:	整个文件系统信息头的初始化
  输入参数: 无       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char CheckFileSysInfo(void)
{
    struct FileBasicTableStruct FileBasicTable;
    
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        if( FileBasicTable.CheckSum == SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) ) )  //校验和
        {
            if( 
                  ( CollectorType == FileBasicTable.DeviceType ) 
               && ( CurrenFileSYSVer == FileBasicTable.FileSYSVer )
               && ( CurrenManuFacInfo == FileBasicTable.ManufacInfo )
              )  
            {
                return 1;
            }
            else return 0;//采集器信息出错
        }
        else return 0;//校验和出错
    }
    else return 0;//读信息出错
}



/******************************************************************************
  函数(模块)名称:unsigned char FileSysInfoInit(void)
  功能:	整个文件系统信息头的初始化
  输入参数: 无       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char FileSysInfoInit(void)
{
    struct FileBasicTableStruct FileBasicTable; 
    if( 0 == CheckFileSysInfo() )
    {
        FileBasicTable.DeviceType = CollectorType;               //数据类型 0X02 表示设备类型是采集器
        SetBuffZero(FileBasicTable.DeviceIDBuff,7);             //设备ID/表号设为0
        FileBasicTable.FileSYSVer = 0X0100;                     //文件系统版本号 默认为 0X0100
        FileBasicTable.ManufacInfo = 0X01;                      //厂商ID,默认为0X01
        
        FileBasicTable.ReadMeterData[0] = 0X00;                 //抄表日初始化为 每个月的XX号，0时0分
        FileBasicTable.ReadMeterData[1] = 0X00;                 //抄表日默认不设定
        FileBasicTable.ReadMeterData[2] = 0XFF;
        
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //校验和
        return WriteCollectorInfo(&FileBasicTable);
    }
    else return 1;
}



//****************************************************************************//
//********                                                          **********//
//********               采集器从机类型操作API                      **********//
//********                                                          **********//
//说明：主要是采集器存储的从机类型信息的读写API函数
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char SetDeviceType( unsigned char DeviceType )
  功能:	读设备类型
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadDeviceType( unsigned char *DeviceType )
{
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceInfo( &DeviceTypeItem ) )
    {
        if( (DeviceTypeItem.Status & EnBit)&&   //启用标志位
            ( DeviceTypeItem.CheckSum == SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) ) )//校验和) 
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
  函数(模块)名称:unsigned char SetDeviceType( unsigned char DeviceType )
  功能:	设置设备类型
  输入参数:需要设置的设备类型        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetDeviceType( unsigned char DeviceType )
{
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceInfo( &DeviceTypeItem ) )
    {
        DeviceTypeItem.Status |= EnBit;      //置标志位，启用
        DeviceTypeItem.DeviceListPoint = DeviceListTable_Addr;
        DeviceTypeItem.DeviceType = DeviceType;
        DeviceTypeItem.CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );  //校验和
        return WriteDeviceInfo( &DeviceTypeItem );
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char CheckDeviceType( void )
  功能:	
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char CheckDeviceType( void )
{
    struct DeviceTypeStruct DeviceTypeItem;
    if( ReadDeviceInfo( &DeviceTypeItem ) )
    {
        if( (DeviceTypeItem.Status & EnBit) //启用标志位
           && ( DeviceTypeItem.CheckSum == SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) ) )//校验和) 
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
//********               采集器从机列表操作API                      **********//
//********                                                          **********//
//****************************************************************************//

/******************************************************************************
  函数(模块)名称:unsigned char SearchDevice(unsigned char *DeviceIDBuff,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	查找设备列表条目
  输入参数:设备类型ID，结构体指针        	              		
  输出参数:          	 		   		 
  其它说明:不涉及具体地址计算 
*******************************************************************************/
unsigned char SearchDevice(unsigned char *DeviceIDBuff,
                                 struct DeviceListStruct *DeviceListItem)
{
    for( unsigned int i=0;i<DeviceCntMax;i++ )       //查找设备列表内的设备
    {
        if( ReadDeviceListItem(i,DeviceListItem) )
        {
            if( IsEqual( DeviceListItem->DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem->Status & EnBit) == 0) )//判断设备ID号及有效位
            {   
                return 1;
            }
        }
    }
    return 0;                                       //查找设备失败
}


/******************************************************************************
  函数(模块)名称:unsigned char Add_Device(unsigned char *DeviceIDBuff)
  功能:	增加一种设备类型的设备
  输入参数:需要增加的设备类型，设备ID号        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Add_Device( unsigned char *DeviceIDBuff )
{
    struct DeviceListStruct DeviceListItem;//设备列表条目
    if( SearchDevice( DeviceIDBuff,&DeviceListItem) )
    {
        return 0;//已经存在增加的设备
    }
    else
    {
        for( unsigned int i=0;i<DeviceCntMax;i++ )       //查找设备列表内的设备
        {
            if( ReadDeviceListItem(i,&DeviceListItem)&&( ( DeviceListItem.Status & EnBit) == 1 ) )
            {
                DeviceListItem.Status &= ~EnBit;                           //置标志位，启用
                DeviceListItem.Num  = i;
                DeviceListItem.DeviceDataPoint = DeviceDataTable_Addr + sizeof(struct DeviceDataStruct)*i;
                StringCopy(DeviceIDBuff,DeviceListItem.DeviceIDBuff,7);   //设备ID号复制过来
                DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //校验和
                return WriteDeviceListItem( i,&DeviceListItem);
            }
        } 
        return 0;
    }
}


/******************************************************************************
  函数(模块)名称:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	增加一种设备类型的设备
  输入参数:需要增加的设备类型，设备ID号        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Del_Device( unsigned char *DeviceIDBuff )
{
    struct DeviceListStruct DeviceListItem;//设备列表条目
    if( SearchDevice( DeviceIDBuff,&DeviceListItem) )
    {
        if( IsEqual( DeviceListItem.DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem.Status & EnBit) == 0) )
        {   //判断设备ID号及有效位
            DeviceListItem.Status |= EnBit;       //清标志位，置无效
            DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //校验和
            return WriteDeviceListItem( DeviceListItem.Num,&DeviceListItem);
        }
        else return 0;
    }
    else return 0;//不存在
}


/******************************************************************************
  函数(模块)名称:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	增加一种设备类型的设备
  输入参数:需要增加的设备类型，设备ID号        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Del_allDevice(void)
{
    struct DeviceListStruct DeviceListItem;//设备列表条目
    for( unsigned int i=0;i<DeviceCntMax;i++ )       //查找设备列表内的设备
    {
        if( ReadDeviceListItem(i,&DeviceListItem)&&( ( DeviceListItem.Status & EnBit) == 0 ) )
        {
            DeviceListItem.Status |= EnBit;       //清标志位，置无效
            DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //校验和
            if( WriteDeviceListItem( i,&DeviceListItem) == 0)
              return 0;
        }
    }
    return 1;       
}


/******************************************************************************
  函数(模块)名称:unsigned char SearchDevice(unsigned char *DeviceIDBuff,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	查找设备列表条目
  输入参数:设备类型ID，结构体指针        	              		
  输出参数:          	 		   		 
  其它说明:不涉及具体地址计算 
*******************************************************************************/
void CheckDeviceList(void)
{ 
    struct DeviceListStruct DeviceListItem;
    
    printf("\n CheckDeviceList: ");
    for( unsigned int i=0;i<DeviceCntMax;i++ )       //查找设备列表内的设备
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
//********               采集器从机数据操作API                      **********//
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
unsigned char ReadDeviceCurrentData(unsigned char *DeviceIDBuffer,
                                 struct DeviceCurrentData *CurrentData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //校验和
        {
            return ReadCurreData( DeviceListItem.DeviceDataPoint,CurrentData);
        }
        else return 0;
    }
    else return 0;
}



/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceListItem(unsigned char DeviceListNum,
                                 struct DeviceListStruct *DeviceListItem)
  功能:	指定序号读出列表中条目
  输入参数:存储序号，列表结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteDeviceCurrentData(unsigned char *DeviceIDBuffer,
                                 struct DeviceCurrentData *CurrentData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //校验和
        {
            return WriteCurreData( DeviceListItem.DeviceDataPoint,CurrentData);
        }
        else return 0;
    }
    else return 0;
}




/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
  功能:	读出指定ID号的从表的指定月份的历史数据
  输入参数:从表的ID号，月份，及数据存储结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 月份：0~11，分别表示1~12月份
*******************************************************************************/
unsigned char ReadDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //校验和
        {
            return ReadHistoryData( ( DeviceListItem.DeviceDataPoint 
                                     + sizeof( struct DeviceCurrentData ) //当前数据偏移
                                       + sizeof( struct DeviceHistoryData )*Month ),HistoryData);
        }
        else return 0;
    }
    else return 0;
}



/******************************************************************************
  函数(模块)名称:unsigned char  WriteDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
  功能:	写入指定ID号的从表的指定月份的历史数据
  输入参数:从表的ID号，月份，及数据存储结构体指针        	              		
  输出参数:          	 		   		 
  其它说明: 月份：0~11，分别表示1~12月份
*******************************************************************************/
unsigned char WriteDeviceHistoryData(unsigned char *DeviceIDBuffer,unsigned char Month,
                                 struct DeviceHistoryData *HistoryData)
{
    struct DeviceListStruct DeviceListItem;
    
    if( SearchDevice( DeviceIDBuffer,&DeviceListItem) )
    {
        if( DeviceListItem.CheckSum == SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) ) )  //校验和
        {
            return  WriteHistoryData( ( DeviceListItem.DeviceDataPoint 
                                     + sizeof( struct DeviceCurrentData ) //当前数据偏移
                                       + sizeof( struct DeviceHistoryData )*Month ),HistoryData);
        }
        else return 0;
    }
    else return 0;
}






/******************************************************************************
  函数(模块)名称:unsigned char FileSysInit(void)
  功能:	
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
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