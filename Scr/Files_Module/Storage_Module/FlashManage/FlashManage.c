//************************************************************************
//
//                     FlashManage 
//            
//************************************************************************
//************************************************************************
//  1、提供
//                    
//            
//************************************************************************
#include "stdlib.h"
#include "Extern_FlashDrive.H"        //FLASH驱动
#include "FlashManage.H"              //FLASH管理层

struct FlashManage_InfoStruct FlashManage_Info = {0,0,0,0,0,0};



/******************************************************************************
  函数(模块)名称:unsigned char CheckBufferData(unsigned char *Buffer,unsigned char Lent,
                              unsigned char ChcekData)
  功能:检查指定缓冲区是否全为指定数据
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char CheckBufferData(unsigned char *Buffer,unsigned char Lent,
                              unsigned char ChcekData)
{

    while( Lent-- )
    {        
        if( ChcekData != *Buffer++ )
        {
            return 0;
        }
    }
    return 1;
}


/******************************************************************************
  函数(模块)名称:void BufferCopy(unsigned char *Buffer0,unsigned char *Buffer1,unsigned int CopyLent)
  功能:	把数据从Buffer0复制到Buffer1去。
  输入参数:源缓冲区指针，目的缓冲区指针，长度         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void BufferCopy(unsigned char *Buffer0,unsigned char *Buffer1,unsigned int CopyLent)
{
    while(CopyLent--)
    {
        *Buffer1++ = *Buffer0++;
    }
}


/******************************************************************************
  函数(模块)名称:unsigned long GetSmalSectAddr(unsigned int PhySectorNum)
  功能:	根据指定的小扇区号，计算小扇区号实际的物理起始地址
  输入参数:物理小扇区号         	              		
  输出参数:long型的物理地址          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned long GetSmalSectAddr(unsigned int PhySectorNum)
{
    return (unsigned long)(PhyFlashStart + (unsigned long)PhySectorNum * (unsigned long)SmalPhySectSize + ((unsigned long)PhySectorNum/(unsigned long)SmalSectPerBig) * (unsigned long)UnusedSizePerBig);  //计算小扇区物理起始地址
}

/******************************************************************************
  函数(模块)名称:unsigned char ReadPhysicSectInfo(unsigned int PhySectorNum,
                                 struct PhysicsSectorInfo *PhysicSectorInfo)
  功能:	扇区基本操作函数　　读取物理小扇区头信息
  输入参数:物理小扇区号，小扇区头信息结构体指针         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadPhysicSectInfo(unsigned int PhySectorNum,
                                 struct PhysicsSectorInfo *PhysicSectorInfo)
{
    FlashDrive_MultiByteRead( GetSmalSectAddr(PhySectorNum),                //小扇区物理起始地址
                              (unsigned char *)PhysicSectorInfo,//小扇区信息缓冲区
                              sizeof(*PhysicSectorInfo)         //小扇区信息区大小
                            );
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char PhySectorEn(unsigned int PhySectorNum)
  功能:	扇区基本操作函数　　指定小扇区无效
  输入参数:物理小扇区号         	              		
  输出参数:          	 		   		 
  其它说明:指定无效后，垃圾回收机制可以回收这个扇区(小扇区)
*******************************************************************************/
unsigned char PhysicSectorValid( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof(struct PhysicsSectorInfo,SectorValid),0);
}

/******************************************************************************
  函数(模块)名称:unsigned char PhysicSectorEn( unsigned int PhySectorNum )
  功能:	扇区基本操作函数　　启用指定物理小扇区
  输入参数:物理小扇区号         	              		
  输出参数:          	 		   		 
  其它说明:启用后，可以与指定的逻辑扇区建立映射。不一定需要写入数据
*******************************************************************************/
unsigned char PhysicSectorEn( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorEn ),0);
}


/******************************************************************************
  函数(模块)名称:unsigned char PhysicSectorDataCS( unsigned int PhySectorNum,unsigned char SectorDataCS )
  功能:	扇区基本操作函数　　向指定物理小扇区写入数据校验
  输入参数:物理小扇区号，校验字节         	              		
  输出参数:          	 		   		 
  其它说明:写入之前，确认扇区的校验字节是FF
*******************************************************************************/
unsigned char PhysicSectorDataCS( unsigned int PhySectorNum,unsigned char SectorDataCS )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorDataCS ),SectorDataCS );
}

/******************************************************************************
  函数(模块)名称:unsigned char PhysicSectorDataStatus( unsigned int PhySectorNum )
  功能:	扇区基本操作函数　　向指定物理小扇区写数据区状态
  输入参数:物理小扇区号         	              		
  输出参数:          	 		   		 
  其它说明:在对物理小扇区写入数据后，把数据状态位清0，表示该物理扇区已经被写入过数据
*******************************************************************************/
unsigned char PhysicSectorDataStatus( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorDataStatus ),0 );    
}


