#ifndef  __FlashManage_H
#define  __FlashManage_H

#define offsetof(type,member) ((size_t)(&((type *)0)->member)) //ȡ��ĳ���ṹ����Ԫ�ص�ƫ�Ƶ�ַ



/******************************************************************************
  ����:�߼��㲿�ֺ궨��
  ����:	�����߼���FLASH����ʼ��ַ����С������С������С�Լ�С������Ŀ��
  ����˵��: 
*******************************************************************************/
#define  FlashStartAddre    (unsigned long)0               //����FLASH����ʼ��ַ
#define  LogicFlashSize     (unsigned long)(60UL*1024UL)   //64KB,����FLASH��С
#define  LogicSectSize      120                            //�߼�С������С
#define  LogicSectMax       (LogicFlashSize/LogicSectSize) //���С������Ŀ



/******************************************************************************
  ����:�����㲿�ֺ궨��
  ����:	����������FLASH����ʼ��ַ����С���Լ�ÿ�����������������ɲ�������С��λ����С
        �������ϻ����ϣ�����С�������ɡ���С�Լ�ÿ�������������ܰ�����С������Ŀ���Լ�
��      �����鶨���FLASH�����У����ܰ�����С�����ĸ���
  ����˵��: 
*******************************************************************************/
#define PhyFlashStart          (unsigned long)0UL               //������FLASH��ʼ��ַ
#define PhyFlashSize           (unsigned long)(128UL*1024UL)     //96KB,������FLASH����(
#define BigPhySectSize         (unsigned long)(4*1024)          //4KB����������������

#define SmalPhySectDataSize    120   //С������������������
#define SmalPhySectInfoSize    8     //С����������Ϣ����С
#define SmalPhySectDataOffset  SmalPhySectInfoSize   //С��������������ƫ�Ƶ�ַ
#define SmalPhySectSize        (SmalPhySectDataSize+ SmalPhySectInfoSize)  //С����������С 
                                                                           //��Ϣ����������
#define SmalSectPerBig         (BigPhySectSize/SmalPhySectSize) //ÿһ������������
                                                                //�ڰ�����С������Ŀ
#define UnusedSizePerBig       (unsigned int)(BigPhySectSize-(SmalSectPerBig)*SmalPhySectSize)   //ÿ������������
                                                                                                 //�����õ��ֽ�
#define BigPhySectMax          (unsigned long)( (PhyFlashSize/BigPhySectSize)*SmalSectPerBig )   //���С��������



//����С������Ϣͷ
struct PhysicsSectorInfo
{
  unsigned char SectorValid;       //������Ч�ԣ�Ĭ��FF��ʾ��Ч��������ʾ��Ч���ɻ���
  unsigned char SectorEn;          //�������ã���Ϊ��Ϣ������
  unsigned char SectorDataCS;      //������У��
  unsigned char SectorDataStatus;  //����������״̬��FF��ʾ�������ɾ���������ʾ��д
  unsigned int  LogicSectorNum;    //ӳ��������  
  unsigned char SectorWritComple;  //����д��ɣ���ʾ����ӳ���������Ѿ�д��
  unsigned char SectorUpDate;      //�¾��������³ɹ���־λ�������������������ʾ���
};





/******************************************************************************
  ����:FLASH�洢������Ϣ�ṹ��
  ����:���Ӧ�Ĵ洢��Ϣ�������������������Լ���ǰ��д����������         	 		   		 
  ����˵��: 
*******************************************************************************/
struct FlashManage_InfoStruct
{
  unsigned int ValidSectorNum;        //��������
  unsigned int UsedSectorNum;         //�������� 
  unsigned int DirtySectorNum;        //�ɻ������� 
  unsigned int ErroSectorNum;         //�������� 
  unsigned int CurrentTrashCheck;     //��ǰ������������
  unsigned int CurrentWritePhySect;   //��ǰд��������
  unsigned int CurrentReadPhySect;    //��ǰ����������
};


/******************************************************************************
  ����:FLASH���������洢������Ϣ�ṹ��
  ����:���Ӧ�Ĵ洢��Ϣ�������������������Լ���ǰ��д����������         	 		   		 
  ����˵��: 
*******************************************************************************/
struct PhyBigSectorInfo
{
  unsigned char PhyBigSectNum;     //����������(ʵ����������)��
  struct FlashManage_InfoStruct SmalPhySectoInfo;
};


extern struct FlashManage_InfoStruct FlashManage_Info;

/******************************************************************************
  ����(ģ��)����:unsigned char FlashManage_Init(void)
  ����:FLASH�������ʼ��
  �������:         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char FlashManage_Init(void);


/******************************************************************************
  ����(ģ��)����:unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,unsigned char *DataBuffer)
  ����:�߼���������ģʽ
  �������:���Զ���ַ����ȡ���ȣ����ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,unsigned char *DataBuffer);


/******************************************************************************
  ����(ģ��)����:unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,unsigned char *DataBuffer)
  ����:�߼�д������ģʽ
  �������:����д��ַ�����ȣ����ݻ�����ָ��         	              		
  �������:          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,unsigned char *DataBuffer);

#endif