  
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

#include "..\..\Sys_Module\Sys_Module.H"             //��Ƭ������ģ��
//#include "..\..\Storage_Module\Storage_Module.H"
//#include "..\..\Storage_Module\FileSys_Module.H"
#include "..\..\RTC_Module\RTC_Module.H"            //RTCģ��
#include ".\USCIA0_UART_Module\USCIA0_UART_Module.H"
#include "..\..\Application Layer\Application_Layer.H" //�ṩЭ���������Ҫ�ĸ�������ʵ�ִ���

#include "Down0CJ188_Module.h"
#include "DownChannel0_Module.H"

//************************************************************************ 
#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
//************************************************************************ 
//[2] ��Ҫ�޸ĵĲ����� 
#define CPU_F ((double)8000000) 


#define  ACKTimeNum     266    //֡��Ӧʱ��
#define  FrameReTryNum  3      //֡���������

//����ͨ��������Ϣ�ṹ��
struct DownChanne0_TaskStruct 
{
    unsigned char Status;     //����״̬�Ĵ��� BIT0�ܿ���
    unsigned char TaskID;     //����ţ�ÿ���������һ��ID��
};
struct DownChanne0_TaskStruct DownChanne0_TaskInfo;


//��������������ṹ��
struct ReadMeasureDataStruct
{
    unsigned char Status;                        //״̬�Ĵ���
    unsigned char CurrenStatus;                  //��ǰ״̬ 00,01,02
    unsigned char MeterType;                     //Ҫ����������
    unsigned char MeterListNum;                  //��ǰ�б����
    struct DeviceListStruct DeviceListItem;      //��Ŷ�Ӧ���豸�б���Ŀ
};
#define MeterReadTask_Bit    BIT0   //�������񿪹�
struct ReadMeasureDataStruct ReadMeasureDataInfo;      //�����б�


//�ӱ���������ṹ��
struct SearDeviceStruct
{
    unsigned char Status;                        //״̬�Ĵ���
    unsigned char CurrenStatus;                  //��ǰ״̬
    unsigned char DeviceType;                    //����������
    unsigned char CurrenDeviceStarID[7];         //��ǰ�б����
    unsigned char SeareLent;                     //��������
    unsigned char SeareCnt;                      //��������
};
struct SearDeviceStruct SeareDeviceInfo;//�����б���Ϣ


//�����ճ�������ṹ��
struct DateMeterReadStruct
{
    unsigned char Status;                        //״̬�Ĵ��� BIT0,�����ճ��������־λ
};
struct DateMeterReadStruct DateMeterReadInfo;    //�����б���Ϣ





/******************************************************************************
  ����(ģ��)����:unsigned char IsEqual(unsigned char *Buff1,unsigned char *Buff2,unsigned char Lent)
  ����:�ж������ֽڴ��Ƿ����	
  �������:�ַ���ָ��1���ַ���ָ��2������         	              		
  �������:          	 		   		 
  ����˵��: static 
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
  ����(ģ��)����:void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
  ����:�ַ�������	
  �������:�ַ���ָ��1���ַ���ָ��2������         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
static void StringCopy(unsigned char *SourceBuff,unsigned char *DstAddr,unsigned int Lent)
{
    while(Lent--)
      *DstAddr++ = *SourceBuff++;
}


/******************************************************************************
  ����(ģ��)����:static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
  ����:��һϵ���ֽڽ����ۼ����	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: �ֽڵ��ۼӺ�         	 		   		 
  ����˵��:�����ڲ�����static 
*******************************************************************************/
static unsigned char SumCheck(unsigned char *SumCheckBuff,unsigned int SumCheckLent)
{
    return Down0CJ188_SumCheck(SumCheckBuff,SumCheckLent);
}


