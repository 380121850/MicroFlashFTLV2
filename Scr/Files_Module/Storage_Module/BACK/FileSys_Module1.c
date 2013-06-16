  
//************************************************************************
//  Lierda, Inc       存储格式
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
  函数(模块)名称:unsigned char ReadBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
  功能:	从指定地址读出一个基本信息头
  输入参数:基本信息头结构体指针        	              		
  输出参数:1成功，0失败 返回基本信息头的结构体          	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ReadBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
{
    if( Storage_Module_Read( BasicHeadAddr,(unsigned char *)FileBasicTable,sizeof(*FileBasicTable),SPIFlashMod ) )
    {
        if( FileBasicTable->CheckSum == SumCheck( (unsigned char *)FileBasicTable,
                                                 (unsigned char *)( &(FileBasicTable->CheckSum) ) - (unsigned char *)FileBasicTable ) )//数据校验
        {
            return 1;
        }
        else return 0;
    }
    else return 0;    
    
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
  功能:	从指定地址读出一个基本信息头
  输入参数:基本信息头结构体指针        	              		
  输出参数:1成功，0失败 返回基本信息头的结构体          	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char WriteBasicHead(unsigned long BasicHeadAddr,struct FileBasicTableStruct *FileBasicTable)
{
    if( FileBasicTable->CheckSum == SumCheck( (unsigned char *)FileBasicTable,
                                             (unsigned char *)( &(FileBasicTable->CheckSum) ) - (unsigned char *)FileBasicTable ) )//数据校验
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
  函数(模块)名称:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  功能:	读整个文件系统的信息头，包含了采集器一些信息
  输入参数: 结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char ReadCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
  功能:	读整个文件系统的信息头，包含了采集器一些信息
  输入参数: 结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteCollectorInfo(struct FileBasicTableStruct *FileBasicTable)
{
    return WriteBasicHead( CollectorInfoTable_Addr,FileBasicTable );
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadCollectorID(unsigned char *CollectorIDBuff)
  功能:	读采集器的ID号
  输入参数:7字节长度的缓冲区首地址        	              		
  输出参数:          	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char ReadCollectorID(unsigned char *CollectorIDBuff)
  功能:	写采集器的ID号
  输入参数:7字节长度的缓冲区首地址        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteCollectorID(unsigned char *CollectorIDBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        StringCopy( CollectorIDBuff,FileBasicTable.DeviceIDBuff,7 );
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //校验和
        return WriteCollectorInfo( &FileBasicTable );
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  功能:	读采集器抄表日期 3个字节
  输入参数:3字节长度的缓冲区首地址        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        StringCopy(FileBasicTable.ReadMeterData,MeterReadDateBuff,3);//取得抄表日期
        return 1;
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  功能:	写采集器抄表日期
  输入参数:1字节长度的缓冲区首地址        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteMeterReadDate(unsigned char *MeterReadDateBuff)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
        StringCopy(MeterReadDateBuff,FileBasicTable.ReadMeterData,3);//抄表日期
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //校验和
        return WriteCollectorInfo( &FileBasicTable );
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  功能:	读采集器抄表日期 1个字节
  输入参数:1字节长度的缓冲区首地址        	              		
  输出参数:          	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char ReadMeterReadDate(unsigned char *MeterReadDateBuff)
  功能:	读采集器抄表日期 1个字节
  输入参数:1字节长度的缓冲区首地址        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteUSCIA0Info(struct CommuniPortInfoStruct *USCIA0Info)
{
    struct FileBasicTableStruct FileBasicTable;
    if( ReadCollectorInfo(&FileBasicTable) )
    {
         FileBasicTable.CommuniPortInfo[0] = *USCIA0Info;
         FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //校验和
         return WriteCollectorInfo( &FileBasicTable );
    }
    else return 0;
}


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
               && ( FileBasicTable.StorageInfo.MaxDeviceType == DeviceTypeCntMax )                     //当前可以支持的最大类型数
               && ( FileBasicTable.StorageInfo.DeviceTypeItemSize == sizeof(struct DeviceTypeStruct) ) //设备类型条目大小 字节
               && ( FileBasicTable.StorageInfo.MaxDevice == DeviceCntMax )                             //每个设备类型对应的最大设备数
               && ( FileBasicTable.StorageInfo.DeviceListItemSize == sizeof(struct DeviceListStruct) ) //设备列表条目大小
               && ( FileBasicTable.StorageInfo.DeviceDataSize == sizeof( struct DeviceDataStruct ) )   //设备数据大小
              )  
            {        //当前厂商信息
                return 1;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
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
        
        FileBasicTable.StorageInfo.MaxDeviceType = DeviceTypeCntMax;                     //当前可以支持的最大类型数
        FileBasicTable.StorageInfo.DeviceTypeItemSize = sizeof(struct DeviceTypeStruct); //设备类型条目大小 字节
        FileBasicTable.StorageInfo.MaxDevice = DeviceCntMax;                             //每个设备类型对应的最大设备数
        FileBasicTable.StorageInfo.DeviceListItemSize = sizeof(struct DeviceListStruct); //设备列表条目大小
        FileBasicTable.StorageInfo.DeviceDataSize = sizeof( struct DeviceDataStruct );   //设备数据大小

        FileBasicTable.CommuniPortInfo[0].Band = 2400;//默认使用2400波特率，偶校验，8位数据位1位停止位
        FileBasicTable.CommuniPortInfo[0].Pariy = 2;
        FileBasicTable.CommuniPortInfo[0].Data = 8;
        FileBasicTable.CommuniPortInfo[0].Stop = 1;
        
        FileBasicTable.CommuniPortInfo[1].Band = 9600;//默认使用9600波特率，偶校验，8位数据位1位停止位
        FileBasicTable.CommuniPortInfo[1].Pariy = 2;
        FileBasicTable.CommuniPortInfo[1].Data = 8;
        FileBasicTable.CommuniPortInfo[1].Stop = 1;
        
        FileBasicTable.CommuniPortInfo[2].Band = 9600;//默认使用9600波特率，偶校验，8位数据位1位停止位
        FileBasicTable.CommuniPortInfo[2].Pariy = 2;
        FileBasicTable.CommuniPortInfo[2].Data = 8;
        FileBasicTable.CommuniPortInfo[2].Stop = 1;
        
        FileBasicTable.CommuniPortInfo[3].Band = 115200;//默认使用115200波特率，无校验，8位数据位1位停止位
        FileBasicTable.CommuniPortInfo[3].Pariy = 0;
        FileBasicTable.CommuniPortInfo[3].Data = 8;
        FileBasicTable.CommuniPortInfo[3].Stop = 1;
        
        FileBasicTable.ReadMeterData[0] = 0X00;                 //抄表日初始化为 每个月的XX号，0时0分
        FileBasicTable.ReadMeterData[1] = 0X00;                 //抄表日默认不设定
        FileBasicTable.ReadMeterData[2] = 0XFF;
        
        FileBasicTable.CheckSum = SumCheck( (unsigned char *)(&FileBasicTable),(unsigned char *)( &(FileBasicTable.CheckSum) ) - (unsigned char *)(&FileBasicTable) );  //校验和
        FlashModule_ChipErase();
        DeviceStructInit(); //存储结构初始化
        return WriteCollectorInfo(&FileBasicTable);
    }
    else return 1;
}



/******************************************************************************/
/*                           设备类型列表                                     */
/******************************************************************************/

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceTypeItem(unsigned char DeviceType,struct DeviceTypeStruct *DeviceTypeItem)
  功能:	读设备类型条目
  输入参数:设备类型，设备类型结构体指针       	              		
  输出参数:          	 		   		 
  其它说明: 类型匹配、条目有效状态、才返回
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
         StartAddr = StartAddr + sizeof(*DeviceTypeItem);//指向下一个条目
    }
    return 0;
}



