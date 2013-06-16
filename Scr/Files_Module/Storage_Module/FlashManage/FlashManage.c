//************************************************************************
//
//                     FlashManage 
//            
//************************************************************************
//************************************************************************
//  1���ṩ
//                    
//            
//************************************************************************
#include "stdlib.h"
#include "Extern_FlashDrive.H"        //FLASH����
#include "FlashManage.H"              //FLASH�����

struct FlashManage_InfoStruct FlashManage_Info = {0,0,0,0,0,0};



/******************************************************************************
  ����(ģ��)����:unsigned char CheckBufferData(unsigned char *Buffer,unsigned char Lent,
                              unsigned char ChcekData)
  ����:���ָ���������Ƿ�ȫΪָ������
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
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
  ����(ģ��)����:void BufferCopy(unsigned char *Buffer0,unsigned char *Buffer1,unsigned int CopyLent)
  ����:	�����ݴ�Buffer0���Ƶ�Buffer1ȥ��
  �������:Դ������ָ�룬Ŀ�Ļ�����ָ�룬����         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
void BufferCopy(unsigned char *Buffer0,unsigned char *Buffer1,unsigned int CopyLent)
{
    while(CopyLent--)
    {
        *Buffer1++ = *Buffer0++;
    }
}


/******************************************************************************
  ����(ģ��)����:unsigned long GetSmalSectAddr(unsigned int PhySectorNum)
  ����:	����ָ����С�����ţ�����С������ʵ�ʵ�������ʼ��ַ
  �������:����С������         	              		
  �������:long�͵������ַ          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned long GetSmalSectAddr(unsigned int PhySectorNum)
{
    return (unsigned long)(PhyFlashStart + (unsigned long)PhySectorNum * (unsigned long)SmalPhySectSize + ((unsigned long)PhySectorNum/(unsigned long)SmalSectPerBig) * (unsigned long)UnusedSizePerBig);  //����С����������ʼ��ַ
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadPhysicSectInfo(unsigned int PhySectorNum,
                                 struct PhysicsSectorInfo *PhysicSectorInfo)
  ����:	����������������������ȡ����С����ͷ��Ϣ
  �������:����С�����ţ�С����ͷ��Ϣ�ṹ��ָ��         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadPhysicSectInfo(unsigned int PhySectorNum,
                                 struct PhysicsSectorInfo *PhysicSectorInfo)
{
    FlashDrive_MultiByteRead( GetSmalSectAddr(PhySectorNum),                //С����������ʼ��ַ
                              (unsigned char *)PhysicSectorInfo,//С������Ϣ������
                              sizeof(*PhysicSectorInfo)         //С������Ϣ����С
                            );
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char PhySectorEn(unsigned int PhySectorNum)
  ����:	��������������������ָ��С������Ч
  �������:����С������         	              		
  �������:          	 		   		 
  ����˵��:ָ����Ч���������ջ��ƿ��Ի����������(С����)
*******************************************************************************/
unsigned char PhysicSectorValid( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof(struct PhysicsSectorInfo,SectorValid),0);
}

/******************************************************************************
  ����(ģ��)����:unsigned char PhysicSectorEn( unsigned int PhySectorNum )
  ����:	������������������������ָ������С����
  �������:����С������         	              		
  �������:          	 		   		 
  ����˵��:���ú󣬿�����ָ�����߼���������ӳ�䡣��һ����Ҫд������
*******************************************************************************/
unsigned char PhysicSectorEn( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorEn ),0);
}


/******************************************************************************
  ����(ģ��)����:unsigned char PhysicSectorDataCS( unsigned int PhySectorNum,unsigned char SectorDataCS )
  ����:	����������������������ָ������С����д������У��
  �������:����С�����ţ�У���ֽ�         	              		
  �������:          	 		   		 
  ����˵��:д��֮ǰ��ȷ��������У���ֽ���FF
*******************************************************************************/
unsigned char PhysicSectorDataCS( unsigned int PhySectorNum,unsigned char SectorDataCS )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorDataCS ),SectorDataCS );
}

