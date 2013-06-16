  
//************************************************************************
//  处理上行通道的188协议解析、打包处理
//  Lierda, Inc    
//  LJZ
//************************************************************************

#include "msp430x54x.h"
#include "stdlib.h"
#include "..\DownChannel0_Module\DownChannel0_Module.H"
#include "..\..\Application Layer\Application_Layer.H" //提供协议解析后需要的各个功能实现代码
#include ".\USCIA1_UART_Module\USCIA1_UART_Module.H" //上行通道层通讯
#include "Up1CJ188_Module.H"


/******************************************************************************
  函数(模块)名称:static unsigned char FrameCheckSum(struct Up1FrameStruct *FrameTemp)
  功能:检查帧校验和	
  输入参数:         	              		
  输出参数:         	 		   		 
  其它说明:属于内部函数
*******************************************************************************/
static unsigned char FrameCheckSum(struct Up1FrameStruct *FrameTemp)
{
    if(FrameTemp->FrameBuffer[FrameTemp->FrameLent-1-1] == SumCheck( FrameTemp->FrameBuffer,FrameTemp->FrameLent-1-1 ) )
      return 1;
    else
      return 0;
}

/******************************************************************************
  函数(模块)名称:CJ188_CommendPack(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
  功能:	188协议打包函数
  输入参数: 设备类型、设备ID，控制码，数据长度以及数据缓冲区
  输出参数: 成功或失败         	 		   		 
  其它说明: 若内存不足或串口忙，会导致失败。
*******************************************************************************/
static unsigned char CJ188_CommendPackSend(unsigned char MeterType,unsigned char *MeterIDbuff,unsigned char CTL_Code,
                  unsigned char DataLent,unsigned char *DataBuffer)
{
    unsigned int i;
    unsigned char *FrameBuffer;
    FrameBuffer = (unsigned char *)malloc(15+DataLent); //申请帧数据空间
    if( NULL == FrameBuffer )//没有申请到，则返回
        return 0;
    else
    {
        FrameBuffer[0] = 0xFE;
        FrameBuffer[1] = 0xFE;
        FrameBuffer[2] = 0x68;         //帧开始字符
        FrameBuffer[3] = MeterType;    //设备类型
        for(i=4;i<11;i++)              //设备ID
          FrameBuffer[i] = *MeterIDbuff++;
        FrameBuffer[11] = CTL_Code;    //控制码
        FrameBuffer[12] = DataLent;    //数据长度
        for(i=13;i<DataLent+13;i++)    //数据域
          FrameBuffer[i] = *DataBuffer++;
        FrameBuffer[i++] = SumCheck( &FrameBuffer[2],11+DataLent );         //校验码
        FrameBuffer[i] = 0x16;         //帧停止字符
        
        i = USCIA1_MultiByteTX(FrameBuffer,15+DataLent);
        
        free(FrameBuffer);            //释放申请到的空间
        return i;
    }
}

