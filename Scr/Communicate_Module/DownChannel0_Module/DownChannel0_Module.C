  
//************************************************************************
//  Lierda, Inc      
//************************************************************************
//                 MSP430F5438
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P3.4/UCA0TXD|------------>
//            |                 | 2400 - 8 EVEN 1
//            |     P3.5/UCA0RXD|<------------


#include "msp430x54x.h"

#include "..\..\Sys_Module\Sys_Module.H"             //单片机基本模块
//#include "..\..\Storage_Module\Storage_Module.H"
//#include "..\..\Storage_Module\FileSys_Module.H"
#include "..\..\RTC_Module\RTC_Module.H"            //RTC模块
#include ".\USCIA0_UART_Module\USCIA0_UART_Module.H"
#include "..\..\Application Layer\Application_Layer.H" //提供协议解析后需要的各个功能实现代码

#include "Down0CJ188_Module.h"
#include "DownChannel0_Module.H"

//************************************************************************ 
#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
//************************************************************************ 
//[2] 你要修改的参数。 
#define CPU_F ((double)8000000) 


#define  ACKTimeNum     266    //帧响应时间
#define  FrameReTryNum  3      //帧最大错误次数

//下行通道任务信息结构体
struct DownChanne0_TaskStruct 
{
    unsigned char Status;     //任务状态寄存器 BIT0总开关
    unsigned char TaskID;     //任务号，每个任务均有一个ID号
};
struct DownChanne0_TaskStruct DownChanne0_TaskInfo;


//读计量数据任务结构体
struct ReadMeasureDataStruct
{
    unsigned char Status;                        //状态寄存器
    unsigned char CurrenStatus;                  //当前状态 00,01,02
    unsigned char MeterType;                     //要抄表的类型
    unsigned char MeterListNum;                  //当前列表序号
    struct DeviceListStruct DeviceListItem;      //序号对应的设备列表条目
};
#define MeterReadTask_Bit    BIT0   //抄表任务开关
struct ReadMeasureDataStruct ReadMeasureDataInfo;      //抄表列表


//从表搜索任务结构体
struct SearDeviceStruct
{
    unsigned char Status;                        //状态寄存器
    unsigned char CurrenStatus;                  //当前状态
    unsigned char DeviceType;                    //搜索表类型
    unsigned char CurrenDeviceStarID[7];         //当前列表序号
    unsigned char SeareLent;                     //搜索长度
    unsigned char SeareCnt;                      //搜索计数
};
struct SearDeviceStruct SeareDeviceInfo;//搜索列表信息


//抄表日抄表任务结构体
struct DateMeterReadStruct
{
    unsigned char Status;                        //状态寄存器 BIT0,抄表日抄表任务标志位
};
struct DateMeterReadStruct DateMeterReadInfo;    //搜索列表信息





/******************************************************************************
  函数(模块)名称:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  功能:判断两串字节串是否相等	
  输入参数:字符串指针1，字符串指针2，长度         	              		
  输出参数:          	 		   		 
  其它说明: static 
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
  函数(模块)名称:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  功能:对一系列字节进行累加求和	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 字节的累加和         	 		   		 
  其它说明:属于内部函数static 
*******************************************************************************/
static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    return Down0CJ188_SumCheck(SumCheckBuff,SumCheckLent);
}


/******************************************************************************
  函数(模块)名称:unsigned char DownChanne0_ReadReciStatus(void)
  功能:	读接收缓冲区状态寄存器
  输入参数:无         	              		
  输出参数: 缓冲区状态寄存器         	 		   		 
  其它说明:
            Status寄存器各位宏定义
            BufferOV_BIT       BIT1  //缓冲区满标志位
*******************************************************************************/
unsigned char DownChanne0_ReadReciStatus(void)
{
    return USCIA0_ReadRXBuffStatus() ;
}

unsigned char DownChanne0_ReadStatus(void)
{
    return DownChanne0_TaskInfo.Status ;
}