/******************************************************************************
  ����(ģ��)����:unsigned char DownChanne0_ReadReciStatus(void)
  ����:	�����ջ�����״̬�Ĵ���
  �������:��         	              		
  �������: ������״̬�Ĵ���         	 		   		 
  ����˵��:
            Status�Ĵ�����λ�궨��
            BufferOV_BIT       BIT1  //����������־λ
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
  ����(ģ��)����:void DownChanne0_Init(void)
  ����:	����ͨ��ͨѶ���������ú���
  �������:��         	              		
  �������: ��        	 		   		 
  ����˵��:Ĭ��ʹ��2400�����ʣ�żУ�飬8λ����λ��1λֹͣλ
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
  ����(ģ��)����:unsigned char DownChanne0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
  ����:����ͨ�����ֽڷ��ͺ���	
  �������:�ֽڻ���ָ�룬����         	              		
  �������: ��         	 		   		 
  ����˵��:�� 
*******************************************************************************/
unsigned char DownChanne0_MultiByteSend(unsigned char *SendBuffer,unsigned int BufferLent)
{
    return USCIA0_MultiByteTX(SendBuffer,BufferLent);
}



/******************************************************************************
  ����(ģ��)����:void RecievProcess(struct Down0FrameStruct *FrameTemp)
  ����:	���������ݵ����ݴ���
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:
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
              
              //�洢����
              Storage_Module_Write(ReadMeasureDataInfo.DeviceListItem.DeviceDataPoint,DataBuffer,35,SPIFlashMod);
          }
      }

}



/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOn(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
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
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOFF(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
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
      USCIA0_FrameAckTime_TimeCntWrite(0);    //���³�ʼ���������
      USCIA0_FrameAckTime_ReTryCntWrite(0);
      
      DateMeterReadInfo.Status &= ~BIT0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  ����:	�������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:���ɹ�������������ͨ���������־λ���������³�ʼ�����������
           �����úö����������������Ϣ
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
  ����(ģ��)����:static unsigned char ReadDeviceListItem(void)
  ����:	��˳�����һ���б���Ϣ
  �������:         	              		
  �������:          	 		   		 
  ����˵��: �����豸���͡���Ŷ����豸�б�����Ӧ����Ŀ��������Ŀ��Ϣ���г�������
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
  ����(ģ��)����:unsigned char ReadMeasureData_CommeSend(void)
  ����:	������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:
*******************************************************************************/
unsigned char ReadMeasureData_CommeSend(void)
{
    unsigned char DataBuffer[3];
    DataBuffer[0] = 0x90;    //���ݱ�ʶ
    DataBuffer[1] = 0x1F;
    DataBuffer[2] = 0x00;    //SER
    return CJ188_CommendPackSend_USCIA0(ReadMeasureDataInfo.MeterType,ReadMeasureDataInfo.DeviceListItem.DeviceIDBuff,
                                 0X01,0X03,DataBuffer);
}


/******************************************************************************
  ����(ģ��)����:void ReadMeasureData_CJ188_ReceProce(struct Down0FrameStruct *FrameTemp)
  ����:	��������������Ľ���188����
  �������:֡�ṹ��ָ��         	              		
  �������:          	 		   		 
  ����˵��:������ͨ��������ȡ��һ֡���ݣ������н��룬���ж��Ƿ�Ϊ��Ӧ��֡����
           00��ʾ�����ݣ�01��ʾ���ݴ�,02��ʾ���䣬03��ʾ���ݶ�
*******************************************************************************/
unsigned char  CJ188_ReceProce(struct Down0FrameStruct *FrameTemp,unsigned char DeviceType,
                               unsigned char *DeviceIDBuffer)
{
    unsigned char ProcessFlag = 0;  //00��ʾ�����ݣ�01��ʾ���ݴ�,02��ʾ���䣬03��ʾ���ݶ�
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
        if( Down0CJ188_GetFrame( FrameTemp ) )           //�ӻ������м��һ֡����
        {
            if( Down0CJ188_FrameCheckSum(FrameTemp) ) //֡У��
            {
                if( FrameTemp->FrameBuffer[9] & BIT7 )
                {   //�ӻ�������֡����
                    MasterFrameBuff[9]|= BIT7;
                    MasterFrameBuff[10]= 0X16;
                    MasterFrameBuff[13]++;
                    if( IsEqual( FrameTemp->FrameBuffer,MasterFrameBuff,14 ) && (35 == FrameTemp->FrameLent) )
                    {
                        ProcessFlag = 3;//��ȷ������
                    }
                    else
                    {
                        ProcessFlag = 1;//��������
                    }
                }
                else
                {
                    //����������֡����,���ԣ��򿪼�ʱ�ӿ���
                    if( IsEqual( FrameTemp->FrameBuffer,MasterFrameBuff,14 ) && (16 ==FrameTemp->FrameLent) )
                    {
                        ProcessFlag = 2; //�Ƿ�������
                    }
                    else
                    {
                        ProcessFlag = 1; //��������
                    }
                }
            }
            else
            {
                //��֡���ݣ�����У����������γ�����������һ��
                ProcessFlag = 1;//��������
            }
        }
        else
        {
            //û��֡���ݻ����ݷǷ������γ�����������һ��
            ProcessFlag = 1;//��������
        }
    }
    else
    {
        ProcessFlag = 0;//û������
    }
    return ProcessFlag;    
}