/******************************************************************************
  函数(模块)名称:static unsigned char GetFrame(struct Up1FrameStruct *FrameTemp)
  功能:	从接收缓冲区中检出一帧数据
  输入参数:帧数据结构体指针，用来存储检出的帧数据         	              		
  输出参数: 1成功，0无帧数据输出         	 		   		 
  其它说明:检出的帧数据，可以确保是完整的，包括帧头，帧尾，数据域长度以及整个帧
           数据的合法长度，检测标准按188协议检测.
  注:不检测帧内的校验和、设备类型以及设备ID、控制码
*******************************************************************************/
static unsigned char GetFrame(struct Up1FrameStruct *FrameTemp)
{
    unsigned int  BufferLent;
    unsigned char ReadData;
    BufferLent = USCIA1_ReadRXBuffSize();     //取得当前缓冲区大小
    if(BufferLent >= 13)                      //算上68，一帧数据最少13个字节
    {
        while(BufferLent--)
        { 
            if( USCIA1_ReadRXBuffer(&ReadData) )
            {
                if( 0x68 == ReadData)        //取得缓冲区一个字节数据//帧头判断
                {
                    if(BufferLent > 12) 
                    {
                        ReadData = USCIA1_ReadBufferOffset(9); //取数据域长度字节
                        if( BufferLent >= (10+ReadData+2) )    //缓冲区字节数大于帧数据域长度＋校验字节和帧结束字节
                        {
                            ReadData = USCIA1_ReadBufferOffset( 9+ReadData+1+1 );//取得帧结束符位置 数据域长度＋校验符长度
                            if(0x16 == ReadData)
                            {
                                ReadData = USCIA1_ReadBufferOffset(9);           //取数据域长度字节
                                FrameTemp->FrameBuffer[0]=0x68;
                                FrameTemp->FrameLent = 13+ ReadData;
                                for(unsigned int i=0;i<FrameTemp->FrameLent-1;i++)
                                {
                                    USCIA1_ReadRXBuffer( &FrameTemp->FrameBuffer[i+1] );
                                }
                                return 1;
                            }
                            continue;
                        }
                        continue;
                    }
                    else return 0;//当前不够一帧数据
                }
            }
            else return 0;
        }
        return 0;     //已经取完数据
    }
    else
    {
        USCIA1_FrameDec();
        return 0;        //缓冲区数据长度不够
    }
}


/******************************************************************************
  函数(模块)名称:static unsigned char ReadFun_MeterReadData( struct Up1FrameStruct *FrameTemp )
  功能:	读抄表日命令功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 需要提供抄表日数据
*******************************************************************************/
static unsigned char ReadFun_MeterReadData( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuffer[4];    
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            //读抄表日命令功能实现函数
            DataBuffer[0] = FrameTemp->FrameBuffer[11];    //DI0
            DataBuffer[1] = FrameTemp->FrameBuffer[12];    //DI1
            DataBuffer[2] = FrameTemp->FrameBuffer[13]+1;  //SER+1
            if( MeterReadDataRead( &DataBuffer[3] ) )      //取得抄表日数据
            {
                return CJ188_CommendPackSend( FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                             FrameTemp->FrameBuffer[9]|BIT7,4,DataBuffer);
            }
            else return 0;  //读抄表数据出错
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}


/******************************************************************************
  函数(模块)名称:static unsigned char ReadDataType_Decode(struct Up1FrameStruct *FrameTemp )
  功能:	标准读数据命令解析
  输入参数: 解析的命令帧指针
  输出参数: 成功或失败         	 		   		 
  其它说明:  
*******************************************************************************/
static unsigned char ReadDataType_Decode(struct Up1FrameStruct *FrameTemp )
{
    unsigned int DataType = ((unsigned int)FrameTemp->FrameBuffer[11])<<8 | ((unsigned int)FrameTemp->FrameBuffer[12]);
    switch( DataType )
    {
        case 0X901F:        //读计量数据
          return 0;
        case 0x8104:        //读抄表日期
          return ReadFun_MeterReadData( FrameTemp ); //读抄表日期功能函数  
        default:
          return 0;
    }    
}


/******************************************************************************
  函数(模块)名称:static ReadAddr_Decode(struct Up1FrameStruct *FrameTemp )
  功能:	广播读地址命令功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 会解析数据标识 0X810A以及是否是广播地址: AA AA AA AA AA AA AA
            需要提供采集器ID
*******************************************************************************/
static unsigned char ReadFun_DeviceAddr(struct Up1FrameStruct *FrameTemp )
{
    unsigned int DataType = ((unsigned int)FrameTemp->FrameBuffer[11])<<8 | ((unsigned int)FrameTemp->FrameBuffer[12]);
    unsigned char DeviceIDBuffer[7];
    unsigned char DataBuffer[3];
    
    if( FrameTemp->FrameBuffer[9] & BIT6 )
    {   //通讯不正常
        return 0;
    }
    else
    {   //通讯正常
        switch( DataType )
        {
            case 0X810A:        //读地址数据标识符
              for( unsigned int i=2;i<9;i++) //判断是否广播地址
              {
                  if( 0xAA == FrameTemp->FrameBuffer[i] )
                    continue;
                  else 
                    return 0;
              }
              if( ReadDeviceID( DeviceIDBuffer ) )//读采集器ID号
              {              
                  DataBuffer[0] = FrameTemp->FrameBuffer[11];    //DI0
                  DataBuffer[1] = FrameTemp->FrameBuffer[12];    //DI1
                  DataBuffer[2] = FrameTemp->FrameBuffer[13]+1;  //SER+1
                  return CJ188_CommendPackSend( FrameTemp->FrameBuffer[1],DeviceIDBuffer,
                                               FrameTemp->FrameBuffer[9]|BIT7,0x03,DataBuffer);  //188命令打包
              }
              else return 0;//读采集器ID号出错
            default:
              return 0;
        }
    }     
}