/******************************************************************************
  函数(模块)名称:void DownChanne0_Init(void)
  功能:	下行通道通讯波特率设置函数
  输入参数:无         	              		
  输出参数: 无        	 		   		 
  其它说明:默认使用2400波特率，偶校验，8位数据位，1位停止位
*******************************************************************************/
void DownChanne0_Init(void)
{
    struct CommuniPortInfoStruct USCIA0Info;
    unsigned char Data;
    if( ReadUSCIA0Info(&USCIA0Info) )
    { 
        USCIA0_UARTSet(USCIA0Info.Band,USCIA0Info.Pariy,USCIA0Info.Data,USCIA0Info.Stop);
    }
    else
    {
        USCIA0_UARTSet(2400,2,8,1);
    }
    if( MeterReadDataRead(&Data) )
    {
        if( Data == 0xff )
        {
            DiaAlarm();
        }
        else
        {
            SetAlarm(0x00,0x00,Data);
        }
    }
    else
    {
        DiaAlarm();
    }
    
}


/******************************************************************************
  函数(模块)名称:unsigned char DownChanne0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
  功能:下行通道多字节发送函数	
  输入参数:字节缓冲指针，长度         	              		
  输出参数: 无         	 		   		 
  其它说明:属 
*******************************************************************************/
unsigned char DownChanne0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
{
    return USCIA0_MultiByteTX(SendBuffer,BufferLent);
}



/******************************************************************************
  函数(模块)名称:void RecievProcess(struct Down0FrameStruct *FrameTemp)
  功能:	读计量数据的数据处理
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:
*******************************************************************************/
void RecievProcess(struct Down0FrameStruct *FrameTemp)
{
      unsigned char DataBuffer[35];
      unsigned temp;
      if( DateMeterReadInfo.Status & BIT0 )//
      {
          StringCopy(&FrameTemp->FrameBuffer[14],DataBuffer,5);
          if( ReadRTC(&DataBuffer[5]) )
          {
              DataBuffer[12] = SumCheck(DataBuffer,12);
              
              //temp = ((DataBuffer[9]&0xF0)>>4)*10+(DataBuffer[9]&0x0F); 
              
              switch(DataBuffer[9])
              {
              case 0x00:
              case 0x01:
                temp = 0;
                break;
                case 0x02:
                temp = 1;
                break;
                case 0x03:
                temp = 2;
                break;
                case 0x04:
                temp = 3;
                break;
                case 0x05:
                temp = 4;
                break;
                case 0x06:
                temp = 5;
                break;
                case 0x07:
                temp = 6;
                break;
                case 0x08:
                temp = 7;
                break;
                case 0x09:
                temp = 8;
                break;
                case 0x10:
                temp = 9;
                break;
                case 0x11:
                temp = 10;
                break;
                case 0x12:
                temp = 11;
                break;
              default:
                break;
              }
              Storage_Module_Write(ReadMeasureDataInfo.DeviceListItem.DeviceDataPoint+temp*13+35,DataBuffer,13,SPIFlashMod);
          }
      }
      else
      {
          DataBuffer[0] = ReadMeasureDataInfo.MeterType;
          StringCopy(ReadMeasureDataInfo.DeviceListItem.DeviceIDBuff,&DataBuffer[1],7);
          StringCopy(&FrameTemp->FrameBuffer[14],&DataBuffer[8],19);
          if( ReadRTC(&DataBuffer[27]) )
          {
              DataBuffer[34] = SumCheck(DataBuffer,34);
              
              //存储数据
              Storage_Module_Write(ReadMeasureDataInfo.DeviceListItem.DeviceDataPoint,DataBuffer,35,SPIFlashMod);
          }
      }

}



