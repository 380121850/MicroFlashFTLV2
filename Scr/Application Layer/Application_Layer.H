#ifndef  __Application_Layer_H
#define  __Application_Layer_H

/******************************************************************************
  函数(模块)名称:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  功能:判断两串字节串是否相等	
  输入参数:字符串指针1，字符串指针2，长度         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned int Lent);

/******************************************************************************
  函数(模块)名称:void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
  功能:字符串复制	
  输入参数:字符串指针1，字符串指针2，长度         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent);

/******************************************************************************
  函数(模块)名称:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  功能:对一系列字节进行累加求和	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 字节的累加和         	 		   		 
  其它说明:属于内部函数static 
*******************************************************************************/
unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent);

/******************************************************************************
  函数(模块)名称:unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff)
  功能:	比较采集器ID与命令下来的ID是否一致
  输入参数: 
  输出参数: 成功或失败         	 		   		 
 其它说明: 
*******************************************************************************/
unsigned char IsDeviceIDEqu(unsigned char *CollectorIDBuff);

/******************************************************************************
  函数(模块)名称:unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff)
  功能:	读采集器基本信息
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 至少48字节的缓冲区首地址
*******************************************************************************/
unsigned char ReadCollectorInfoFun(unsigned char *CollectorInfoBuff);

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceID(unsigned char *IDBuffer)
  功能:	提供采集器ID号
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadDeviceID(unsigned char *IDBuffer);


/******************************************************************************
  函数(模块)名称:unsigned char WriteDeviceID(unsigned char *IDBuffer)
  功能:	修改采集器地址
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明:
*******************************************************************************/
unsigned char WriteDeviceID(unsigned char *IDBuffer);



/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	读抄表日命令功能实现函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 对外提供抄表日期数据，一个字节
*******************************************************************************/
unsigned char MeterReadDataRead( unsigned char *DataBuffer );

/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataWrite( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	读抄表日处理
  输入参数: 
  输出参数: 成功或失败         	 		   		 
其它说明: 
*******************************************************************************/
unsigned char MeterReadDataWrite(unsigned char *MeterReadDateBuff );


/******************************************************************************
  函数(模块)名称:unsigned char CheckReadMeterData(void)
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 内部存储是 分 时 日的格式，协议只提供 日 数据 
*******************************************************************************/
unsigned char CheckReadMeterData(void);

/******************************************************************************
  函数(模块)名称:unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff )
  功能:	扩展的表号读
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 输出数据为 设备类型 序号 表号
*******************************************************************************/
unsigned char ExtReadDeviceItem( unsigned char ListNum,unsigned char *DataBuff );

/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceItemNum(unsigned char DeviceType,unsigned char *DeviceIDBuff,unsigned char *Num)
  功能:	根据设备类型、表号读取这个表在采集器存储的列表中的序号
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char ReadDeviceItemNum(unsigned char DeviceType,unsigned char *DeviceIDBuff,unsigned char *Num);

/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char ExtWriteDeviceItem( unsigned char DeviceType,unsigned char *DeviceIDBuff );


/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char ExtWriteDeviceTypeItem( unsigned char DeviceType );


/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char ExtDelDeviceTypeItem( unsigned char DeviceType );


/******************************************************************************
  函数(模块)名称:unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer )
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 返回数据 序号NUM,设备类型，设备个数 4个字节
            默认取第1个设备类型
*******************************************************************************/
unsigned char ExtReadDeviceTypeInfoFun( unsigned char *DataBuffer );

/******************************************************************************
  函数(模块)名称:unsigned char CTL_MBUSON(void)
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char CTL_MBUSON(void);

/******************************************************************************
  函数(模块)名称:unsigned char CTL_MBUSON(void)
  功能:	
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 根据表类型及表号，增加到列表中去
*******************************************************************************/
unsigned char CTL_MBUSOFF(void);


/******************************************************************************
  函数(模块)名称:unsigned char ClearDeviceList(unsigned char DeviceTypeNum)
  功能:	设备列表清除
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ClearDeviceList(unsigned char DeviceTypeNum);


/******************************************************************************
  函数(模块)名称:unsigned char ReadDeviceCurrenData(unsigned char DeviceListNum,unsigned char *DataBuffer)
  功能:	读某个设备的数据
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadDeviceCurrenData(unsigned char DeviceListNum,unsigned char *DataBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	写标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SetRTCFun(unsigned char *DateBuffer);

/******************************************************************************
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	读标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadRTCFun(unsigned char *DateBuffer);


/******************************************************************************
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	写标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadUSCIA0(unsigned char *DataBuffer);


/******************************************************************************
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	写标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteUSCIA0(unsigned char *DataBuffer);


/******************************************************************************
  函数(模块)名称:unsigned char SetRTCFun(unsigned char *DateBuffer)
  功能:	读标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadMeterAllDataFun(unsigned char DeviceListNum, unsigned char *DateBuffer);
/******************************************************************************
  函数(模块)名称:unsigned char MeterReadDataRead( unsigned char *CollectorIDBuff,unsigned char SER )
  功能:	读抄表日命令功能实现函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 对外提供抄表日期数据，一个字节
*******************************************************************************/
unsigned char MeterReadDataRead( unsigned char *DataBuffer );


#endif