/******************************************************************************
  函数(模块)名称:static unsigned char WriteFun_DeviceAddr(struct Up1FrameStruct *FrameTemp )
  功能:	写地址命令功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 会解析数据标识 0X810A以及是否是广播地址: AA AA AA AA AA AA AA
*******************************************************************************/
static unsigned char WriteFun_DeviceAddr(struct Up1FrameStruct *FrameTemp )
{
    unsigned int DataType = ((unsigned int)FrameTemp->FrameBuffer[11])<<8 | ((unsigned int)FrameTemp->FrameBuffer[12]);

    unsigned char DeviceIDBuffer[7];
    unsigned char DataBuffer[3];
    
    if( FrameTemp->FrameBuffer[9] & BIT6 )
    {   //通讯不正常
        return 0;
    }
    else
    {   //通讯正常
        switch( DataType )
        {
            case 0XA018:        //读地址数据标识符
              for( unsigned int i=2;i<9;i++) //判断是否广播地址
              {
                  if( 0xAA == FrameTemp->FrameBuffer[i] )
                    continue;
                  else 
                    return 0;
              }
              StringCopy( &FrameTemp->FrameBuffer[14],DeviceIDBuffer,7);//从命令中取出要写的表号
              if( WriteDeviceID( DeviceIDBuffer ) )//写采集器ID号
              { 
                  if( ReadDeviceID( DeviceIDBuffer ) )//读采集器ID号
                  {
                      DataBuffer[0] = FrameTemp->FrameBuffer[11];    //DI0
                      DataBuffer[1] = FrameTemp->FrameBuffer[12];    //DI1
                      DataBuffer[2] = FrameTemp->FrameBuffer[13]+1;  //SER+1
                      return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],DeviceIDBuffer,
                                               FrameTemp->FrameBuffer[9]|BIT7,0x03,DataBuffer);  //188命令打包
                  }
                  else return 0; //读采集器ID号出错
              }
              else return 0;     //写采集器ID号出错
            default:
              return 0;
        }
    }
}



/******************************************************************************
  函数(模块)名称:static unsigned char WriteFun_MeterReadData( struct Up1FrameStruct *FrameTemp )
  功能:	写抄表日命令功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 需要提供抄表日数据
*******************************************************************************/
static unsigned char WriteFun_MeterReadData( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuffer[4];    
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            //写抄表日命令功能实现函数
            if( MeterReadDataWrite( &FrameTemp->FrameBuffer[14] ) )  //写抄表日数据
            {
                DataBuffer[0] = FrameTemp->FrameBuffer[11];    //DI0
                DataBuffer[1] = FrameTemp->FrameBuffer[12];    //DI1
                DataBuffer[2] = FrameTemp->FrameBuffer[13]+1;  //SER+1
                return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                             FrameTemp->FrameBuffer[9]|BIT7,3,DataBuffer);
            }
            else return 0;  //写抄表数据出错
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char WriteFun_StandardData( struct Up1FrameStruct *FrameTemp )
  功能:	写标准时间功能函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char WriteFun_StandardData( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuffer[3];    
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            if( SetRTCFun( &FrameTemp->FrameBuffer[14] ) )  //写标准时间
            {
                DataBuffer[0] = FrameTemp->FrameBuffer[11];    //DI0
                DataBuffer[1] = FrameTemp->FrameBuffer[12];    //DI1
                DataBuffer[2] = FrameTemp->FrameBuffer[13]+1;  //SER+1
                return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                             FrameTemp->FrameBuffer[9]|BIT7,0x03,DataBuffer);
            }
            else return 0;  //时间设定出错
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char WriteDataType_Decode(struct Up1FrameStruct *FrameTemp )
  功能:	读数据命令
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明:  
*******************************************************************************/
static unsigned char WriteDataType_Decode(struct Up1FrameStruct *FrameTemp )
{
    unsigned int DataType = ((unsigned int)FrameTemp->FrameBuffer[11])<<8 | ((unsigned int)FrameTemp->FrameBuffer[12]);
    switch( DataType )
    {
      case 0xA012:       //写抄表日期
        return WriteFun_MeterReadData( FrameTemp ); 
      case 0xA015:       //写标准时间
        return WriteFun_StandardData( FrameTemp ); 
      default:
        return 0;
    }
}