/******************************************************************************
  ����(ģ��)����:void ReadMeasureData_Task(void)
  ����:	��������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:��3���׶�
*******************************************************************************/
void ReadMeasureData_Task(void)
{
    struct Down0FrameStruct FrameTemp;
    if( ReadMeasureDataInfo.Status & MeterReadTask_Bit )//����������
    {
        switch( ReadMeasureDataInfo.CurrenStatus )
        {
            case 0x00:  //��ʼ���׶�
              if( DateMeterReadInfo.Status & BIT0 )//
              {
                  MBUSOn();
                  delay_ms(4000);
              }
              USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);
              USCIA0_FrameAckTime_ReTryCntWrite(3);
              ReadMeasureDataInfo.CurrenStatus = 0X01; //ת������ͽ׶�
              
            case 0x01: //����ͽ׶�
              if( ReadMeasureData_DeviceListItem() )  //�Ӵӻ��б���ȡ��һ����Ч����Ŀ
              {
                  USCIA0_RXBuffClearn();
                  if( ReadMeasureData_CommeSend() )   //���ͳ�������
                  {
                      ReadMeasureDataInfo.CurrenStatus = 0X02;//���ͳɹ�����ת����һ���׶�
                      USCIA0_FrameAckTime_En(1); //��ʱ�����ܿ��ؿ���������֡������󣬻����ϴ򿪼�ʱ�����ӿ���
                  }
                  else
                  {
                      ReadMeasureDataInfo.CurrenStatus = 0X01;//���Ͳ��ɹ�����ת�ص���ǰ�׶�
                  }
              }
              else 
              {  //�ӻ��б����Ѿ�û����Ч�б����Ѿ���Ѱ���б�
                  if( DateMeterReadInfo.Status & BIT0 )//
                  {
                      MBUSOFF();
                  }                
                  ReadMeasuData_TaskOFF();//��������ֹͣ
              }
              break;
              
            case 0x02: //���մ����׶�
              if( USCIA0_FrameAckTime_ReTryCntRead() )     //��鳢�Դ���
              {
                  if( USCIA0_FrameAckTime_TimeCntRead() )  //����Ƿ�ʱ
                  {
                      //ReadMeasureData_CJ188_ReceProce( &FrameTemp ); //��������������Ľ��ս��봦��
                      switch( CJ188_ReceProce(&FrameTemp,ReadMeasureDataInfo.MeterType,
                                              ReadMeasureDataInfo.DeviceListItem.DeviceIDBuff ) )
                      {
                      case 0x00://û������
                        break;
                      case 0x01://��������
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum); //���¼���ʱ�����
                          USCIA0_FrameAckTime_ReTryCntDec();            //������1
                          ReadMeasureDataInfo.CurrenStatus = 0X01;          //ת���׶�,���³���
                        break;
                      case 0x02://���ݷ���
                          USCIA0_FrameAckTime_SubEn(1);//���¿�ʼ��ʱ
                        break;
                      case 0x03://������ȷ
                          RecievProcess(&FrameTemp);  //��������
                          
                          ReadMeasureDataInfo.MeterListNum++;          //ָ����һ����
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);    //���³�ʼ���������
                          USCIA0_FrameAckTime_ReTryCntWrite(3);
                          ReadMeasureDataInfo.CurrenStatus = 0X01;             //ת������׶�  
                        break;
                      default:
                        ReadMeasuData_TaskOFF();//������ǰ����
                        break;
                      }
                  }
                  else
                  {
                      //��ʱ����ת����һ��
                      USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum); //���¼���ʱ�����
                      USCIA0_FrameAckTime_ReTryCntDec();            //������1         
                      ReadMeasureDataInfo.CurrenStatus = 0X01;   //ת���׶�,���³���
                  }
              }
              else
              {
                  //�������������ת����һ����
                  ReadMeasureDataInfo.MeterListNum++;     //ָ����һ����
                  USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);//���³�ʼ���������
                  USCIA0_FrameAckTime_ReTryCntWrite(3);   
                  ReadMeasureDataInfo.CurrenStatus = 0X01;//ת������׶�
              }
              break;
              
            default:
              ReadMeasuData_TaskOFF();//���󣬳�������ֹͣ
              break;
        }
    }
}