/******************************************************************************
  函数(模块)名称:unsigned char PhysicLogicSectorNum( unsigned int PhySectorNum,unsigned int LogicSectorNum )
  功能:	扇区基本操作函数　　向指定物理小扇区写逻辑映射扇区号
  输入参数:物理小扇区号，逻辑小扇区号         	              		
  输出参数:          	 		   		 
  其它说明:向物理小扇区写入与之映射关系的逻辑小扇区号
*******************************************************************************/
unsigned char PhysicLogicSectorNum( unsigned int PhySectorNum,unsigned int LogicSectorNum )
{
    FlashDrive_MultiByteWrite( GetSmalSectAddr(PhySectorNum)+offsetof( struct PhysicsSectorInfo,LogicSectorNum ),(unsigned char *)&LogicSectorNum,sizeof(LogicSectorNum));
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char PhysicSectorWritComple( unsigned int PhySectorNum )
  功能:	扇区基本操作函数　　物理扇区信息写完成。
  输入参数:物理小扇区号         	              		
  输出参数:          	 		   		 
  其它说明:在写完映射的逻辑扇区号后，可以清0这位，表示映射信息建立。
*******************************************************************************/
unsigned char PhysicSectorWritComple( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorWritComple ),0 );    
}

/******************************************************************************
  函数(模块)名称:unsigned char PhysicSectorWritComple( unsigned int PhySectorNum )
  功能:	扇区基本操作函数　　新旧映射关系替换成功。
  输入参数:物理小扇区号         	              		
  输出参数:          	 		   		 
  其它说明:表示成功用新扇区替换旧扇区。若一直为FF，则表示当前是第一次建立的扇区。
*******************************************************************************/
unsigned char PhysicSectorUpDate( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorUpDate ),0 );
}

/******************************************************************************
  函数(模块)名称:unsigned int GetCurretnWritePhySect(void)
  功能:	返回当前写的物理扇区号。
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:。
*******************************************************************************/
unsigned int GetCurretnWritePhySect(void)
{
    return (FlashManage_Info.CurrentWritePhySect++)%BigPhySectMax;
}


/******************************************************************************
  函数(模块)名称:unsigned int GetCurretnReadPhySect(void)
  功能:	返回当前读的扇区号。
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned int GetCurretnReadPhySect(void)
{
    return (FlashManage_Info.CurrentReadPhySect++)%BigPhySectMax;
}


/******************************************************************************
  函数(模块)名称:unsigned int GetCurretnWritePhySect(void)
  功能:	返回当前写的物理扇区号。
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:。
*******************************************************************************/
unsigned int GetCurrentTrashCheck(void)
{
    return (FlashManage_Info.CurrentTrashCheck++)%(PhyFlashSize/BigPhySectSize);
}