/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOn(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
unsigned char ReadMeasuData_TaskOn(void)
{
    if( 0 == (DownChanne0_TaskInfo.Status & BIT0) )
    {
        DownChanne0_TaskInfo.Status |= BIT0;
        DownChanne0_TaskInfo.TaskID = 0X00;
        return 1;
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOFF(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
void ReadMeasuData_TaskOFF(void)
{
      DownChanne0_TaskInfo.Status &= ~BIT0;
      DownChanne0_TaskInfo.TaskID = 0XFF;
      
      ReadMeasureDataInfo.Status &= ~MeterReadTask_Bit;
      ReadMeasureDataInfo.MeterType = 0;
      ReadMeasureDataInfo.CurrenStatus = 0x00;  
      ReadMeasureDataInfo.MeterListNum = 0;
      
      USCIA0_FrameAckTime_En(0);
      USCIA0_FrameAckTime_SubEn(0);
      USCIA0_FrameAckTime_TimeCntWrite(0);    //重新初始化任务读数
      USCIA0_FrameAckTime_ReTryCntWrite(0);
      
      DateMeterReadInfo.Status &= ~BIT0;
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  功能:	申请读计量数据任务函数
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:若成功，则置起下行通道主任务标志位，并且重新初始化任务计数器
           并配置好读计量数据任务的信息
*******************************************************************************/
unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
{
    if( ReadMeasuData_TaskOn() )
    {
        ReadMeasureDataInfo.MeterType = DeviceType;
        ReadMeasureDataInfo.Status |= MeterReadTask_Bit;
        ReadMeasureDataInfo.CurrenStatus = 0x00;
        return 1;
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:static unsigned char ReadDeviceListItem(void)
  功能:	按顺序读出一条列表信息
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 根据设备类型、序号读出设备列表中相应的条目，根据条目信息进行抄表操作
*******************************************************************************/
static unsigned char ReadMeasureData_DeviceListItem(void)
{
     while( ReadMeasureDataInfo.MeterListNum < DeviceCntMax )
     {
         if( ReadDeviceListItem_Ext(ReadMeasureDataInfo.MeterType,ReadMeasureDataInfo.MeterListNum, 
                                 &ReadMeasureDataInfo.DeviceListItem) )
         {
             return 1;
         }
         else ReadMeasureDataInfo.MeterListNum++;
     }
     return 0;    
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasureData_CommeSend(void)
  功能:	发送任务命令
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char ReadMeasureData_CommeSend(void)
{
    unsigned char DataBuffer[3];
    DataBuffer[0] = 0x90;    //数据标识
    DataBuffer[1] = 0x1F;
    DataBuffer[2] = 0x00;    //SER
    return CJ188_CommendPackSend_USCIA0(ReadMeasureDataInfo.MeterType,ReadMeasureDataInfo.DeviceListItem.DeviceIDBuff,
                                 0X01,0X03,DataBuffer);
}


/******************************************************************************
  函数(模块)名称:void ReadMeasureData_CJ188_ReceProce(struct Down0FrameStruct *FrameTemp)
  功能:	读计量数据任务的接收188解码
  输入参数:帧结构体指针         	              		
  输出参数:          	 		   		 
  其它说明:从下行通道缓冲区取得一帧数据，并进行解码，以判断是否为对应的帧命令
           00表示无数据，01表示数据错,02表示反射，03表示数据对
*******************************************************************************/
unsigned char  CJ188_ReceProce(struct Down0FrameStruct *FrameTemp,unsigned char DeviceType,
                               unsigned char *DeviceIDBuffer)
{
    unsigned char ProcessFlag = 0;  //00表示无数据，01表示数据错,02表示反射，03表示数据对
    //unsigned char SlaveFrameBuff[14];
    unsigned char MasterFrameBuff[14];
    
    MasterFrameBuff[0] = 0X68;
    MasterFrameBuff[1] = DeviceType;
    StringCopy(DeviceIDBuffer,&MasterFrameBuff[2],7);
    MasterFrameBuff[9] = 0X01;
    MasterFrameBuff[10] = 0X03;
    MasterFrameBuff[11] = 0X90;
    MasterFrameBuff[12] = 0X1F;
    MasterFrameBuff[13] = 0X00;
    
    //StringCopy(MasterFrameBuff,SlaveFrameBuff,14);
    
    //SlaveFrameBuff[13]++;
    //SlaveFrameBuff[10] = 0X16;
    //SlaveFrameBuff[9] |= BIT7;
    
    if( USCIA0_ReadRXBuffFrame() || (USCIA0_ReadRXBuffStatus()&BufferOV_BIT) )
    {
        if( Down0CJ188_GetFrame( FrameTemp ) )           //从缓冲区中检出一帧数据
        {
            if( Down0CJ188_FrameCheckSum(FrameTemp) ) //帧校验
            {
                if( FrameTemp->FrameBuffer[9] & BIT7 )
                {   //从机发出的帧数据
                    MasterFrameBuff[9]|= BIT7;
                    MasterFrameBuff[10]= 0X16;
                    MasterFrameBuff[13]++;
                    if( IsEqual( FrameTemp->FrameBuffer,MasterFrameBuff,14 ) && (35 == FrameTemp->FrameLent) )
                    {
                        ProcessFlag = 3;//正确的数据
                    }
                    else
                    {
                        ProcessFlag = 1;//数据有误
                    }
                }
                else
                {
                    //主机发出的帧数据,忽略，打开计时子开关
                    if( IsEqual( FrameTemp->FrameBuffer,MasterFrameBuff,14 ) && (16 ==FrameTemp->FrameLent) )
                    {
                        ProcessFlag = 2; //是反射数据
                    }
                    else
                    {
                        ProcessFlag = 1; //数据有误
                    }
                }
            }
            else
            {
                //有帧数据，但是校验出错，当次出错，重试下一次
                ProcessFlag = 1;//数据有误
            }
        }
        else
        {
            //没有帧数据或数据非法，当次出错，重试下一次
            ProcessFlag = 1;//数据有误
        }
    }
    else
    {
        ProcessFlag = 0;//没有数据
    }
    return ProcessFlag;    
}


/******************************************************************************
  函数(模块)名称:void ReadMeasureData_Task(void)
  功能:	读计量数据任务
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:分3个阶段
*******************************************************************************/
void ReadMeasureData_Task(void)
{
    struct Down0FrameStruct FrameTemp;
    if( ReadMeasureDataInfo.Status & MeterReadTask_Bit )//抄表任务开启
    {
        switch( ReadMeasureDataInfo.CurrenStatus )
        {
            case 0x00:  //初始化阶段
              if( DateMeterReadInfo.Status & BIT0 )//
              {
                  MBUSOn();
                  delay_ms(4000);
              }
              USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);
              USCIA0_FrameAckTime_ReTryCntWrite(3);
              ReadMeasureDataInfo.CurrenStatus = 0X01; //转到命令发送阶段
              
            case 0x01: //命令发送阶段
              if( ReadMeasureData_DeviceListItem() )  //从从机列表中取出一个有效的条目
              {
                  USCIA0_RXBuffClearn();
                  if( ReadMeasureData_CommeSend() )   //发送抄表命令
                  {
                      ReadMeasureDataInfo.CurrenStatus = 0X02;//发送成功，则转到下一个阶段
                      USCIA0_FrameAckTime_En(1); //计时任务总开关开，当命令帧发送完后，会马上打开计时任务子开关
                  }
                  else
                  {
                      ReadMeasureDataInfo.CurrenStatus = 0X01;//发送不成功，则转回到当前阶段
                  }
              }
              else 
              {  //从机列表中已经没有有效列表或已经搜寻完列表
                  if( DateMeterReadInfo.Status & BIT0 )//
                  {
                      MBUSOFF();
                  }                
                  ReadMeasuData_TaskOFF();//抄表任务停止
              }
              break;
              
            case 0x02: //接收处理阶段
              if( USCIA0_FrameAckTime_ReTryCntRead() )     //检查尝试次数
              {
                  if( USCIA0_FrameAckTime_TimeCntRead() )  //检查是否超时
                  {
                      //ReadMeasureData_CJ188_ReceProce( &FrameTemp ); //读计量数据任务的接收解码处理
                      switch( CJ188_ReceProce(&FrameTemp,ReadMeasureDataInfo.MeterType,
                                              ReadMeasureDataInfo.DeviceListItem.DeviceIDBuff ) )
                      {
                      case 0x00://没有数据
                        break;
                      case 0x01://数据有误
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum); //重新加载时间读数
                          USCIA0_FrameAckTime_ReTryCntDec();            //次数减1
                          ReadMeasureDataInfo.CurrenStatus = 0X01;          //转到阶段,重新尝试
                        break;
                      case 0x02://数据反射
                          USCIA0_FrameAckTime_SubEn(1);//重新开始计时
                        break;
                      case 0x03://数据正确
                          RecievProcess(&FrameTemp);  //处理数据
                          
                          ReadMeasureDataInfo.MeterListNum++;          //指向下一个表
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);    //重新初始化任务读数
                          USCIA0_FrameAckTime_ReTryCntWrite(3);
                          ReadMeasureDataInfo.CurrenStatus = 0X01;             //转到命令阶段  
                        break;
                      default:
                        ReadMeasuData_TaskOFF();//错误，提前结束
                        break;
                      }
                  }
                  else
                  {
                      //超时错误，转到下一次
                      USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum); //重新加载时间读数
                      USCIA0_FrameAckTime_ReTryCntDec();            //次数减1         
                      ReadMeasureDataInfo.CurrenStatus = 0X01;   //转到阶段,重新尝试
                  }
              }
              else
              {
                  //超过错误次数，转到下一个表
                  ReadMeasureDataInfo.MeterListNum++;     //指向下一个表
                  USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);//重新初始化任务读数
                  USCIA0_FrameAckTime_ReTryCntWrite(3);   
                  ReadMeasureDataInfo.CurrenStatus = 0X01;//转到命令阶段
              }
              break;
              
            default:
              ReadMeasuData_TaskOFF();//错误，抄表任务停止
              break;
        }
    }
}



/******************************************************************************
  函数(模块)名称:unsigned char SeareCommeSend(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	向指定类型及表号的表发送命令
  输入参数:无         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char SeareCommeSend(void)
{
    unsigned char DataBuffer[3];
    DataBuffer[0] = 0x90;
    DataBuffer[1] = 0x1f;
    DataBuffer[2] = 0x00;
    return CJ188_CommendPackSend_USCIA0(SeareDeviceInfo.DeviceType,SeareDeviceInfo.CurrenDeviceStarID,0X01,0X03,DataBuffer);
    
}


/******************************************************************************
  函数(模块)名称:unsigned char SeareCommeSend(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  功能:	向指定类型及表号的表发送命令
  输入参数:无         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char SeareCommeSendTEST(void)
{
    unsigned char DataBuffer[3];
    unsigned char CurrenDeviceStarID[7]={0,0,0,0,0,0,0};
    DataBuffer[0] = 0x90;
    DataBuffer[1] = 0x1f;
    DataBuffer[2] = 0x00;
    for(unsigned char i=1;i<9;i++)
    {
      CurrenDeviceStarID[0] = i;
      CJ188_CommendPackSend_USCIA0(0X10,CurrenDeviceStarID,0X01,0X03,DataBuffer);
      delay_ms(2000);
    }
    
}

/******************************************************************************
  函数(模块)名称:void DeviceIDInc(unsigned char *DeviceBuffer)
  功能:	表号递增1
  输入参数:无         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
void DeviceIDInc(unsigned char *DeviceBuffer)
{
    unsigned char LongCharBuff[8];
    StringCopy(DeviceBuffer,LongCharBuff,7);
    *( (unsigned long long *)(LongCharBuff) ) = __bcd_add_long_long( *( (unsigned long long *)(LongCharBuff) ),1);
    StringCopy(LongCharBuff,DeviceBuffer,7);
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOn(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
unsigned char SeareDevcie_TaskOn(void)
{
    if( 0 == (DownChanne0_TaskInfo.Status & BIT0) )
    {
        DownChanne0_TaskInfo.Status |= BIT0;
        DownChanne0_TaskInfo.TaskID = 0X01;
        return 1;
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskOFF(void)
  功能:	读计量数据任务申请
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
void SeareDevcie_TaskOFF(void)
{
      DownChanne0_TaskInfo.Status &= ~BIT0;//清主任务标志位及任务ID号
      DownChanne0_TaskInfo.TaskID = 0XFF;
      
      SeareDeviceInfo.Status &= ~BIT0;
      SeareDeviceInfo.DeviceType = 0;
      SeareDeviceInfo.CurrenStatus = 0x00;  
      SeareDeviceInfo.SeareCnt = 0;
      
      USCIA0_FrameAckTime_En(0);
      USCIA0_FrameAckTime_SubEn(0);
      USCIA0_FrameAckTime_TimeCntWrite(0);    //重新初始化任务读数
      USCIA0_FrameAckTime_ReTryCntWrite(0);
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  功能:	
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:申请成功，则返回1
*******************************************************************************/
unsigned char SeareDevcie_TaskRequ(unsigned char DeviceType,unsigned char *DeviceIDBuff,
                                     unsigned char SeareLent)
{
    if( SeareDevcie_TaskOn() )
    {
        SeareDeviceInfo.Status |= BIT0;
        SeareDeviceInfo.DeviceType = DeviceType;
        StringCopy(DeviceIDBuff,SeareDeviceInfo.CurrenDeviceStarID,7);
        SeareDeviceInfo.CurrenStatus = 0x00;  
        SeareDeviceInfo.SeareLent = SeareLent;
        SeareDeviceInfo.SeareCnt = 0;
        return 1;
    }
    else return 0;
}



/******************************************************************************
  函数(模块)名称:void SeareDevcie_Task(void)
  功能:	表搜寻任务
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:分3个阶段,入口参数为表类型，起始表号以及搜索长度
           若相对应的从表有应答，则把对应表号加入从表列表内
           主机向从机发送完命令帧后，会马上启动帧响应计时
*******************************************************************************/
void SeareDevcie_Task(void)
{
    struct Down0FrameStruct FrameTemp;
    if( SeareDeviceInfo.Status & BIT0 )//查询搜表任务标志位
    {
        switch( SeareDeviceInfo.CurrenStatus )//查询任务状态 
        {
            case 0x00:  //初始化阶段
              USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);//响应时间
              USCIA0_FrameAckTime_ReTryCntWrite(3);//错误尝试次数
              SeareDeviceInfo.CurrenStatus = 0X01; //转到命令发送阶段
              
            case 0x01:  //命令发送阶段
              if( SeareDeviceInfo.SeareCnt < SeareDeviceInfo.SeareLent  )//查询是否搜索完成
              {
                  USCIA0_RXBuffClearn(); //清接收缓冲区
                  if( SeareCommeSend() ) //发送抄表命令
                  {
                      SeareDeviceInfo.CurrenStatus = 0X02;//发送成功，则转到下一个阶段
                      USCIA0_FrameAckTime_En(1); //计时任务总开关开，当命令帧发送完后，会马上打开计时任务子开关
                  }
                  else
                  {
                      SeareDeviceInfo.CurrenStatus = 0X01;//发送不成功，则转回到当前阶段
                  }
              }
              else                         //从机列表中已经没有有效列表或已经搜寻完列表
              {   //搜索完成
                  SeareDevcie_TaskOFF();
              }
              break;
              
            case 0x02://接收处理阶段
              if( USCIA0_FrameAckTime_ReTryCntRead() )     //检查尝试次数
              {
                  if( USCIA0_FrameAckTime_TimeCntRead() ) //检查是否超时
                  {
                      switch( CJ188_ReceProce(&FrameTemp,SeareDeviceInfo.DeviceType,
                                              SeareDeviceInfo.CurrenDeviceStarID ) )
                      {
                      case 0x00://没有数据
                        break;
                      case 0x01://数据有误
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum); //重新加载时间读数
                          USCIA0_FrameAckTime_ReTryCntDec();            //次数减1
                          SeareDeviceInfo.CurrenStatus = 0X01;          //转到阶段,重新尝试
                        break;
                      case 0x02://数据反射
                          USCIA0_FrameAckTime_SubEn(1);//重新开始计时
                        break;
                      case 0x03://数据正确
                          //存储表号
                          Add_DeviceListItem(SeareDeviceInfo.DeviceType,SeareDeviceInfo.CurrenDeviceStarID);
                          DeviceIDInc(SeareDeviceInfo.CurrenDeviceStarID); //当前表号加1
                          SeareDeviceInfo.SeareCnt++;                      //指向下一个表
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);    //重新初始化任务读数
                          USCIA0_FrameAckTime_ReTryCntWrite(3);
                          SeareDeviceInfo.CurrenStatus = 0X01;             //转到命令阶段  
                        break;
                      default:
                        SeareDevcie_TaskOFF();//错误，提前结束
                        break;
                      }
                  }
                  else
                  {   //DownChanne0_TaskInfo.TryTime 次连接超时，则重新下一个
                      USCIA0_FrameAckTime_TimeCntWrite( ACKTimeNum ); //重新加载时间读数
                      USCIA0_FrameAckTime_ReTryCntDec();              //次数减1
                      SeareDeviceInfo.CurrenStatus = 0X01;            //转到阶段,重新尝试
                  }
              }
              else
              {
                  DeviceIDInc(SeareDeviceInfo.CurrenDeviceStarID);//当前表号加1
                  SeareDeviceInfo.SeareCnt++;                     //指向下一个表
                  USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);   //重新初始化任务读数
                  USCIA0_FrameAckTime_ReTryCntWrite(3);
                  SeareDeviceInfo.CurrenStatus = 0X01;            //转到命令阶段
              }
              break;
              
            default:
              break;
        }
    }
}


