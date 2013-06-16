#ifndef  __DataMapping_H
#define  __DataMapping_H


//*****************�ɼ�������ͷ��Ϣ*****************************//
#define CollectorInfoTable_Addr    0x00000000   //�ɼ���������Ϣ����������ʼ��ַ 
                                                //��СΪ0XFF
#define CollectorType              0X02         //�ɼ������ͺ�
#define CurrenFileSYSVer           0X0100       //��ǰ�洢�汾�� V01.00
#define CurrenManuFacInfo          0X01         // LSD==0X01
struct FileBasicTableStruct
{
    unsigned char DeviceType;       //�豸����
    unsigned char DeviceIDBuff[7];  //�ɼ���ID��
    unsigned int  FileSYSVer;       //�洢�汾��
    unsigned char ManufacInfo;      //�豸������Ϣ
    unsigned char ReadMeterData[3]; //��������
    unsigned char CheckSum;         //У���
};




//***************************�豸���ͽṹ��**********************//
#define DeviceTypeTable_Addr           (CollectorInfoTable_Addr + sizeof(struct FileBasicTableStruct) + 256)  //�����б���ʼ��ַ
#define EnBit   0x01                   //��Чλ  0��Ч��1��Ч
struct DeviceTypeStruct
{
    unsigned char Num;                 //�б����
    unsigned char Status;              //�豸״̬�Ĵ���
    unsigned char DeviceType;          //�豸����
    unsigned long DeviceListPoint;     //�豸�б�ָ�� ָ�豸�б���FLASH�е���ʼ��ַ
    unsigned int  DeviceCnt;           //�豸���� ��ʾ���б����������豸
    unsigned int  Reaserv;             //����
    unsigned char CheckSum;            //��У���
};




//********************************�豸�б��ṹ��*******************************//
#define  DeviceListTable_Addr  ( DeviceTypeTable_Addr + sizeof(struct DeviceTypeStruct) + 256)  //�豸�б���ʼ��ַ
#define  DeviceCntMax    128    //�豸�б��������
struct DeviceListStruct
{
    unsigned char Num;                 //�б����
    unsigned char Status;              //�豸״̬�Ĵ���
    unsigned char DeviceIDBuff[7];     //�豸ID��
    unsigned long DeviceDataPoint;     //�豸���ݴ洢��ָ��
    unsigned char CheckSum;            //��У���    
};
#define  DeviceTypeDataMax   (unsigned long)sizeof(struct DeviceListStruct)*DeviceCntMax //һ���豸������Ҫ�����洢�ռ�






//*******************************�豸���ݽṹ��********************************//
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

#define  DeviceDataTable_Addr    ( DeviceListTable_Addr + DeviceTypeDataMax )  //�豸������ʼ��ַ
struct DeviceDataStruct
{
    struct DeviceCurrentData CurrenData;       //��ǰ���� 
    struct DeviceHistoryData HistoryData[12];  //��ʷ����
};



#endif