/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceTypeExt(unsigned char Num,unsigned char *DeviceType)
  功能:	读设备类型条目中的设备类型
  输入参数:       	              		
  输出参数:          	 		   		 
  其它说明: 类型匹配、条目有效状态、才返回
*******************************************************************************/
unsigned char ReadDeviceTypeExt(unsigned char Num,unsigned char *DeviceType)
{
    unsigned long StartAddr = DeviceTypeTable_Addr;
    struct DeviceTypeStruct DeviceTypeItem;
    
    StartAddr = StartAddr + (unsigned long)sizeof(&DeviceTypeItem)*Num;    //指向下一个条目
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
  函数(模块)名称:unsigned char Add_DeviceType(unsigned char DeviceType)
  功能:	增加一种设备类型
  输入参数:需要增加的设备类型        	              		
  输出参数:          	 		   		 
  其它说明:若已经列表中已经存在这种类型，则返回失败。 
*******************************************************************************/
unsigned char Add_DeviceTypeItem(unsigned char DeviceType)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )
    {
        return 0;   //增加失败，设备类型已经存在并且有效
    }
    else
    {//设备类型不存在或是无效
        for( unsigned int i=0;i<DeviceTypeCntMax;i++ )
        {
             if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod ) )
             {
                 if( (DeviceTypeItem.Status & EnBit) == 0)//当前条目还没有被启用
                 {
                     DeviceTypeItem.Status |= EnBit;      //置标志位，启用
                     DeviceTypeItem.DeviceType = DeviceType;
                     DeviceTypeItem.CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );  //校验和
                     return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod );
                 }
             }
             StartAddr = StartAddr + sizeof(DeviceTypeItem);//指向下一个条目
        }
        return 0;   //设备类型已经满     
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char Dell_DeviceType(unsigned char DeviceType)
  功能:	删除一种设备类型
  输入参数:需要删除的设备类型        	              		
  输出参数:          	 		   		 
  其它说明:若不存在或已经删除状态，则返回失败 
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
                 if( (DeviceTypeItem.Status & EnBit) == 1)//条目启用状态
                 {
                      DeviceTypeItem.Status &= ~EnBit;      //清标志位，置无效
                      DeviceTypeItem.CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );  //校验和
                      return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod );
                 }
                 else return 0;//删除动作失败，条目已经无效状态
             }
         }
         StartAddr = StartAddr + sizeof(DeviceTypeItem);//指向下一个条目
    }
    return 0;  //找不到这个设备类型
}