/******************************************************************************
  ����(ģ��)����:unsigned char PhysicSectorDataStatus( unsigned int PhySectorNum )
  ����:	����������������������ָ������С����д������״̬
  �������:����С������         	              		
  �������:          	 		   		 
  ����˵��:�ڶ�����С����д�����ݺ󣬰�����״̬λ��0����ʾ�����������Ѿ���д�������
*******************************************************************************/
unsigned char PhysicSectorDataStatus( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorDataStatus ),0 );    
}


/******************************************************************************
  ����(ģ��)����:unsigned char PhysicLogicSectorNum( unsigned int PhySectorNum,unsigned int LogicSectorNum )
  ����:	����������������������ָ������С����д�߼�ӳ��������
  �������:����С�����ţ��߼�С������         	              		
  �������:          	 		   		 
  ����˵��:������С����д����֮ӳ���ϵ���߼�С������
*******************************************************************************/
unsigned char PhysicLogicSectorNum( unsigned int PhySectorNum,unsigned int LogicSectorNum )
{
    FlashDrive_MultiByteWrite( GetSmalSectAddr(PhySectorNum)+offsetof( struct PhysicsSectorInfo,LogicSectorNum ),(unsigned char *)&LogicSectorNum,sizeof(LogicSectorNum));
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char PhysicSectorWritComple( unsigned int PhySectorNum )
  ����:	������������������������������Ϣд��ɡ�
  �������:����С������         	              		
  �������:          	 		   		 
  ����˵��:��д��ӳ����߼������ź󣬿�����0��λ����ʾӳ����Ϣ������
*******************************************************************************/
unsigned char PhysicSectorWritComple( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorWritComple ),0 );    
}

/******************************************************************************
  ����(ģ��)����:unsigned char PhysicSectorWritComple( unsigned int PhySectorNum )
  ����:	���������������������¾�ӳ���ϵ�滻�ɹ���
  �������:����С������         	              		
  �������:          	 		   		 
  ����˵��:��ʾ�ɹ����������滻����������һֱΪFF�����ʾ��ǰ�ǵ�һ�ν�����������
*******************************************************************************/
unsigned char PhysicSectorUpDate( unsigned int PhySectorNum )
{
    return FlashDrive_WriteByte( GetSmalSectAddr(PhySectorNum) + offsetof( struct PhysicsSectorInfo,SectorUpDate ),0 );
}

/******************************************************************************
  ����(ģ��)����:unsigned int GetCurretnWritePhySect(void)
  ����:	���ص�ǰд�����������š�
  �������:         	              		
  �������:          	 		   		 
  ����˵��:��
*******************************************************************************/
unsigned int GetCurretnWritePhySect(void)
{
    return (FlashManage_Info.CurrentWritePhySect++)%BigPhySectMax;
}


/******************************************************************************
  ����(ģ��)����:unsigned int GetCurretnReadPhySect(void)
  ����:	���ص�ǰ���������š�
  �������:         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned int GetCurretnReadPhySect(void)
{
    return (FlashManage_Info.CurrentReadPhySect++)%BigPhySectMax;
}


/******************************************************************************
  ����(ģ��)����:unsigned int GetCurretnWritePhySect(void)
  ����:	���ص�ǰд�����������š�
  �������:         	              		
  �������:          	 		   		 
  ����˵��:��
*******************************************************************************/
unsigned int GetCurrentTrashCheck(void)
{
    return (FlashManage_Info.CurrentTrashCheck++)%(PhyFlashSize/BigPhySectSize);
}