/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_DeviceItem( struct Up1FrameStruct *FrameTemp )//扩展读表号
  功能:	根据数据标识返回相应的序号及表号以及表类型
  输入参数: 数据标识
  输出参数: 成功或失败         	 		   		 
  其它说明: 返回数据:DI0 DI1 SER 设备类型 序号 ID共 12个字节数据 
            unsigned char ExtReadDeviceIDRead( unsigned char ListNum,unsigned char *DataBuff )
*******************************************************************************/
static unsigned char ExtReadFun_DeviceItem( struct Up1FrameStruct *FrameTemp )//扩展读表号
{
    unsigned char DataBuff[12];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            if( ExtReadDeviceItem( FrameTemp->FrameBuffer[12],&DataBuff[3] ) ) //根据指定的序号，取得设备类型，序号以及表号，9个字节
            {
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,12,DataBuff);
            }
            else return 0;
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}



/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_DeviceCurrenData( struct Up1FrameStruct *FrameTemp )
  功能:	根据数据标识返回相应的序号对应的表号的数据
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 返回数据:DI0 DI1 SER 设备类型 序号 ID共 12个字节数据 
            unsigned char ExtReadDeviceIDRead( unsigned char ListNum,unsigned char *DataBuff )
*******************************************************************************/
static unsigned char ExtReadFun_DeviceCurrenData( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[38];
    
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            
            if( ReadDeviceCurrenData(FrameTemp->FrameBuffer[12],&DataBuff[3]) ) //根据指定的序号，取得设备当前数据，35个字节
            {
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,38,DataBuff);
            }
            else return 0;
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}


/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_StandardData( struct Up1FrameStruct *FrameTemp )
  功能:	扩展读标准时间
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtReadFun_StandardData( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[11];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常   
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            DataBuff[3] = ReadRTCFun( &DataBuff[4] );
            return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                         FrameTemp->FrameBuffer[9]|BIT7,11,DataBuff);
        } 
    }
    else return 0;//表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_StandardData( struct Up1FrameStruct *FrameTemp )
  功能:	读标准时间
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtReadFun_DownBand( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[10];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            ReadUSCIA0( &DataBuff[3] );
            return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,10,DataBuff);
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_DeviceTypeInfo( struct Up1FrameStruct *FrameTemp )
  功能:	读设备类型信息
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtReadFun_DeviceTypeInfo( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[7];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
        
            if( ExtReadDeviceTypeInfoFun( &DataBuff[3] ) )
            {
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,7,DataBuff);
            }
            else return 0;
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_DeviceTypeInfo( struct Up1FrameStruct *FrameTemp )
  功能:	读设备类型信息
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtReadFun_TaskInfo( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[4];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常 
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            DataBuff[3] = DownChanne0_ReadStatus();
            return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,4,DataBuff);
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}