/******************************************************************************/
/*                             设备列表                                       */
/******************************************************************************/

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff, 
                                                  struct DeviceListStruct *DeviceListItem)
  功能:	读设备列表条目
  输入参数:设备类型，设备ID号及结构体指针        	              		
  输出参数:          	 		   		 
  其它说明:根据设备类型、设备ID号及条目有效位 
*******************************************************************************/
unsigned char ReadDeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff, 
                                 struct DeviceListStruct *DeviceListItem)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //查找设备类型 根据指定的设备类型，在设备类型表中查找
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;      //取得设备列表地址
        for( unsigned int i=0;i<DeviceCntMax;i++ )       //查找设备列表内的设备
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( DeviceListItem ),sizeof(*DeviceListItem),SPIFlashMod ) )
            {
                if( IsEqual( DeviceListItem->DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem->Status & EnBit) == 1) )
                {            //判断设备ID号及有效位
                    return 1;
                }
            }
            StartAddr = StartAddr + sizeof(*DeviceListItem);    //取得下一个设备条目
        }
        return 0;  //查找设备失败
    }
    else return 0; //查找设备类型失败
}

/******************************************************************************
  函数(模块)名称:unsigned char CountDeviceNum(unsigned char DeviceType)
  功能:	根据序号读设备列表条目
  输入参数:       	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char CountDeviceNum(unsigned char DeviceType,unsigned int *DeviceNum)
{
    struct DeviceTypeStruct DeviceTypeItem;
    struct DeviceListStruct DeviceListItem;
    unsigned long StartAddr;
    unsigned int Cnt=0;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //查找设备类型 根据指定的设备类型，在设备类型表中查找
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;      //取得设备列表地址
        for( unsigned int i=0;i<DeviceCntMax;i++ )       //查找设备列表内的设备
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
            {
                if( ( DeviceListItem.Status & EnBit) == 1 )
                {            //判断设备有效位
                    Cnt++;
                }
            }
            StartAddr = StartAddr + sizeof(DeviceListItem);    //取得下一个设备条目
        }
        *DeviceNum = Cnt;
        return 1;
    }
    else return 0; //查找设备类型失败
}

/******************************************************************************
  函数(模块)名称:unsigned char CountDeviceNum(unsigned char DeviceType)
  功能:	根据序号读设备列表条目
  输入参数:       	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char WriteDeviceNum(unsigned char DeviceType,unsigned int DeviceNum)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //查找设备类型 根据指定的设备类型，在设备类型表中查找
    {
        DeviceTypeItem.DeviceCnt = DeviceNum;  //设备数量
        DeviceTypeItem.CheckSum  = SumCheck( (unsigned char *)(&DeviceTypeItem),(unsigned char *)( &(DeviceTypeItem.CheckSum) ) - (unsigned char *)(&DeviceTypeItem) );
        StartAddr = StartAddr + (unsigned long)sizeof(&DeviceTypeItem)*DeviceTypeItem.Num;    //指向下一个条目
        return  Storage_Module_Write( StartAddr,(unsigned char *)(&DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod );
    }
    else return 0; //查找设备类型失败
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceListItem_Ext(unsigned char DeviceType,unsigned char Num, 
                                 struct DeviceListStruct *DeviceListItem)
  功能:	根据序号读设备列表条目
  输入参数:       	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char ReadDeviceListItem_Ext(unsigned char DeviceType,unsigned char Num, 
                                 struct DeviceListStruct *DeviceListItem)
{
    struct DeviceTypeStruct DeviceTypeItem;
    unsigned long StartAddr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) ) //查找设备类型 根据指定的设备类型，在设备类型表中查找
    {
        StartAddr = DeviceTypeItem.DeviceListPoint +  sizeof(*DeviceListItem)*Num;      //取得设备列表地址
        if( Storage_Module_Read( StartAddr,(unsigned char *)( DeviceListItem ),sizeof(*DeviceListItem),SPIFlashMod ) )
        {
            if( ( DeviceListItem->Status & EnBit) == 1 )
            {            //判断有效位
                return 1;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0; //查找设备类型失败
}


/******************************************************************************
  函数(模块)名称:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	增加一种设备类型的设备
  输入参数:需要增加的设备类型，设备ID号        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Add_DeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff)
{
    struct DeviceTypeStruct DeviceTypeItem;//设备类型条目
    struct DeviceListStruct DeviceListItem;//设备列表条目
    
    unsigned long StartAddr = DeviceTypeTable_Addr;
    
    if( ReadDeviceListItem(DeviceType,DeviceIDBuff,&DeviceListItem) )
    {
        return 0;//已经存在增加的设备
    }
    else
    {
        if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//取设备类型表
        {
            StartAddr = DeviceTypeItem.DeviceListPoint;  //取得设备列表地址
            for(unsigned int j=0;j<DeviceCntMax;j++)     //在设备列表中查找空条目
            {
                if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
                {
                   if( (DeviceListItem.Status & EnBit) == 0 )//当前条目是空/无效
                   {
                       DeviceListItem.Status |= EnBit;                           //置标志位，启用
                       StringCopy(DeviceIDBuff,DeviceListItem.DeviceIDBuff,7);   //设备ID号复制过来
                       DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //校验和
                       return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
                   }
               }
               StartAddr = StartAddr + sizeof(DeviceListItem);//指向下一个条目
           }
           return 0;         
        }
        else return 0; //不存在设备类型，返回0
    }
}


/******************************************************************************
  函数(模块)名称:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	增加一种设备类型的设备
  输入参数:需要增加的设备类型，设备ID号        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Dell_DeviceListItem(unsigned char DeviceType,unsigned char *DeviceIDBuff)
{
    struct DeviceTypeStruct DeviceTypeItem;//设备类型条目
    struct DeviceListStruct DeviceListItem;//设备列表条目
    
    unsigned long StartAddr;

    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//取设备类型表
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;  //取得设备列表地址
        for(unsigned int j=0;j<DeviceCntMax;j++)     //在设备列表中查找空条目
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
            {
                if( IsEqual( DeviceListItem.DeviceIDBuff,DeviceIDBuff,7 ) && (( DeviceListItem.Status & EnBit) == 1) )
                { //判断设备ID号及有效位
                    DeviceListItem.Status &= ~EnBit;       //清标志位，置无效
                    DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //校验和
                    return Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
                }
           }
           StartAddr = StartAddr + sizeof(DeviceListItem);//指向下一个条目
       }
       return 0;   //没有此设备，删除操作失败        
    }
    else return 0; //不存在设备类型，返回0
}


/******************************************************************************
  函数(模块)名称:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	增加一种设备类型的设备
  输入参数:需要增加的设备类型，设备ID号        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
/*unsigned char Dell_AllDeviceList(unsigned char DeviceType)
{
    struct DeviceTypeStruct DeviceTypeItem;//设备类型条目
    struct DeviceListStruct DeviceListItem;//设备列表条目
    unsigned long StartAddr;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//取设备类型表
    {
        StartAddr = DeviceTypeItem.DeviceListPoint;  //取得设备列表地址
        for(unsigned int j=0;j<DeviceCntMax;j++)     //在设备列表中查找空条目
        {
            if( Storage_Module_Read( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod ) )
            {
                if( ( DeviceListItem.Status & EnBit) == 1 )
                { //判断设备ID号及有效位
                    Storage_Module_Write( DeviceListItem.DeviceDataPoint,(unsigned char *)(FFBuffer ),sizeof(struct DeviceDataStruct),SPIFlashMod );
                    DeviceListItem.Status &= ~EnBit;       //清标志位，置无效
                    DeviceListItem.CheckSum = SumCheck( (unsigned char *)(&DeviceListItem),(unsigned char *)( &(DeviceListItem.CheckSum) ) - (unsigned char *)(&DeviceListItem) );  //校验和
                    Storage_Module_Write( StartAddr,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
                }
                
           }
           StartAddr = StartAddr + sizeof(DeviceListItem);//指向下一个条目
        }
       return 1;        
    }
    else return 0; //不存在设备类型，返回0
}
*/