/******************************************************************************
  ����(ģ��)����:unsigned char SeareCommeSend(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	��ָ�����ͼ����ŵı���������
  �������:��         	              		
  �������:          	 		   		 
  ����˵��:
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
  ����(ģ��)����:unsigned char SeareCommeSend(unsigned char DeviceType,unsigned char *DeviceIDBuff)
  ����:	��ָ�����ͼ����ŵı���������
  �������:��         	              		
  �������:          	 		   		 
  ����˵��:
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
  ����(ģ��)����:void DeviceIDInc(unsigned char *DeviceBuffer)
  ����:	���ŵ���1
  �������:��         	              		
  �������:          	 		   		 
  ����˵��:
*******************************************************************************/
void DeviceIDInc(unsigned char *DeviceBuffer)
{
    unsigned char LongCharBuff[8];
    StringCopy(DeviceBuffer,LongCharBuff,7);
    *( (unsigned long long *)(LongCharBuff) ) = __bcd_add_long_long( *( (unsigned long long *)(LongCharBuff) ),1);
    StringCopy(LongCharBuff,DeviceBuffer,7);
}


/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOn(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
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
  ����(ģ��)����:unsigned char ReadMeasuData_TaskOFF(void)
  ����:	������������������
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
*******************************************************************************/
void SeareDevcie_TaskOFF(void)
{
      DownChanne0_TaskInfo.Status &= ~BIT0;//���������־λ������ID��
      DownChanne0_TaskInfo.TaskID = 0XFF;
      
      SeareDeviceInfo.Status &= ~BIT0;
      SeareDeviceInfo.DeviceType = 0;
      SeareDeviceInfo.CurrenStatus = 0x00;  
      SeareDeviceInfo.SeareCnt = 0;
      
      USCIA0_FrameAckTime_En(0);
      USCIA0_FrameAckTime_SubEn(0);
      USCIA0_FrameAckTime_TimeCntWrite(0);    //���³�ʼ���������
      USCIA0_FrameAckTime_ReTryCntWrite(0);
}

/******************************************************************************
  ����(ģ��)����:unsigned char ReadMeasuData_TaskRequ(unsigned char DeviceType)
  ����:	
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:����ɹ����򷵻�1
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
  ����(ģ��)����:void SeareDevcie_Task(void)
  ����:	����Ѱ����
  �������:��         	              		
  �������:��          	 		   		 
  ����˵��:��3���׶�,��ڲ���Ϊ�����ͣ���ʼ�����Լ���������
           �����Ӧ�Ĵӱ���Ӧ����Ѷ�Ӧ���ż���ӱ��б���
           ������ӻ�����������֡�󣬻���������֡��Ӧ��ʱ
*******************************************************************************/
void SeareDevcie_Task(void)
{
    struct Down0FrameStruct FrameTemp;
    if( SeareDeviceInfo.Status & BIT0 )//��ѯ�ѱ������־λ
    {
        switch( SeareDeviceInfo.CurrenStatus )//��ѯ����״̬ 
        {
            case 0x00:  //��ʼ���׶�
              USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);//��Ӧʱ��
              USCIA0_FrameAckTime_ReTryCntWrite(3);//�����Դ���
              SeareDeviceInfo.CurrenStatus = 0X01; //ת������ͽ׶�
              
            case 0x01:  //����ͽ׶�
              if( SeareDeviceInfo.SeareCnt < SeareDeviceInfo.SeareLent  )//��ѯ�Ƿ��������
              {
                  USCIA0_RXBuffClearn(); //����ջ�����
                  if( SeareCommeSend() ) //���ͳ�������
                  {
                      SeareDeviceInfo.CurrenStatus = 0X02;//���ͳɹ�����ת����һ���׶�
                      USCIA0_FrameAckTime_En(1); //��ʱ�����ܿ��ؿ���������֡������󣬻����ϴ򿪼�ʱ�����ӿ���
                  }
                  else
                  {
                      SeareDeviceInfo.CurrenStatus = 0X01;//���Ͳ��ɹ�����ת�ص���ǰ�׶�
                  }
              }
              else                         //�ӻ��б����Ѿ�û����Ч�б����Ѿ���Ѱ���б�
              {   //�������
                  SeareDevcie_TaskOFF();
              }
              break;
              
            case 0x02://���մ����׶�
              if( USCIA0_FrameAckTime_ReTryCntRead() )     //��鳢�Դ���
              {
                  if( USCIA0_FrameAckTime_TimeCntRead() ) //����Ƿ�ʱ
                  {
                      switch( CJ188_ReceProce(&FrameTemp,SeareDeviceInfo.DeviceType,
                                              SeareDeviceInfo.CurrenDeviceStarID ) )
                      {
                      case 0x00://û������
                        break;
                      case 0x01://��������
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum); //���¼���ʱ�����
                          USCIA0_FrameAckTime_ReTryCntDec();            //������1
                          SeareDeviceInfo.CurrenStatus = 0X01;          //ת���׶�,���³���
                        break;
                      case 0x02://���ݷ���
                          USCIA0_FrameAckTime_SubEn(1);//���¿�ʼ��ʱ
                        break;
                      case 0x03://������ȷ
                          //�洢����
                          Add_DeviceListItem(SeareDeviceInfo.DeviceType,SeareDeviceInfo.CurrenDeviceStarID);
                          DeviceIDInc(SeareDeviceInfo.CurrenDeviceStarID); //��ǰ���ż�1
                          SeareDeviceInfo.SeareCnt++;                      //ָ����һ����
                          USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);    //���³�ʼ���������
                          USCIA0_FrameAckTime_ReTryCntWrite(3);
                          SeareDeviceInfo.CurrenStatus = 0X01;             //ת������׶�  
                        break;
                      default:
                        SeareDevcie_TaskOFF();//������ǰ����
                        break;
                      }
                  }
                  else
                  {   //DownChanne0_TaskInfo.TryTime �����ӳ�ʱ����������һ��
                      USCIA0_FrameAckTime_TimeCntWrite( ACKTimeNum ); //���¼���ʱ�����
                      USCIA0_FrameAckTime_ReTryCntDec();              //������1
                      SeareDeviceInfo.CurrenStatus = 0X01;            //ת���׶�,���³���
                  }
              }
              else
              {
                  DeviceIDInc(SeareDeviceInfo.CurrenDeviceStarID);//��ǰ���ż�1
                  SeareDeviceInfo.SeareCnt++;                     //ָ����һ����
                  USCIA0_FrameAckTime_TimeCntWrite(ACKTimeNum);   //���³�ʼ���������
                  USCIA0_FrameAckTime_ReTryCntWrite(3);
                  SeareDeviceInfo.CurrenStatus = 0X01;            //ת������׶�
              }
              break;
              
            default:
              break;
        }
    }
}


/******************************************************************************
  ����(ģ��)����:void DownChanne0_TaskProcess(void)
  ����:	����ͨ��0����������
  �������:��         	              		
  �������:         	 		   		 
  ����˵��:����ͨ��������������ڴ��������ڴ���
*******************************************************************************/
void DownChanne0_TaskProcess(void)
{
    
    if( ReadRTCFlag()&BIT0 || CheckReadMeterData() )
    {
        if( ReadMeasuData_TaskRequ(0x30) )
        {//��������ɹ�
            
            WriteRTCFlag( ReadRTCFlag()&(~BIT0) );
            DateMeterReadInfo.Status |= BIT0;
        }
    }
  
    if( DownChanne0_TaskInfo.Status & BIT0 )
    {
        switch( DownChanne0_TaskInfo.TaskID )
        {
        case 0x00: 
          ReadMeasureData_Task();  //����ͨ����������������
          break;
        case 0x01:
          SeareDevcie_Task();       //����ͨ���ѱ�����
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