/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_CollectorInfo( struct Up1FrameStruct *FrameTemp )
  功能:	读设备类型信息
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtReadFun_CollectorInfo( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[52];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常  
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            ReadCollectorInfoFun( &DataBuff[3] );
            return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,52,DataBuff);
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_StandardData( struct Up1FrameStruct *FrameTemp )
  功能:	读标准时间
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtReadFun_ALLMeterData( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[194];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            
            if( ReadMeterAllDataFun( FrameTemp->FrameBuffer[12], &DataBuff[3]) )
            {
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,194,DataBuff);
            }
            else
            {
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,3,DataBuff);
            }
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错   
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadDataType_Decode( struct Up1FrameStruct *FrameTemp )
  功能:	扩展的读功能解码
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtReadDataType_Decode( struct Up1FrameStruct *FrameTemp )
{
    unsigned int DataType = ((unsigned int)FrameTemp->FrameBuffer[11])<<8 | ((unsigned int)FrameTemp->FrameBuffer[12]);
    
    if( 0x10 == FrameTemp->FrameBuffer[11] )
    {
        return ExtReadFun_DeviceItem( FrameTemp );     //扩展读表号信息
    }
    else if( 0x20 == FrameTemp->FrameBuffer[11] )
    {
        return ExtReadFun_DeviceCurrenData( FrameTemp );//扩展读表号数据  
    }
    else if( 0x30 == FrameTemp->FrameBuffer[11] )
    {
        return ExtReadFun_ALLMeterData( FrameTemp );    //扩展读表全部数据
    }
    
    switch( DataType )
    {
      case 0XA015:
        return ExtReadFun_StandardData( FrameTemp );    //扩展读标准时间数据 
      case 0XA016:
        return ExtReadFun_DownBand( FrameTemp );        //扩展读下行波特率数据 
      case 0XA017:
        return ExtReadFun_DeviceTypeInfo( FrameTemp );  // 
      case 0XA018:
        return ExtReadFun_TaskInfo( FrameTemp );        // 
      case 0XA019:
        return ExtReadFun_CollectorInfo( FrameTemp );        // 
      default:
        return 0;
    }  
}


/******************************************************************************
  函数(模块)名称:static unsigned char ExtWriteFun_DeviceTypeItem( struct Up1FrameStruct *FrameTemp )
  功能:	根据数据标识返回相应的序号及表号
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 需要提供 设备类型 序号 ID共 9个字节数据 
            unsigned char ExtReadDeviceIDRead( unsigned char ListNum,unsigned char *DataBuff )
*******************************************************************************/
static unsigned char ExtWriteFun_DeviceTypeItem( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[5];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            if( ExtWriteDeviceTypeItem( FrameTemp->FrameBuffer[14] ) ) //把设备类型、表号加入到采集器列表中
            {
                DataBuff[0] = FrameTemp->FrameBuffer[11];   //DI0
                DataBuff[1] = FrameTemp->FrameBuffer[12];   //DI1
                DataBuff[2] = FrameTemp->FrameBuffer[13]++; //SER+1
                DataBuff[3] = FrameTemp->FrameBuffer[14];   //设备类型
                DataBuff[4] = 0xff;
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,5,DataBuff);
            }
            else return 0;
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_DeviceItem( struct Up1FrameStruct *FrameTemp )//扩展读表号
  功能:	根据数据标识返回相应的序号及表号
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 需要提供 设备类型 序号 ID共 9个字节数据 
            unsigned char ExtReadDeviceIDRead( unsigned char ListNum,unsigned char *DataBuff )
*******************************************************************************/
static unsigned char ExtWriteFun_DeviceItem( struct Up1FrameStruct *FrameTemp )//扩展读表号
{
    unsigned char DataBuff[12];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            if(ReadDeviceItemNum( FrameTemp->FrameBuffer[14],&FrameTemp->FrameBuffer[16],&DataBuff[4]))
            {
                DataBuff[0] = FrameTemp->FrameBuffer[11];   //DI0
                DataBuff[1] = FrameTemp->FrameBuffer[12];   //DI1
                DataBuff[2] = FrameTemp->FrameBuffer[13]+1; //SER+1
                DataBuff[3] = FrameTemp->FrameBuffer[14];   //设备类型
                StringCopy( &FrameTemp->FrameBuffer[16],&DataBuff[5],7);//取得表号
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,12,DataBuff);

            }
            else
            {                
              if( ExtWriteDeviceItem( FrameTemp->FrameBuffer[14],&FrameTemp->FrameBuffer[16] ) ) //把设备类型、表号加入到采集器列表中
                {
                    DataBuff[0] = FrameTemp->FrameBuffer[11];   //DI0
                    DataBuff[1] = FrameTemp->FrameBuffer[12];   //DI1
                    DataBuff[2] = FrameTemp->FrameBuffer[13]+1; //SER+1
                    DataBuff[3] = FrameTemp->FrameBuffer[14];   //设备类型
                    ReadDeviceItemNum( FrameTemp->FrameBuffer[14],&FrameTemp->FrameBuffer[16],&DataBuff[4]);
                    StringCopy( &FrameTemp->FrameBuffer[16],&DataBuff[5],7);//取得表号
                    return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,12,DataBuff);
                }
                else return 0;
            }
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错  
}