/******************************************************************************
  函数(模块)名称:unsigned char Add_Device(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	增加一种设备类型的设备
  输入参数:需要增加的设备类型，设备ID号        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Dell_AllDeviceList(unsigned char DeviceType)
{
    struct DeviceTypeStruct DeviceTypeItem;//设备类型条目
    struct DeviceListStruct *DeviceListItem;//设备列表条目
    unsigned char *DeviceListItemBuffer;
    unsigned char Flag=0;
    
    if( ReadDeviceTypeItem(DeviceType,&DeviceTypeItem) )//取设备类型表
    {
        DeviceListItemBuffer = (unsigned char *)malloc( sizeof(struct DeviceListStruct)*DeviceCntMax ); //申请帧数据空间
        if( NULL != DeviceListItemBuffer )//没有申请到，则返回
        {
            if( Storage_Module_Read( DeviceTypeItem.DeviceListPoint,DeviceListItemBuffer,sizeof(struct DeviceListStruct)*DeviceCntMax,SPIFlashMod ) )
            {
                DeviceListItem = (struct DeviceListStruct *)DeviceListItemBuffer;
                for(unsigned int j=0;j<DeviceCntMax;j++)     //在设备列表中查找非空条目
                {
                    if( ( DeviceListItem->Status & EnBit) == 1 )
                    { //判断有效位
                        Storage_Module_Write( DeviceListItem->DeviceDataPoint,(unsigned char *)(FFBuffer ),sizeof(struct DeviceDataStruct),SPIFlashMod );
                        DeviceListItem->Status &= ~EnBit;       //清标志位，置无效
                        DeviceListItem->CheckSum = SumCheck( (unsigned char *)(DeviceListItem),(unsigned char *)( &(DeviceListItem->CheckSum) ) - (unsigned char *)(DeviceListItem) );  //校验和
                        Flag = 1;
                    }
                    DeviceListItem++;  //指向下一个条目 
               }
               if(Flag)
                 Flag = Storage_Module_Write( DeviceTypeItem.DeviceListPoint,DeviceListItemBuffer,sizeof(struct DeviceListStruct)*DeviceCntMax,SPIFlashMod );
               free(DeviceListItemBuffer);            //释放申请到的空间
               return Flag;
            }
            else return 0;
        }
        else return 0;
    }
    else return 0;
}
/******************************************************************************
  函数(模块)名称:unsigned char ReadMeterAllData(unsigned char MeterType,unsigned char *MeterIDBuff,
                               unsigned char *ReadBuffer)
  功能:	读从表所有数据
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
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
  函数(模块)名称:unsigned char CheckDeviceTypeTable(void)
  功能:	检查设备类型列表是否正常
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char CheckDeviceTypeTable(void)
{
    struct DeviceTypeStruct  DeviceTypeItem;  //设备类型列表
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
        StartAddr = StartAddr + sizeof(DeviceTypeItem);//指向下一个条目
    }
    return 1;
}


/******************************************************************************
  函数(模块)名称:unsigned char DeviceTypeStructInit(void)
  功能:	设备类型列表初始化
  输入参数:        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char DeviceStructInit(void)
{
    struct DeviceTypeStruct  DeviceTypeItem[DeviceTypeCntMax];  //设备类型列表
    struct DeviceListStruct  DeviceListItem[DeviceCntMax];      //设备列表
    
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
               DeviceListItem[j].CheckSum = SumCheck( (unsigned char *)(&DeviceListItem[j]),(unsigned char *)( &(DeviceListItem[j].CheckSum) ) - (unsigned char *)(&DeviceListItem[j]) );  //校验和;;
           }
           DeviceTypeItem[i].DeviceCnt = 0x00;
           DeviceTypeItem[i].Reaserv = 0xFF;
           DeviceTypeItem[i].CheckSum = SumCheck( (unsigned char *)(&DeviceTypeItem[i]),(unsigned char *)( &(DeviceTypeItem[i].CheckSum) ) - (unsigned char *)(&DeviceTypeItem[i]) );  //校验和;
           
           Storage_Module_Write( DeviceTypeItem[i].DeviceListPoint,(unsigned char *)( &DeviceListItem ),sizeof(DeviceListItem),SPIFlashMod );
        }
        return Storage_Module_Write( DeviceTypeTable_Addr,(unsigned char *)( &DeviceTypeItem ),sizeof(DeviceTypeItem),SPIFlashMod ); 
    }
    else return 1;
}