/******************************************************************************
  ����(ģ��)����:unsigned char SearEmptySect(unsigned int Offset,unsigned int *EmptyPhySectNum)
  ����:	����һ���յ�����С����
  �������:         	              		
  �������:          	 		   		 
  ����˵��: ��ָ������С������ʼ������������ֱ���ҵ�һ���յ�������
*******************************************************************************/
unsigned char SearEmptySect(unsigned int Offset,unsigned int *EmptyPhySectNum)
{
    struct PhysicsSectorInfo PhysicSectorInfo;
    unsigned char *SectDataBuff;
    unsigned int i=Offset;
    do{
        ReadPhysicSectInfo(i,&PhysicSectorInfo);
        if( (0xFF == PhysicSectorInfo.SectorEn) &&  //����ĩ����
            (0xFF == PhysicSectorInfo.SectorValid)  //������Ч
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
            else//����ʧ�ܣ���ֱ�ӷ���
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
  ����(ģ��)����:unsigned char CheckLogicSectInPhy(unsigned int PhySectNum,unsigned int LogicSectorNum)
  ����:	���ָ������С�����ڵ�ӳ���������Ƿ���ָ������ͬ��
  �������:����С�����ţ��߼�������         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char CheckLogicSectInPhy(unsigned int PhySectNum,unsigned int LogicSectorNum)
{
    struct PhysicsSectorInfo PhysicSectorInfo;
    ReadPhysicSectInfo(PhySectNum,&PhysicSectorInfo);
    if( LogicSectorNum == PhysicSectorInfo.LogicSectorNum)//����������ӳ��ŵ���Ҫ���ҵ��߼�������
    {
        if(  (0x00 == PhysicSectorInfo.SectorEn) &&        //���������Ѿ�����
             (0xFF == PhysicSectorInfo.SectorValid) &&     //������Ч
             (0x00 == PhysicSectorInfo.SectorWritComple) ) //��Ч��ӳ��������
        {
             return 1;
        }
        else return 0;
    }
    else return 0;
}

/******************************************************************************
  ����(ģ��)����:unsigned char SearchLogicMappingSect( unsigned int LogicSectorNum,unsigned int CurrenPhySect,unsigned int *PhySectNum )
  ����:	ָ��λ������ָ���߼�������ӳ������������
  �������:�߼������ţ�ָ��������ʼ���������ţ�����С������ָ��         	              		
  �������:          	 		   		 
  ����˵��: ������ҳ�������߼������Ż�ĩ����ӳ�䣬�򷵻�0
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
  ����(ģ��)����:unsigned char GetPhySector( unsigned int LogicSectorNum,unsigned int *PhySectNum )
  ����:	����ָ�����߼��������ҵ�ӳ�������������
  �������:�߼������ţ�����С������ָ��         	              		
  �������:          	 		   		 
  ����˵��: ������ҳ�������߼������Ż�ĩ����ӳ�䣬�򷵻�0
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
  ����(ģ��)����:unsigned char DumMapSector(unsigned int LogicSectNum,unsigned int *PhySectNum )
  ����:	������ӳ�䣬�����ر�ӳ�䵽������С������
  �������:�߼�������         	              		
  �������:��ӳ�������С������          	 		   		 
  ����˵��: ��ָ�����߼������ţ�ӳ�䵽һ���յ�����С������ȥ��
            �����߼�������������������ӳ�䣬��ʱ����ΪȫFF��
            ͬʱ��������ʹ���������
*******************************************************************************/
unsigned char DumMapSector(unsigned int LogicSectNum,unsigned int *PhySectNum )
{
    unsigned int EmptyPhySectNum;
    if( SearEmptySect( GetCurretnWritePhySect(),&EmptyPhySectNum) )
    {
        if( PhysicSectorEn( EmptyPhySectNum ) )                       //�����������
        {
            FlashManage_Info.ValidSectorNum--;
            if( PhysicLogicSectorNum( EmptyPhySectNum,LogicSectNum ) )//д�߼�������
            {
                if( PhysicSectorWritComple( EmptyPhySectNum ) )
                {
                    if( PhysicSectorUpDate( EmptyPhySectNum ) )
                    {
                        FlashManage_Info.UsedSectorNum++;
                        *PhySectNum = EmptyPhySectNum;
                        return 1;  //ӳ��ɹ�
                    }
                    else 
                    {
                        FlashManage_Info.ErroSectorNum++;
                        return 0;//ӳ��ʧ��
                    }
                }
                else 
                {
                    FlashManage_Info.ErroSectorNum++;
                    return 0;//ӳ��ʧ��
                }
            }
            else 
            {
                FlashManage_Info.ErroSectorNum++;
                return 0;//ӳ��ʧ��
            }
        }
        else 
        {
            FlashManage_Info.ErroSectorNum++;
            return 0;//ӳ��ʧ��
        }
    }
    else return 0;//ӳ��ʧ��
}


/******************************************************************************
  ����(ģ��)����:unsigned char ScanBiyPhySector(unsigned int BigPhySector,
                               struct FlashManage_InfoStruct *FlashTemp_Info )
  ����:	��������������������ɨ��������ڵ�С������Ϣ��
  �������:����������ţ�������Ϣ�ṹ��         	              		
  �������:          	 		   		 
  ����˵��:��ʾ�ɹ����������滻����������һֱΪFF�����ʾ��ǰ�ǵ�һ�ν�����������
*******************************************************************************/
unsigned char ScanBiyPhySector(unsigned int BigPhySector,
                               struct FlashManage_InfoStruct *FlashTemp_Info )
{
  
    unsigned int SectorStartAddr;
    unsigned int OldPhySector;
    struct PhysicsSectorInfo PhysicSectorInfo;
    
    SectorStartAddr = BigPhySector*SmalSectPerBig;
    for( unsigned int j=0;j<SmalSectPerBig;j++ )        //����С����
    {
        ReadPhysicSectInfo( j+SectorStartAddr,&PhysicSectorInfo ); //����������Ϣ
        if( 0xFF == PhysicSectorInfo.SectorValid )//�ж�������Ч�ֽ�
        {   //��Ч����
            if( 0xFF == PhysicSectorInfo.SectorEn )//�ж����������ֽ�
            {   //ĩ��������
                if( CheckBufferData( (unsigned char *)&PhysicSectorInfo,
                                    sizeof(PhysicSectorInfo),0xFF) )              //ĩ���õ���Ч����������ȫΪFF
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
            {   //��������
                if( 0x00 == PhysicSectorInfo.SectorWritComple )//�ж�ӳ���
                {   
                    if(  0x00 == PhysicSectorInfo.SectorUpDate )//�ж�����������Ч
                    {   //��������
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
                {   //ӳ��Ų�������֤��û�����ӳ�����
                    PhysicSectorValid( j+SectorStartAddr );
                    FlashTemp_Info->DirtySectorNum++;
                }
            }
        }
        else
        {   //��������
            FlashTemp_Info->DirtySectorNum++;
        } 
    }
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char GetMaxDirtyBigSector(struct PhyBigSectorInfo *BigSectorInfo)
  ����:	�ҳ���Ч�������Ĵ�������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:
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
  ����(ģ��)����:unsigned char ScanFlashInfo(void)
  ����:	Flash���ɨ�裬�õ�ʹ�������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:
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
  ����(ģ��)����:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  ����:	��ȡ����С����������
  �������:����С������,������ƫ�Ƶ�ַ����ȡ���ȣ�С�������ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
  unsigned long PhySectorStarAddr;                   //����С������ʼ��ַ
  if( (OffsetAddr<SmalPhySectDataSize) && (DataLetn<=(SmalPhySectDataSize-OffsetAddr)) )
  {
      PhySectorStarAddr = GetSmalSectAddr(PhySectorNum) + SmalPhySectDataOffset + OffsetAddr; //����С����������������ַ
      FlashDrive_MultiByteRead(
                                PhySectorStarAddr,      //������������ʼ��ַ
                                DataBuffer,             //С�������ݻ�����
                                DataLetn                //С������������С
                              );
      return 1;
  }
  else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  ����:	��ȡ����С����������
  �������:����С������,������ƫ�Ƶ�ַ����ȡ���ȣ�С�������ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WritePhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
  unsigned long PhySectorStarAddr;                   //����С������ʼ��ַ
  if( (OffsetAddr<SmalPhySectDataSize) && (DataLetn<=(SmalPhySectDataSize-OffsetAddr)) )
  {
      PhySectorStarAddr = GetSmalSectAddr(PhySectorNum) + SmalPhySectDataOffset + OffsetAddr; //����С����������������ַ
      FlashDrive_MultiByteWrite(
                                PhySectorStarAddr,      //������������ʼ��ַ
                                DataBuffer,             //���ݻ�����
                                DataLetn                //���ݴ�С
                              );
      return 1;
  }
  else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char CheckSectorFF( unsigned int PhySectorNum )
  ����:���ָ������С�����Ƿ�ȫΪFF
  �������:��Ҫ����С������         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char CheckSectorFF( unsigned int PhySectorNum )
{
    unsigned long PhySectorStarAddr;//����С������ʼ��ַ
    unsigned char *SectDataBuff;
    
    PhySectorStarAddr = GetSmalSectAddr(PhySectorNum) + SmalPhySectDataOffset; //����С����������������ַ
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
  ����(ģ��)����:unsigned int GetLogicSectNum(unsigned long ReadAddr)
  ����:�������Ե�ַȡ���߼�������
  �������:���Ե�ַ         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned int GetLogicSectNum(unsigned long ReadAddr)
{
    return (unsigned int)( (ReadAddr-FlashStartAddre)/(unsigned long)LogicSectSize );
}

/******************************************************************************
  ����(ģ��)����:unsigned int GetLogicSectOffset(unsigned long ReadAddr)
  ����:�������Ե�ַȡ���߼�����ƫ�Ƶ�ַ
  �������:���Ե�ַ         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned int GetLogicSectOffset(unsigned long ReadAddr)
{
    return (unsigned int)( (ReadAddr-FlashStartAddre)%(unsigned long)LogicSectSize );
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  ����:	��ȡ����С����������
  �������:����С������,������ƫ�Ƶ�ַ����ȡ���ȣ�������ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��:����ǰ�����߼�С������ĩ��ӳ�䣬������µ�����С��������ӳ����ٶ� 
*******************************************************************************/
unsigned char ReadLogicSectData( unsigned int LogicSectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
    unsigned int PhySectNum;
    if( (OffsetAddr<LogicSectSize) &&             //ƫ�Ƶ�ַС���߼���������ַ
        (DataLetn<=(LogicSectSize-OffsetAddr)) && //���ݳ���С��ƫ�ƺ�ĵ�ַ����
        (LogicSectorNum<LogicSectMax) )           //������С��������ַ���ȵ����������
    {
        if( GetPhySector( LogicSectorNum,&PhySectNum ) )  //�����߼������Ų���ʵ�ʵ�����С������
        {
           return ReadPhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer ); //��ʵ�ʵ�����С�����Ŷ�����Ҫ������
        }
        else
        {
          if( DumMapSector(LogicSectorNum,&PhySectNum ) )  //���߼�������ӳ�䵽һ��ʵ�ʵ�����С����
          {
              return ReadPhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer );//ӳ��ɹ��������ݷ��ص����������ţ���������
          }
          else return 0;//�޷�ӳ��
        }
    }
    else return 0;//��������
}



/******************************************************************************
  ����(ģ��)����:unsigned char WriteLogicDataToPhy(unsigned int LogicSectorNum,unsigned int PhySectNum,
                                  unsigned char *SectDataBuff)
  ����:	д�߼��������ݵ�ָ������С����
  �������:�߼������ţ�����������,���ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: д������ݴ�С�̶�Ϊһ��������С
*******************************************************************************/
unsigned char WriteLogicDataToPhy(unsigned int LogicSectorNum,unsigned int PhySectNum,
                                  unsigned char *SectDataBuff)
{
    unsigned int Flag = 1;
    if( PhysicSectorEn( PhySectNum ) )//���������ñ�־
    {
        FlashManage_Info.ValidSectorNum--;          
        if( WritePhysicSectData( PhySectNum,0,LogicSectSize,SectDataBuff ) )//д����
        {
            if( PhysicSectorDataStatus( PhySectNum ) ) //д������ɱ�־
            {
                if( PhysicLogicSectorNum( PhySectNum,LogicSectorNum ) )//д�߼�������
                {
                    if( PhysicSectorWritComple( PhySectNum ) )
                    {
                       Flag = 1;
                    }
                    else Flag = 0;
                }
                else Flag = 0; //дӳ��������ʧ��
            }
            else Flag = 0;//д����״̬λʧ��
        }
        else Flag = 0; //д����ʧ�� 
    }
    else Flag = 0;//����������ʧ��
    if( Flag ==0 )
      FlashManage_Info.ErroSectorNum++;
    return Flag;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadPhysicSectData( unsigned int PhySectorNum,unsigned int OffsetAddr,
                                                   unsigned int DataLetn,unsigned char *DataBuffer )
  ����:	��ȡ����С����������
  �������:����С������,������ƫ�Ƶ�ַ����ȡ���ȣ�С�������ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char WriteLogicSectData( unsigned int LogicSectorNum,unsigned int OffsetAddr,
                                 unsigned int DataLetn,unsigned char *DataBuffer )
{
    if( (OffsetAddr<LogicSectSize) &&             //ƫ�Ƶ�ַС���߼���������ַ
        (DataLetn<=(LogicSectSize-OffsetAddr)) && //���ݳ���С��ƫ�ƺ�ĵ�ַ����
        (LogicSectorNum<LogicSectMax) )           //������С��������ַ���ȵ����������
    {
        unsigned int PhySectNum;
        //if( SearchLogicMappingSect( LogicSectorNum,GetCurretnReadPhySect(),&PhySectNum ) )  //�����߼������Ų���ʵ�ʵ�����С������
        if( GetPhySector( LogicSectorNum,&PhySectNum ) )  //�����߼������Ų���ʵ�ʵ�����С������
        { 
            struct PhysicsSectorInfo PhysicSectorInfo;
            ReadPhysicSectInfo(PhySectNum,&PhysicSectorInfo);
            
            if((0xFF == PhysicSectorInfo.SectorDataStatus) &&  //ȷ���������ɾ���д
               CheckSectorFF( PhySectNum ) )
            {
                if( WritePhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer ) )//д����
                {
                    return PhysicSectorDataStatus( PhySectNum );                      //д������ɱ�־
                }
                else return 0;//д����ʧ��
            }
            else
            {
                unsigned char *SectDataBuff;
                unsigned int NewPhySectNum;
                SectDataBuff = (unsigned char *)malloc( LogicSectSize );          //����֡���ݿռ�
                if( SectDataBuff != NULL )
                {
                    if( ReadLogicSectData( LogicSectorNum,0,LogicSectSize,SectDataBuff ) )//��������������
                    {
                        BufferCopy(DataBuffer,SectDataBuff+OffsetAddr,DataLetn);//Ҫд������׷�ӽ�ȥ
                        if( SearEmptySect( GetCurretnWritePhySect(), &NewPhySectNum ) )    //����������
                        {                        
                            if( WriteLogicDataToPhy( LogicSectorNum,NewPhySectNum,SectDataBuff) )
                            {
                                free(SectDataBuff);//�ͷ�����Ŀռ�
                                if( PhysicSectorValid( PhySectNum ) )            //��������Ч
                                {
                                    FlashManage_Info.DirtySectorNum++;
                                    return PhysicSectorUpDate( NewPhySectNum );//����������
                                }
                                else return 0;//ɾ��������ʧ��
                            }
                            else                           
                            {
                                free(SectDataBuff);//�ͷ�����Ŀռ�
                                return 0;          //д������ʧ��
                            } 
                        }
                        else  //����������ʧ��
                        {
                            free(SectDataBuff);//�ͷ�����Ŀռ�
                            return 0;
                        } 
                    }
                    else
                    {
                        free(SectDataBuff);//�ͷ�����Ŀռ�
                        return 0;//������������ʧ��
                    } 
                }
                else return 0;//����������ʧ��
            }
        }
        else
        {   //Ϊд��������ӳ�䣬��д����
            if( DumMapSector(LogicSectorNum,&PhySectNum ) )  //���߼�������ӳ�䵽һ��ʵ�ʵ�����С����
            {
                struct PhysicsSectorInfo PhysicSectorInfo;
                ReadPhysicSectInfo(PhySectNum,&PhysicSectorInfo);
                
                if((0xFF == PhysicSectorInfo.SectorDataStatus) &&  //ȷ���������ɾ���д
                   CheckSectorFF( PhySectNum ) )
                {
                    if( WritePhysicSectData( PhySectNum,OffsetAddr,DataLetn,DataBuffer ) )//д����
                    {
                        return PhysicSectorDataStatus( PhySectNum );
                    }
                    else return 0;//д����ʧ��                  
                }
                else return 0;//�������ǿ�
            }
            else return 0;//ӳ��ʧ��
        }
    }
    else return 0;//��������
}

/******************************************************************************
  ����(ģ��)����:unsigned char RubishRecycSector(unsigned int BigPhySectorNum)
  ����:��������ָ���Ĵ���������
  �������:         	              		
  �������:          	 		   		 
  ����˵��:��������������Ч�����Ƶ���������֮�⣬�ٲ��������� 
*******************************************************************************/
unsigned char RubishRecycSector(unsigned int BigPhySectorNum)
{
    unsigned int SmalSectorAddr;
    unsigned int EmptyPhySectNum;
    struct PhysicsSectorInfo PhysicSectorInfo;
  
    SmalSectorAddr = BigPhySectorNum*SmalSectPerBig;
    for( unsigned int j=0;j<SmalSectPerBig;j++ )        //����С����
    {
        ReadPhysicSectInfo( j+SmalSectorAddr,&PhysicSectorInfo ); //����������Ϣ
        if( (0x00 == PhysicSectorInfo.SectorEn) &&         //�����Ѿ�����
             (0xFF == PhysicSectorInfo.SectorValid) &&     //������Ч
             (0x00 == PhysicSectorInfo.SectorWritComple) ) //��Ч��ӳ��������
        {   //Ҫ�ƶ�������
            if( SearEmptySect( (((j+SmalSectorAddr)/SmalSectPerBig)+1)*SmalSectPerBig,&EmptyPhySectNum ) )//����������
            {
                if( (EmptyPhySectNum/SmalSectPerBig) != ((j+SmalSectorAddr)/SmalSectPerBig) )
                {
                    unsigned char *SectDataBuff;
                    SectDataBuff = (unsigned char *)malloc( LogicSectSize );          //����֡���ݿռ�
                    if( SectDataBuff != NULL )
                    {
                        if( ReadLogicSectData( PhysicSectorInfo.LogicSectorNum,0,LogicSectSize,SectDataBuff ) )//��������������
                        { 
                            if( WriteLogicDataToPhy( PhysicSectorInfo.LogicSectorNum,EmptyPhySectNum,SectDataBuff) )
                            {
                                free(SectDataBuff);//�ͷ�����Ŀռ�
                                if( PhysicSectorValid( j+SmalSectorAddr ) )            //��������Ч
                                {
                                    FlashManage_Info.DirtySectorNum++;
                                    if( PhysicSectorUpDate( EmptyPhySectNum ) == 0 )      //����������
                                      return 0;
                                }
                                else return 0;//ɾ��������ʧ��
                            }
                            else                           
                            {
                                free(SectDataBuff);//�ͷ�����Ŀռ�
                                return 0;//д������ʧ��
                            } 
                        }
                        else return 0;
                    }
                    else return 0;
                }
                else return 0;//�ҵ�����������Ҫ�ƶ��ľ�������ͬһ��������
            }
            else return 0;//�Ҳ���������
        }
    }
    FlashDrive_SectorErase( BigPhySectorNum );
    FlashManage_Info.DirtySectorNum -= SmalSectPerBig;
    FlashManage_Info.ValidSectorNum += SmalSectPerBig;
    //printf("\nReback IS ok!");
    return 1;
}


/******************************************************************************
  ����(ģ��)����:void TrashReclai(void)
  ����:��������ʹ��������������������ջ���
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
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
  ����(ģ��)����:unsigned char CheckFlash(void)
  ����:
  �������:         	              		
  �������:          	 		   		 
  ����˵��:���FLASH 
*******************************************************************************/
unsigned char CheckFlash(void)
{
    struct FlashManage_InfoStruct FlashManage_InfoTemp;
    
    FlashManage_Info.ValidSectorNum = 0;
    FlashManage_Info.UsedSectorNum = 0;
    FlashManage_Info.DirtySectorNum = 0;
    FlashManage_InfoTemp = FlashManage_Info;
    
    for(unsigned int i=0;i<(PhyFlashSize/BigPhySectSize);i++)      //���������
    { 
        ScanBiyPhySector( i,&FlashManage_InfoTemp );               //ɨ�赱ǰ�������ռ����
        if( FlashManage_InfoTemp.DirtySectorNum == SmalSectPerBig )//��ǰ������ȫ��Ϊ��Ч�������ɻ���
        {
            FlashDrive_SectorErase( i );
            FlashManage_InfoTemp.DirtySectorNum -= SmalSectPerBig;
            FlashManage_InfoTemp.ValidSectorNum += SmalSectPerBig;
        }
        FlashManage_Info.ValidSectorNum += FlashManage_InfoTemp.ValidSectorNum;//����������Ϣ�ۼ�
        FlashManage_Info.UsedSectorNum  += FlashManage_InfoTemp.UsedSectorNum;
        FlashManage_Info.DirtySectorNum += FlashManage_InfoTemp.DirtySectorNum;
        
        FlashManage_InfoTemp.ValidSectorNum = 0;//��ʱ������0
        FlashManage_InfoTemp.UsedSectorNum  = 0;
        FlashManage_InfoTemp.DirtySectorNum = 0;       
    }
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char FlashManage_Init(void)
  ����:FLASH������ʼ��
  �������:         	              		
  �������:          	 		   		 
  ����˵��:�ȳ�ʼ���ײ��������ټ������FLASH���õ���������Լ��ָ��������� 
*******************************************************************************/
unsigned char FlashManage_Init(void)
{
    FlashDrive_Init();      //FLASH�ײ�������ʼ�� 
    //FlashDrive_ChipErase();
    CheckFlash();           //���һ��FLASH�����Բ������ʹ����������ָ���������
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,
                                              unsigned char *DataBuffer)
  ����:�߼���������ģʽ
  �������:���Զ���ַ����ȡ���ȣ����ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,
                             unsigned char *DataBuffer)
{
    unsigned int CurreLogiSectNum;
    unsigned int CurreLogiSectOffset;
    unsigned int CurreLogiSectReadLent;
    while(DataLent)
    {
        CurreLogiSectNum = GetLogicSectNum( ReadAddr );      //ȡ�õ�ǰ�߼�������
        CurreLogiSectOffset = GetLogicSectOffset( ReadAddr );//ȡ������ƫ��ֵ
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
  ����(ģ��)����:unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,
                                               unsigned char *DataBuffer)
  ����:�߼�д������ģʽ
  �������:����д��ַ�����ȣ����ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: ������˳��д��FLASH�洢���򣬿��ܻ������������ջ���
*******************************************************************************/
unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,
                              unsigned char *DataBuffer)
{
    unsigned int CurreLogiSectNum;
    unsigned int CurreLogiSectOffset;
    unsigned int CurreLogiSectWriteLent;
    
    TrashReclai();//�������ջ���
    while( DataLent )
    {
        CurreLogiSectNum = GetLogicSectNum( WriteAddr );      //ȡ�õ�ǰ�߼�������
        CurreLogiSectOffset = GetLogicSectOffset( WriteAddr );//ȡ������ƫ��ֵ
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

