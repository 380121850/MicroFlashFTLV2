#ifndef  __DownChannel0_Module_H
#define  __DownChannel0_Module_H


/******************************************************************************
  函数(模块)名称:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  功能:对一系列字节进行累加求和	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 字节的累加和         	 		   		 
  其它说明:属于内部函数static 
*******************************************************************************/
unsigned char ReadMeterList(void);
void EnMeterReadTask(void);

/******************************************************************************
  函数(模块)名称:void DownChanne0_Init(void)
  功能:	下行通道通讯波特率设置函数
  输入参数:无         	              		
  输出参数: 无        	 		   		 
  其它说明:默认使用2400波特率，偶校验，8位数据位，1位停止位
*******************************************************************************/
void DownChanne0_Init(void);

unsigned char DownChanne0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent);

/******************************************************************************
  函数(模块)名称:void DownChanne0_CountTaskTime(void)
  功能:	下行通道任务计时函数
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:
******************************************************************************/
void DownChanne0_CountTaskTime(void);

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOn(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
unsigned char ReadMeasuData_TaskOn(void);

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOFF(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
void ReadMeasuData_TaskOFF(void);


/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  功能:	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
extern unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType);

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  功能:	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
unsigned char SeareDevcie_TaskRequ(unsigned char DeviceType,unsigned char *DeviceIDBuff,
                                     unsigned char SeareLent);

unsigned char DownChanne0_ReadStatus(void);

/******************************************************************************
  函数(模块)名称:void DownChanne0_TaskProcess(void)
  功能:	下行通道0任务处理函数
  输入参数:无         	              		
  输出参数:         	 		   		 
  其它说明:下行通道的所有任务均在此任务函数内处理
*******************************************************************************/
void DownChanne0_TaskProcess(void);

unsigned char ReadDownChanne0_TaskStatus(void);
unsigned char ReadDownChanne0_TaskID(void);

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOFF(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
void SeareDevcie_TaskOFF(void);

unsigned char ReadDateMeterInfo(void);


/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOFF(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
void ReadMeasuData_TaskOFF(void);

#endif