/******************************************************************************
  函数(模块)名称:unsigned char SearEmptySect(unsigned int Offset,unsigned int *EmptyPhySectNum)
  功能:	查找一个空的物理小扇区
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 从指定物理小扇区开始，依次搜索，直到找到一个空的扇区。
*******************************************************************************/
unsigned char SearEmptySect(unsigned int Offset,unsigned int *EmptyPhySectNum)
{
    struct PhysicsSectorInfo PhysicSectorInfo;
    unsigned char *SectDataBuff;
    unsigned int i=Offset;
    do{
        ReadPhysicSectInfo(i,&PhysicSectorInfo);
        if( (0xFF == PhysicSectorInfo.SectorEn) &&  //扇区末启用
            (0xFF == PhysicSectorInfo.SectorValid)  //扇区有效
          )
        {
            SectDataBuff = (unsigned char *)malloc( SmalPhySectSize ); 
            if( SectDataBuff!=NULL )
            {
                FlashDrive_MultiByteRead( GetSmalSectAddr(i),SectDataBuff,SmalPhySectSize );//
                if( CheckBufferData( SectDataBuff,SmalPhySectSize,0xff) )
                {        
                    free(SectDataBuff);  
                    *EmptyPhySectNum = i;
                    return 1;
                }
                else
                {
                    free(SectDataBuff);
                }
            }
            else//申请失败，则直接返回
            {
               *EmptyPhySectNum = i;
                return 1; 
            }
        }
        i = (i+1)%BigPhySectMax;
    }while((i!=Offset));
    return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char CheckLogicSectInPhy(unsigned int PhySectNum,unsigned int LogicSectorNum)
  功能:	检查指定物理小扇区内的映射扇区号是否与指定的相同。
  输入参数:物理小扇区号，逻辑扇区号         	              		
  输出参数:          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char CheckLogicSectInPhy(unsigned int PhySectNum,unsigned int LogicSectorNum)
{
    struct PhysicsSectorInfo PhysicSectorInfo;
    ReadPhysicSectInfo(PhySectNum,&PhysicSectorInfo);
    if( LogicSectorNum == PhysicSectorInfo.LogicSectorNum)//物理扇区的映射号等于要查找的逻辑扇区号
    {
        if(  (0x00 == PhysicSectorInfo.SectorEn) &&        //并且扇区已经启用
             (0xFF == PhysicSectorInfo.SectorValid) &&     //扇区有效
             (0x00 == PhysicSectorInfo.SectorWritComple) ) //有效的映射扇区号
        {
             return 1;
        }
        else return 0;
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char SearchLogicMappingSect( unsigned int LogicSectorNum,unsigned int CurrenPhySect,unsigned int *PhySectNum )
  功能:	指定位置搜索指定逻辑扇区的映射物理扇区号
  输入参数:逻辑扇区号，指定搜索起始物理扇区号，物理小扇区号指针         	              		
  输出参数:          	 		   		 
  其它说明: 如果查找出错或是逻辑扇区号还末建立映射，则返回0
*******************************************************************************/
unsigned char SearchLogicMappingSect( unsigned int LogicSectorNum,unsigned int CurrenPhySect,unsigned int *PhySectNum )
{
    unsigned int i=CurrenPhySect;                   //;(CurrenPhySect+1)%
    do{
        if( CheckLogicSectInPhy( i,LogicSectorNum) )
        {
            *PhySectNum = i;
            FlashManage_Info.CurrentReadPhySect = i;
            return 1;
        }
        i = (i+1)%BigPhySectMax;
    }while((i!=CurrenPhySect));
    return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char GetPhySector( unsigned int LogicSectorNum,unsigned int *PhySectNum )
  功能:	根据指定的逻辑扇区号找到映射的物理扇区号
  输入参数:逻辑扇区号，物理小扇区号指针         	              		
  输出参数:          	 		   		 
  其它说明: 如果查找出错或是逻辑扇区号还末建立映射，则返回0
*******************************************************************************/
unsigned char GetPhySector( unsigned int LogicSectorNum,unsigned int *PhySectNum )
{
    /*if( SearchMappingBuffer( LogicSectorNum,PhySectNum ) )
    {
        return 1;
    }
    else
    {*/
        return SearchLogicMappingSect( LogicSectorNum,GetCurretnReadPhySect(),PhySectNum );
    //}
}


/******************************************************************************
  函数(模块)名称:unsigned char DumMapSector(unsigned int LogicSectNum,unsigned int *PhySectNum )
  功能:	建立空映射，并返回被映射到的物理小扇区号
  输入参数:逻辑扇区号         	              		
  输出参数:被映射的物理小扇区号          	 		   		 
  其它说明: 把指定的逻辑扇区号，映射到一个空的物理小扇区中去。
            仅把逻辑扇区与物理扇区建立映射，此时数据为全FF。
            同时更新扇区使用情况数据
*******************************************************************************/
unsigned char DumMapSector(unsigned int LogicSectNum,unsigned int *PhySectNum )
{
    unsigned int EmptyPhySectNum;
    if( SearEmptySect( GetCurretnWritePhySect(),&EmptyPhySectNum) )
    {
        if( PhysicSectorEn( EmptyPhySectNum ) )                       //启用这个扇区
        {
            FlashManage_Info.ValidSectorNum--;
            if( PhysicLogicSectorNum( EmptyPhySectNum,LogicSectNum ) )//写逻辑扇区号
            {
                if( PhysicSectorWritComple( EmptyPhySectNum ) )
                {
                    if( PhysicSectorUpDate( EmptyPhySectNum ) )
                    {
                        FlashManage_Info.UsedSectorNum++;
                        *PhySectNum = EmptyPhySectNum;
                        return 1;  //映射成功
                    }
                    else 
                    {
                        FlashManage_Info.ErroSectorNum++;
                        return 0;//映射失败
                    }
                }
                else 
                {
                    FlashManage_Info.ErroSectorNum++;
                    return 0;//映射失败
                }
            }
            else 
            {
                FlashManage_Info.ErroSectorNum++;
                return 0;//映射失败
            }
        }
        else 
        {
            FlashManage_Info.ErroSectorNum++;
            return 0;//映射失败
        }
    }
    else return 0;//映射失败
}


/******************************************************************************
  函数(模块)名称:unsigned char ScanBiyPhySector(unsigned int BigPhySector,
                               struct FlashManage_InfoStruct *FlashTemp_Info )
  功能:	扇区基本操作函数　　扫描大扇区内的小扇区信息。
  输入参数:物理大扇区号，扇区信息结构体         	              		
  输出参数:          	 		   		 
  其它说明:表示成功用新扇区替换旧扇区。若一直为FF，则表示当前是第一次建立的扇区。
*******************************************************************************/
unsigned char ScanBiyPhySector(unsigned int BigPhySector,
                               struct FlashManage_InfoStruct *FlashTemp_Info )
{
  
    unsigned int SectorStartAddr;
    unsigned int OldPhySector;
    struct PhysicsSectorInfo PhysicSectorInfo;
    
    SectorStartAddr = BigPhySector*SmalSectPerBig;
    for( unsigned int j=0;j<SmalSectPerBig;j++ )        //物理小扇区
    {
        ReadPhysicSectInfo( j+SectorStartAddr,&PhysicSectorInfo ); //读出扇区信息
        if( 0xFF == PhysicSectorInfo.SectorValid )//判断扇区有效字节
        {   //有效扇区
            if( 0xFF == PhysicSectorInfo.SectorEn )//判断扇区启用字节
            {   //末启用扇区
                if( CheckBufferData( (unsigned char *)&PhysicSectorInfo,
                                    sizeof(PhysicSectorInfo),0xFF) )              //末启用的有效扇区，理论全为FF
                {
                    FlashTemp_Info->ValidSectorNum++;
                }
                else
                {
                    PhysicSectorValid( j+SectorStartAddr );
                    FlashTemp_Info->DirtySectorNum++;
                }
            }
            else
            {   //已用扇区
                if( 0x00 == PhysicSectorInfo.SectorWritComple )//判断映射号
                {   
                    if(  0x00 == PhysicSectorInfo.SectorUpDate )//判断扇区更新有效
                    {   //正常扇区
                        FlashTemp_Info->UsedSectorNum++;
                    }
                    else
                    {
                        if( SearchLogicMappingSect( PhysicSectorInfo.LogicSectorNum,(j+SectorStartAddr),&OldPhySector ) )
                        {
                            PhysicSectorValid( OldPhySector );
                            FlashTemp_Info->DirtySectorNum++;
                        }
                        FlashTemp_Info->UsedSectorNum++;
                        PhysicSectorUpDate( j+SectorStartAddr );
                    }
                }
                else
                {   //映射号不正常，证明没有完成映射过程
                    PhysicSectorValid( j+SectorStartAddr );
                    FlashTemp_Info->DirtySectorNum++;
                }
            }
        }
        else
        {   //废弃扇区
            FlashTemp_Info->DirtySectorNum++;
        } 
    }
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char GetMaxDirtyBigSector(struct PhyBigSectorInfo *BigSectorInfo)
  功能:	找出无效扇区最多的大扇区。
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:
*******************************************************************************/
unsigned char GetDirtyBigSector(struct PhyBigSectorInfo *BigSectorInfo)
{
    struct PhyBigSectorInfo BigSectorInfoTemp = {0,{0,0,0} };
    unsigned char SearchFlag = 0;

    unsigned int i = GetCurrentTrashCheck();
    unsigned int Offset = i;
    do{
        ScanBiyPhySector( i,&BigSectorInfoTemp.SmalPhySectoInfo );
        if( BigSectorInfoTemp.SmalPhySectoInfo.DirtySectorNum 
           >  BigSectorInfo->SmalPhySectoInfo.DirtySectorNum )
        {
            BigSectorInfoTemp.PhyBigSectNum = i;
            *BigSectorInfo = BigSectorInfoTemp;
            SearchFlag = 1;
            if( BigSectorInfoTemp.SmalPhySectoInfo.DirtySectorNum > (SmalSectPerBig/2) )
              return 1;
        }
        BigSectorInfoTemp.SmalPhySectoInfo.ValidSectorNum = 0;
        BigSectorInfoTemp.SmalPhySectoInfo.UsedSectorNum = 0;
        BigSectorInfoTemp.SmalPhySectoInfo.DirtySectorNum = 0;
        BigSectorInfoTemp.SmalPhySectoInfo.ErroSectorNum = 0;
        i = (i+1)%(PhyFlashSize/BigPhySectSize);
    }while((i!=Offset));
    return SearchFlag;
}

/******************************************************************************
  函数(模块)名称:unsigned char ScanFlashInfo(void)
  功能:	Flash情况扫描，得到使用情况。
  输入参数:无         	              		
  输出参数:无          	 		   		 
  其它说明:
*******************************************************************************/
/*unsigned char ScanFlashInfo(void)
{
    FlashManage_Info.ValidSectorNum = 0;
    FlashManage_Info.UsedSectorNum = 0;
    FlashManage_Info.DirtySectorNum = 0;
    
    for(unsigned int i=0;i<(PhyFlashSize/BigPhySectSize);i++)
    {
        ScanBiyPhySector( i,&FlashManage_Info );
    }
    return 1; 
}
*/




/******************************************************************************
  函数(模块)名称:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  功能:	读取物理小扇区数据区
  输入参数:物理小扇区号,数据区偏移地址，读取长度，小扇区数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
  unsigned long PhySectorStarAddr;                   //物理小扇区起始地址
  if( (OffsetAddr<SmalPhySectDataSize) && (DataLetn<=(SmalPhySectDataSize-OffsetAddr)) )
  {
      PhySectorStarAddr = GetSmalSectAddr(PhySectorNum) + SmalPhySectDataOffset + OffsetAddr; //计算小物理扇区数据区地址
      FlashDrive_MultiByteRead(
                                PhySectorStarAddr,      //数据区物理起始地址
                                DataBuffer,             //小扇区数据缓冲区
                                DataLetn                //小扇区数据区大小
                              );
      return 1;
  }
  else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  功能:	读取物理小扇区数据区
  输入参数:物理小扇区号,数据区偏移地址，读取长度，小扇区数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WritePhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
  unsigned long PhySectorStarAddr;                   //物理小扇区起始地址
  if( (OffsetAddr<SmalPhySectDataSize) && (DataLetn<=(SmalPhySectDataSize-OffsetAddr)) )
  {
      PhySectorStarAddr = GetSmalSectAddr(PhySectorNum) + SmalPhySectDataOffset + OffsetAddr; //计算小物理扇区数据区地址
      FlashDrive_MultiByteWrite(
                                PhySectorStarAddr,      //数据区物理起始地址
                                DataBuffer,             //数据缓冲区
                                DataLetn                //数据大小
                              );
      return 1;
  }
  else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char CheckSectorFF( unsigned int PhySectorNum )
  功能:检查指定物理小扇区是否全为FF
  输入参数:需要检查的小扇区号         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char CheckSectorFF( unsigned int PhySectorNum )
{
    unsigned long PhySectorStarAddr;//物理小扇区起始地址
    unsigned char *SectDataBuff;
    
    PhySectorStarAddr = GetSmalSectAddr(PhySectorNum) + SmalPhySectDataOffset; //计算小物理扇区数据区地址
    SectDataBuff = (unsigned char *)malloc( SmalPhySectDataSize ); 
    if( SectDataBuff!=NULL )
    {
        FlashDrive_MultiByteRead(PhySectorStarAddr,SectDataBuff,SmalPhySectDataSize);//
        if( CheckBufferData( SectDataBuff,SmalPhySectDataSize,0xff) )
        {        
            free(SectDataBuff);  
            return 1;
        }
        free(SectDataBuff);
    }
    return 0; 
}


/******************************************************************************
  函数(模块)名称:unsigned int GetLogicSectNum(unsigned long ReadAddr)
  功能:根据线性地址取得逻辑扇区号
  输入参数:线性地址         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned int GetLogicSectNum(unsigned long ReadAddr)
{
    return (unsigned int)( (ReadAddr-FlashStartAddre)/(unsigned long)LogicSectSize );
}

/******************************************************************************
  函数(模块)名称:unsigned int GetLogicSectOffset(unsigned long ReadAddr)
  功能:根据线性地址取得逻辑扇区偏移地址
  输入参数:线性地址         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned int GetLogicSectOffset(unsigned long ReadAddr)
{
    return (unsigned int)( (ReadAddr-FlashStartAddre)%(unsigned long)LogicSectSize );
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  功能:	读取物理小扇区数据区
  输入参数:物理小扇区号,数据区偏移地址，读取长度，输出数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明:若当前读的逻辑小扇区还末被映射，则查找新的物理小扇区进行映射后再读 
*******************************************************************************/
unsigned char ReadLogicSectData( unsigned int LogicSectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
    unsigned int PhySectNum;
    if( (OffsetAddr<LogicSectSize) &&             //偏移地址小于逻辑扇区最大地址
        (DataLetn<=(LogicSectSize-OffsetAddr)) && //数据长度小于偏移后的地址长度
        (LogicSectorNum<LogicSectMax) )           //扇区号小于整个地址长度的最大扇区号
    {
        if( GetPhySector( LogicSectorNum,&PhySectNum ) )  //根据逻辑扇区号查找实际的物理小扇区号
        {
           return ReadPhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer ); //从实际的物理小扇区号读回所要求数据
        }
        else
        {
          if( DumMapSector(LogicSectorNum,&PhySectNum ) )  //把逻辑扇区号映射到一个实际的物理小扇区
          {
              return ReadPhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer );//映射成功，并根据返回的物理扇区号，返回数据
          }
          else return 0;//无法映射
        }
    }
    else return 0;//参数出错
}



/******************************************************************************
  函数(模块)名称:unsigned char WriteLogicDataToPhy(unsigned int LogicSectorNum,unsigned int PhySectNum,
                                  unsigned char *SectDataBuff)
  功能:	写逻辑扇区数据到指定物理小扇区
  输入参数:逻辑扇区号，物理扇区号,数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 写入的数据大小固定为一整扇区大小
*******************************************************************************/
unsigned char WriteLogicDataToPhy(unsigned int LogicSectorNum,unsigned int PhySectNum,
                                  unsigned char *SectDataBuff)
{
    unsigned int Flag = 1;
    if( PhysicSectorEn( PhySectNum ) )//新扇区启用标志
    {
        FlashManage_Info.ValidSectorNum--;          
        if( WritePhysicSectData( PhySectNum,0,LogicSectSize,SectDataBuff ) )//写数据
        {
            if( PhysicSectorDataStatus( PhySectNum ) ) //写数据完成标志
            {
                if( PhysicLogicSectorNum( PhySectNum,LogicSectorNum ) )//写逻辑扇区号
                {
                    if( PhysicSectorWritComple( PhySectNum ) )
                    {
                       Flag = 1;
                    }
                    else Flag = 0;
                }
                else Flag = 0; //写映射扇区号失败
            }
            else Flag = 0;//写数据状态位失败
        }
        else Flag = 0; //写数据失败 
    }
    else Flag = 0;//新扇区启用失败
    if( Flag ==0 )
      FlashManage_Info.ErroSectorNum++;
    return Flag;
}


/******************************************************************************
  函数(模块)名称:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  功能:	读取物理小扇区数据区
  输入参数:物理小扇区号,数据区偏移地址，读取长度，小扇区数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char WriteLogicSectData( unsigned int LogicSectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
    if( (OffsetAddr<LogicSectSize) &&             //偏移地址小于逻辑扇区最大地址
        (DataLetn<=(LogicSectSize-OffsetAddr)) && //数据长度小于偏移后的地址长度
        (LogicSectorNum<LogicSectMax) )           //扇区号小于整个地址长度的最大扇区号
    {
        unsigned int PhySectNum;
        //if( SearchLogicMappingSect( LogicSectorNum,GetCurretnReadPhySect(),&PhySectNum ) )  //根据逻辑扇区号查找实际的物理小扇区号
        if( GetPhySector( LogicSectorNum,&PhySectNum ) )  //根据逻辑扇区号查找实际的物理小扇区号
        { 
            struct PhysicsSectorInfo PhysicSectorInfo;
            ReadPhysicSectInfo(PhySectNum,&PhysicSectorInfo);
            
            if((0xFF == PhysicSectorInfo.SectorDataStatus) &&  //确保数据区干净可写
               CheckSectorFF( PhySectNum ) )
            {
                if( WritePhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer ) )//写数据
                {
                    return PhysicSectorDataStatus( PhySectNum );                      //写数据完成标志
                }
                else return 0;//写数据失败
            }
            else
            {
                unsigned char *SectDataBuff;
                unsigned int NewPhySectNum;
                SectDataBuff = (unsigned char *)malloc( LogicSectSize );          //申请帧数据空间
                if( SectDataBuff != NULL )
                {
                    if( ReadLogicSectData( LogicSectorNum,0,LogicSectSize,SectDataBuff ) )//读出旧扇区数据
                    {
                        BufferCopy(DataBuffer,SectDataBuff+OffsetAddr,DataLetn);//要写的数据追加进去
                        if( SearEmptySect( GetCurretnWritePhySect(), &NewPhySectNum ) )    //查找新扇区
                        {                        
                            if( WriteLogicDataToPhy( LogicSectorNum,NewPhySectNum,SectDataBuff) )
                            {
                                free(SectDataBuff);//释放申请的空间
                                if( PhysicSectorValid( PhySectNum ) )            //旧扇区无效
                                {
                                    FlashManage_Info.DirtySectorNum++;
                                    return PhysicSectorUpDate( NewPhySectNum );//新扇区启用
                                }
                                else return 0;//删除旧扇区失败
                            }
                            else                           
                            {
                                free(SectDataBuff);//释放申请的空间
                                return 0;          //写新扇区失败
                            } 
                        }
                        else  //查找新扇区失败
                        {
                            free(SectDataBuff);//释放申请的空间
                            return 0;
                        } 
                    }
                    else
                    {
                        free(SectDataBuff);//释放申请的空间
                        return 0;//读旧扇区数据失败
                    } 
                }
                else return 0;//缓冲区申请失败
            }
        }
        else
        {   //为写扇区建立映射，再写数据
            if( DumMapSector(LogicSectorNum,&PhySectNum ) )  //把逻辑扇区号映射到一个实际的物理小扇区
            {
                struct PhysicsSectorInfo PhysicSectorInfo;
                ReadPhysicSectInfo(PhySectNum,&PhysicSectorInfo);
                
                if((0xFF == PhysicSectorInfo.SectorDataStatus) &&  //确保数据区干净可写
                   CheckSectorFF( PhySectNum ) )
                {
                    if( WritePhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer ) )//写数据
                    {
                        return PhysicSectorDataStatus( PhySectNum );
                    }
                    else return 0;//写数据失败                  
                }
                else return 0;//数据区非空
            }
            else return 0;//映射失败
        }
    }
    else return 0;//参数出错
}

/******************************************************************************
  函数(模块)名称:unsigned char RubishRecycSector(unsigned int BigPhySectorNum)
  功能:回收整理指定的大物理扇区
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:把扇区内所有有效扇区移到整理扇区之外，再擦除该扇区 
*******************************************************************************/
unsigned char RubishRecycSector(unsigned int BigPhySectorNum)
{
    unsigned int SmalSectorAddr;
    unsigned int EmptyPhySectNum;
    struct PhysicsSectorInfo PhysicSectorInfo;
  
    SmalSectorAddr = BigPhySectorNum*SmalSectPerBig;
    for( unsigned int j=0;j<SmalSectPerBig;j++ )        //物理小扇区
    {
        ReadPhysicSectInfo( j+SmalSectorAddr,&PhysicSectorInfo ); //读出扇区信息
        if( (0x00 == PhysicSectorInfo.SectorEn) &&         //扇区已经启用
             (0xFF == PhysicSectorInfo.SectorValid) &&     //扇区有效
             (0x00 == PhysicSectorInfo.SectorWritComple) ) //有效的映射扇区号
        {   //要移动的扇区
            if( SearEmptySect( (((j+SmalSectorAddr)/SmalSectPerBig)+1)*SmalSectPerBig,&EmptyPhySectNum ) )//查找新扇区
            {
                if( (EmptyPhySectNum/SmalSectPerBig) != ((j+SmalSectorAddr)/SmalSectPerBig) )
                {
                    unsigned char *SectDataBuff;
                    SectDataBuff = (unsigned char *)malloc( LogicSectSize );          //申请帧数据空间
                    if( SectDataBuff != NULL )
                    {
                        if( ReadLogicSectData( PhysicSectorInfo.LogicSectorNum,0,LogicSectSize,SectDataBuff ) )//读出旧扇区数据
                        { 
                            if( WriteLogicDataToPhy( PhysicSectorInfo.LogicSectorNum,EmptyPhySectNum,SectDataBuff) )
                            {
                                free(SectDataBuff);//释放申请的空间
                                if( PhysicSectorValid( j+SmalSectorAddr ) )            //旧扇区无效
                                {
                                    FlashManage_Info.DirtySectorNum++;
                                    if( PhysicSectorUpDate( EmptyPhySectNum ) == 0 )      //新扇区启用
                                      return 0;
                                }
                                else return 0;//删除旧扇区失败
                            }
                            else                           
                            {
                                free(SectDataBuff);//释放申请的空间
                                return 0;//写新扇区失败
                            } 
                        }
                        else return 0;
                    }
                    else return 0;
                }
                else return 0;//找到的新扇区与要移动的旧扇区在同一大扇区内
            }
            else return 0;//找不到新扇区
        }
    }
    FlashDrive_SectorErase( BigPhySectorNum );
    FlashManage_Info.DirtySectorNum -= SmalSectPerBig;
    FlashManage_Info.ValidSectorNum += SmalSectPerBig;
    //printf("\nReback IS ok!");
    return 1;
}


/******************************************************************************
  函数(模块)名称:void TrashReclai(void)
  功能:根据扇区使用情况，以启动垃圾回收机制
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char TrashReclai(void)
{
    struct PhyBigSectorInfo BigSectorInfo = {0,{0,0,0} };
    if( (FlashManage_Info.ValidSectorNum < 128) 
       || (FlashManage_Info.DirtySectorNum > 128) )
    {
        if( GetDirtyBigSector( &BigSectorInfo ) )
        {
            RubishRecycSector(BigSectorInfo.PhyBigSectNum);
            //printf("\nReback IS Ok!");
            return 1;
        }
        else 
        {
          //printf("\nReback IS Fail!");
          return 0;
        }
        
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char CheckFlash(void)
  功能:
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:检查FLASH 
*******************************************************************************/
unsigned char CheckFlash(void)
{
    struct FlashManage_InfoStruct FlashManage_InfoTemp;
    
    FlashManage_Info.ValidSectorNum = 0;
    FlashManage_Info.UsedSectorNum = 0;
    FlashManage_Info.DirtySectorNum = 0;
    FlashManage_InfoTemp = FlashManage_Info;
    
    for(unsigned int i=0;i<(PhyFlashSize/BigPhySectSize);i++)      //物理大扇区
    { 
        ScanBiyPhySector( i,&FlashManage_InfoTemp );               //扫描当前大扇区空间情况
        if( FlashManage_InfoTemp.DirtySectorNum == SmalSectPerBig )//当前大扇区全部为无效扇区，可回收
        {
            FlashDrive_SectorErase( i );
            FlashManage_InfoTemp.DirtySectorNum -= SmalSectPerBig;
            FlashManage_InfoTemp.ValidSectorNum += SmalSectPerBig;
        }
        FlashManage_Info.ValidSectorNum += FlashManage_InfoTemp.ValidSectorNum;//扇区分配信息累加
        FlashManage_Info.UsedSectorNum  += FlashManage_InfoTemp.UsedSectorNum;
        FlashManage_Info.DirtySectorNum += FlashManage_InfoTemp.DirtySectorNum;
        
        FlashManage_InfoTemp.ValidSectorNum = 0;//临时变量清0
        FlashManage_InfoTemp.UsedSectorNum  = 0;
        FlashManage_InfoTemp.DirtySectorNum = 0;       
    }
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char FlashManage_Init(void)
  功能:FLASH管理层初始化
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明:先初始化底层驱动，再检查整个FLASH，得到扇区情况以及恢复错误扇区 
*******************************************************************************/
unsigned char FlashManage_Init(void)
{
    FlashDrive_Init();      //FLASH底层驱动初始化 
    //FlashDrive_ChipErase();
    CheckFlash();           //检查一次FLASH区，以查得扇区使用情况，并恢复错误扇区
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,
                                              unsigned char *DataBuffer)
  功能:逻辑读，线性模式
  输入参数:线性读地址，读取长度，数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,
                             unsigned char *DataBuffer)
{
    unsigned int CurreLogiSectNum;
    unsigned int CurreLogiSectOffset;
    unsigned int CurreLogiSectReadLent;
    while(DataLent)
    {
        CurreLogiSectNum = GetLogicSectNum( ReadAddr );      //取得当前逻辑扇区号
        CurreLogiSectOffset = GetLogicSectOffset( ReadAddr );//取得扇区偏移值
        if( (LogicSectSize-CurreLogiSectOffset) > DataLent )
            CurreLogiSectReadLent = DataLent;
        else
            CurreLogiSectReadLent = (LogicSectSize-CurreLogiSectOffset);
        if( ReadLogicSectData( CurreLogiSectNum,CurreLogiSectOffset,CurreLogiSectReadLent,DataBuffer ) )
        {
            ReadAddr = ReadAddr + CurreLogiSectReadLent;
            DataLent = DataLent - CurreLogiSectReadLent;
            DataBuffer = DataBuffer + CurreLogiSectReadLent;
        }
        else return 0;
    }
    return 1;
}


/******************************************************************************
  函数(模块)名称:unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,
                                               unsigned char *DataBuffer)
  功能:逻辑写，线性模式
  输入参数:线性写地址，长度，数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 把数据顺序写入FLASH存储区域，可能会启动垃圾回收机制
*******************************************************************************/
unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,
                              unsigned char *DataBuffer)
{
    unsigned int CurreLogiSectNum;
    unsigned int CurreLogiSectOffset;
    unsigned int CurreLogiSectWriteLent;
    
    TrashReclai();//垃圾回收机制
    while( DataLent )
    {
        CurreLogiSectNum = GetLogicSectNum( WriteAddr );      //取得当前逻辑扇区号
        CurreLogiSectOffset = GetLogicSectOffset( WriteAddr );//取得扇区偏移值
        if( (LogicSectSize-CurreLogiSectOffset) > DataLent )
            CurreLogiSectWriteLent = DataLent;
        else
            CurreLogiSectWriteLent = (LogicSectSize-CurreLogiSectOffset);
        if( WriteLogicSectData( CurreLogiSectNum,CurreLogiSectOffset,CurreLogiSectWriteLent,DataBuffer ) )
        {
            WriteAddr = WriteAddr + CurreLogiSectWriteLent;
            DataLent = DataLent - CurreLogiSectWriteLent;
            DataBuffer = DataBuffer + CurreLogiSectWriteLent;
        }
        else return 0;
    }
    return 1;
}