/******************************************************************************
  函数(模块)名称:void DownChanne0_TaskProcess(void)
  功能:	下行通道0任务处理函数
  输入参数:无         	              		
  输出参数:         	 		   		 
  其它说明:下行通道的所有任务均在此任务函数内处理
*******************************************************************************/
void DownChanne0_TaskProcess(void)
{
    
    if( ReadRTCFlag()&BIT0 || CheckReadMeterData() )
    {
        if( ReadMeasuData_TaskRequ(0x30) )
        {//申请任务成功
            
            WriteRTCFlag( ReadRTCFlag()&(~BIT0) );
            DateMeterReadInfo.Status |= BIT0;
        }
    }
  
    if( DownChanne0_TaskInfo.Status & BIT0 )
    {
        switch( DownChanne0_TaskInfo.TaskID )
        {
        case 0x00: 
          ReadMeasureData_Task();  //下行通道读计量数据任务
          break;
        case 0x01:
          SeareDevcie_Task();       //下行通道搜表任务
          break;
        default:
          DownChanne0_TaskInfo.Status &= ~BIT0;
          break;
        }
    }
}

unsigned char ReadDownChanne0_TaskStatus(void)
{
    return DownChanne0_TaskInfo.Status;
}

unsigned char ReadDownChanne0_TaskID(void)
{
    return DownChanne0_TaskInfo.TaskID;
}
unsigned char ReadDateMeterInfo(void)
{
    return DateMeterReadInfo.Status;
}