/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadFun_StandardData( struct Up1FrameStruct *FrameTemp )
  功能:	读标准时间
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtWriteFun_DownBand( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[10];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常   
            DataBuff[0] = FrameTemp->FrameBuffer[11];
            DataBuff[1] = FrameTemp->FrameBuffer[12];
            DataBuff[2] = FrameTemp->FrameBuffer[13]+1;
            WriteUSCIA0( &FrameTemp->FrameBuffer[14] );
            //DownChanne0_Init();
            return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,3,DataBuff);
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}


/******************************************************************************
  函数(模块)名称:static unsigned char ExtWriteFun_DelDeviceTypeItem( struct Up1FrameStruct *FrameTemp )
  功能:	删除设备类型
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtWriteFun_DelDeviceTypeItem( struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[3];
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) ) //判断表号
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            if( ExtDelDeviceTypeItem( FrameTemp->FrameBuffer[14] ) ) //把设备类型
            {
                DataBuff[0] = FrameTemp->FrameBuffer[11];   //DI0
                DataBuff[1] = FrameTemp->FrameBuffer[12];   //DI1
                DataBuff[2] = FrameTemp->FrameBuffer[13]++; //SER+1
                return CJ188_CommendPackSend(0x02,&FrameTemp->FrameBuffer[2],FrameTemp->FrameBuffer[9]|BIT7,3,DataBuff);
            }
            else return 0;
        } 
    }
    else return 0;          //表号不匹配或读设备表号出错
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtReadDataType_Decode( struct Up1FrameStruct *FrameTemp )
  功能:	扩展的写功能解码
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtWriteDataType_Decode( struct Up1FrameStruct *FrameTemp )
{
    unsigned int DataType = ((unsigned int)FrameTemp->FrameBuffer[11])<<8 | ((unsigned int)FrameTemp->FrameBuffer[12]);
    switch( DataType )
    {
      case 0X2000:
      return ExtWriteFun_DeviceItem( FrameTemp ); //写/增加子设备解码 
      
      case 0X2001:
      return ExtWriteFun_DeviceTypeItem( FrameTemp ); //设置设备类型解码
      
      case 0X2002:
      return ExtWriteFun_DownBand( FrameTemp );
      
      case 0X2003:
      return ExtWriteFun_DelDeviceTypeItem( FrameTemp ); //删除设备类型解码
                
      default:
        return 0;
    }  
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtCTLFun_MBUSON(  struct Up1FrameStruct *FrameTemp )
  功能:	扩展的控制指令解码,打开M-Bus总线
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtCTLFun_MBUSON(  struct Up1FrameStruct *FrameTemp ) //扩展的总线打开指令
{
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) )
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            FrameTemp->FrameBuffer[13]++;
            CTL_MBUSON();          //总线输出打开
            return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                         FrameTemp->FrameBuffer[9]|BIT7,3,&FrameTemp->FrameBuffer[11]);
        }
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtCTLFun_MBUSON(  struct Up1FrameStruct *FrameTemp )
  功能:	扩展的控制指令解码 M-Bus总线关闭
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtCTLFun_MBUSOFF(  struct Up1FrameStruct *FrameTemp ) //扩展的总线打开指令
{
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) )
    {
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常       
            FrameTemp->FrameBuffer[13]++;
            CTL_MBUSOFF();          //总线输出关闭
            return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                         FrameTemp->FrameBuffer[9]|BIT7,3,&FrameTemp->FrameBuffer[11]);
        }
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:static unsigned char ExtCTLFun_ClearALLList(  struct Up1FrameStruct *FrameTemp ) //扩展的列表清除指令
  功能:	扩展的控制指令解码
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 删除第一种设备类型对应的设备列表 
*******************************************************************************/
static unsigned char ExtCTLFun_ClearALLList(  struct Up1FrameStruct *FrameTemp ) //扩展的列表清除指令
{
    unsigned char DataBuff[3];

    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) )
    { 
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            StringCopy(&FrameTemp->FrameBuffer[11],DataBuff,3);
            DataBuff[2]++;
            CJ188_CommendPackSend( FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                         FrameTemp->FrameBuffer[9]|BIT7,3,DataBuff );
            return ClearDeviceList(0);//删除第一个设备类型对应的设备列表   
        }
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:static unsigned char ExtCTLFun_SeareDevcie(  struct Up1FrameStruct *FrameTemp ) //扩展的列表清除指令
  功能:	扩展的控制指令解码
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtCTLFun_SeareDevcie(  struct Up1FrameStruct *FrameTemp )
{
    unsigned char DataBuff[4];
    
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) )//判断ID号是否相符
    {    
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            StringCopy(&FrameTemp->FrameBuffer[11],DataBuff,3);
            DataBuff[2]++;
            DataBuff[3] = SeareDevcie_TaskRequ(FrameTemp->FrameBuffer[14],&FrameTemp->FrameBuffer[15],
                                     FrameTemp->FrameBuffer[22]);
            return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                         FrameTemp->FrameBuffer[9]|BIT7,4,DataBuff);
        }
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtCTLFun_ReadMeasureData(  struct Up1FrameStruct *FrameTemp ) //扩展的列表清除指令
  功能:	扩展的控制指令解码
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtCTLFun_ReadMeasureData(  struct Up1FrameStruct *FrameTemp ) //扩展的
{
    unsigned char DataBuff[4];
    
    if( IsDeviceIDEqu( &FrameTemp->FrameBuffer[2] ) )
    { 
        if( FrameTemp->FrameBuffer[9] & BIT6 )
        {   //通讯不正常
            return 0;
        }
        else
        {   //通讯正常
            StringCopy(&FrameTemp->FrameBuffer[11],DataBuff,3);
            DataBuff[2]++; 
            DataBuff[3] = ReadMeasuData_TaskRequ(0x10);
            return CJ188_CommendPackSend(FrameTemp->FrameBuffer[1],&FrameTemp->FrameBuffer[2],
                                         FrameTemp->FrameBuffer[9]|BIT7,4,DataBuff);
        }
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:static unsigned char ExtCTLDataType_Decode( struct Up1FrameStruct *FrameTemp )
  功能:	扩展的控制指令解码
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 
*******************************************************************************/
static unsigned char ExtCTLDataType_Decode( struct Up1FrameStruct *FrameTemp )
{
    unsigned int DataType = ((unsigned int)FrameTemp->FrameBuffer[11])<<8 | ((unsigned int)FrameTemp->FrameBuffer[12]);
    switch( DataType )
    {
      case 0X5555:
      return ExtCTLFun_MBUSON( FrameTemp );           //扩展的总线打开指令 
      
      case 0XAAAA:
      return ExtCTLFun_MBUSOFF( FrameTemp );          //扩展的总线关闭指令   
      
      case 0X0030:
      return ExtCTLFun_ClearALLList( FrameTemp );     //扩展的列表清除指令
      
      case 0X0040:
      return ExtCTLFun_SeareDevcie( FrameTemp );      //扩展的启动搜索表号任务指令 
      
      case 0X0050:
      return ExtCTLFun_ReadMeasureData( FrameTemp );  //扩展的启动计量数据任务指令 
      default:
        return 0;
    }  
}

/******************************************************************************
  函数(模块)名称:static unsigned char ControlCode_Decode(struct Up1FrameStruct *FrameTemp )
  功能:	188控制码解析函数
  输入参数: 
  输出参数: 成功或失败         	 		   		 
  其它说明: 解析通讯方向位(主机或从机)，以及命令厂商自定义位
            再解析各功能位
            对于上行，采集器处在从机状态，因此解码只针对主机发出的数据.
*******************************************************************************/
static unsigned char ControlCode_Decode(struct Up1FrameStruct *FrameTemp )
{
    if( FrameTemp->FrameBuffer[9] & BIT7 )
    {   //从机发出的帧数据
        if( FrameTemp->FrameBuffer[9] & BIT5 )
        {   //厂商自定义命令
            return 0;
        }
        else
        {  //协议标准命令
            return 0;
        }  
    }
    else
    {//主机发出的帧数据  接收上行主机的命令
        if( FrameTemp->FrameBuffer[9] & BIT5 )
        {   //厂商自定义命令
           switch(FrameTemp->FrameBuffer[9] & 0X3F)
            {
                case 0X21:        //扩展读数据
                  return ExtReadDataType_Decode( FrameTemp );//数据类型解码
                case 0X24:        //扩展写数据
                  return ExtWriteDataType_Decode( FrameTemp );
                case 0X25:        //扩展控制指令
                  return ExtCTLDataType_Decode( FrameTemp );
                  
                default:
                  return 0;
            }
        }
        else
        {  //协议标准命令
            switch(FrameTemp->FrameBuffer[9] & 0X1F)
            {
          
                //case 0X00:      //保留功能
                  //break;
                case 0X01:        //读数据
                  return ReadDataType_Decode( FrameTemp );//读数据类型解析函数
                  
                case 0X03:        //读地址(表号)
                return ReadFun_DeviceAddr( FrameTemp );   //读地址功能函数 广播
                
                case 0X04:        //写数据
                  return WriteDataType_Decode( FrameTemp );
                  
                
                //case 0X09:        //读密钥版本号
                //break;
                
                case 0X15:         //写地址(表号)
                return WriteFun_DeviceAddr( FrameTemp );  //写地址功能函数 广播
                
                //case 0X16:        //写机电同步数(置表底数)
                //break;
                default:
                  return 0;
            }
        }
    }
}

/******************************************************************************
  函数(模块)名称:void CJ188_Decode(struct FrameStruct *FrameTemp )
  功能:CJ188协议解码函数	
  输入参数:帧数据（结构体）         	              		
  输出参数: 无         	 		   		 
  其它说明:按设备类型、控制码、数据标识顺序进行解码
*******************************************************************************/
static unsigned char CJ188_Decode(struct Up1FrameStruct *FrameTemp )
{
    if( FrameTemp->FrameBuffer[FrameTemp->FrameBuffer[10]+11] == SumCheck(FrameTemp->FrameBuffer,FrameTemp->FrameBuffer[10]+11) )
    {
        switch(FrameTemp->FrameBuffer[1])
        {
        case 0x02: //采集器类型
          return ControlCode_Decode( FrameTemp );    //控制码解码
          
        default:
          return 0;
        }
    }
    return 0;
}

/******************************************************************************
  函数(模块)名称:void CJ188_ReceivProcess(void)
  功能:188协议接收处理任务函数
  输入参数:无         	              		
  输出参数:无         	 		   		 
  其它说明:调用后，会查询一次接收缓冲区，若有帧标志位或缓冲区满置起，则启动处理程序
*******************************************************************************/
unsigned char Up1CJ188_ReceivProcess(struct Up1FrameStruct *FrameTemp)
{
    if( USCIA1_ReadRXBuffFrame() || (USCIA1_ReadRXBuffStatus()&BIT1) )
    {
        if(GetFrame(FrameTemp))   //从缓冲区中检出一帧数据
        {
            //USCIA1_FrameDec();
            if( FrameCheckSum( FrameTemp) )       //检查帧校验
            {
                return CJ188_Decode( FrameTemp ); //帧数据解码
            }
            else return 0;//帧数据校验错
        }
        else return 0;//没有完整的帧数据
    }
    else return 0;//没有帧数据
}

