#ifndef  __FlashManage_H
#define  __FlashManage_H

#define offsetof(type,member) ((size_t)(&((type *)0)->member)) //取得某个结构体内元素的偏移地址



/******************************************************************************
  名称:逻辑层部分宏定义
  功能:	定义逻辑层FLASH的起始地址、大小、定义小扇区大小以及小扇区数目。
  其它说明: 
*******************************************************************************/
#define  FlashStartAddre    (unsigned long)0               //线性FLASH的起始地址
#define  LogicFlashSize     (unsigned long)(60UL*1024UL)   //64KB,线性FLASH大小
#define  LogicSectSize      120                            //逻辑小扇区大小
#define  LogicSectMax       (LogicFlashSize/LogicSectSize) //最大小扇区数目



/******************************************************************************
  名称:物理层部分宏定义
  功能:	定义物理层FLASH的起始地址、大小、以及每个物理扇区（物理可擦除的最小单位）大小
        再在以上基本上，定义小扇区构成、大小以及每个物理扇区所能包含的小扇区数目。以及
　      在整块定义的FLASH区域中，所能包含的小扇区的个数
  其它说明: 
*******************************************************************************/
#define PhyFlashStart          (unsigned long)0UL               //物理层FLASH起始地址
#define PhyFlashSize           (unsigned long)(128UL*1024UL)     //96KB,物理层FLASH容量(
#define BigPhySectSize         (unsigned long)(4*1024)          //4KB，大物理扇区容量

#define SmalPhySectDataSize    120   //小物理扇区数据区容量
#define SmalPhySectInfoSize    8     //小物理扇区信息区大小
#define SmalPhySectDataOffset  SmalPhySectInfoSize   //小物理扇区数据区偏移地址
#define SmalPhySectSize        (SmalPhySectDataSize+ SmalPhySectInfoSize)  //小物理扇区大小 
                                                                           //信息区＋数据区
#define SmalSectPerBig         (BigPhySectSize/SmalPhySectSize) //每一个大物理扇区
                                                                //内包含的小扇区数目
#define UnusedSizePerBig       (unsigned int)(BigPhySectSize-(SmalSectPerBig)*SmalPhySectSize)   //每个大物理扇区
                                                                                                 //不能用的字节
#define BigPhySectMax          (unsigned long)( (PhyFlashSize/BigPhySectSize)*SmalSectPerBig )   //最大小扇区容量



//物理小扇区信息头
struct PhysicsSectorInfo
{
  unsigned char SectorValid;       //扇区有效性，默认FF表示有效，其它表示无效，可回收
  unsigned char SectorEn;          //扇区启用，作为信息检测起点
  unsigned char SectorDataCS;      //数据区校验
  unsigned char SectorDataStatus;  //扇区数据区状态，FF表示数据区干净，其它表示已写
  unsigned int  LogicSectorNum;    //映射扇区号  
  unsigned char SectorWritComple;  //扇区写完成，表示至少映射扇区号已经写入
  unsigned char SectorUpDate;      //新旧扇区更新成功标志位，屏掉旧扇区后，置起表示完成
};





/******************************************************************************
  名称:FLASH存储管理信息结构体
  功能:相对应的存储信息，包括可用扇区数等以及当前读写的物理扇区         	 		   		 
  其它说明: 
*******************************************************************************/
struct FlashManage_InfoStruct
{
  unsigned int ValidSectorNum;        //可用扇区
  unsigned int UsedSectorNum;         //已用扇区 
  unsigned int DirtySectorNum;        //可回收扇区 
  unsigned int ErroSectorNum;         //错误扇区 
  unsigned int CurrentTrashCheck;     //当前垃圾回收扇区
  unsigned int CurrentWritePhySect;   //当前写物理扇区
  unsigned int CurrentReadPhySect;    //当前读物理扇区
};


/******************************************************************************
  名称:FLASH物理扇区存储管理信息结构体
  功能:相对应的存储信息，包括可用扇区数等以及当前读写的物理扇区         	 		   		 
  其它说明: 
*******************************************************************************/
struct PhyBigSectorInfo
{
  unsigned char PhyBigSectNum;     //大物理扇区(实际物理扇区)号
  struct FlashManage_InfoStruct SmalPhySectoInfo;
};


extern struct FlashManage_InfoStruct FlashManage_Info;

/******************************************************************************
  函数(模块)名称:unsigned char FlashManage_Init(void)
  功能:FLASH管理层初始化
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char FlashManage_Init(void);


/******************************************************************************
  函数(模块)名称:unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,unsigned char *DataBuffer)
  功能:逻辑读，线性模式
  输入参数:线性读地址，读取长度，数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char LogicFlashRead(unsigned long ReadAddr,unsigned int DataLent,unsigned char *DataBuffer);


/******************************************************************************
  函数(模块)名称:unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,unsigned char *DataBuffer)
  功能:逻辑写，线性模式
  输入参数:线性写地址，长度，数据缓冲区指针         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char LogicFlashWrite(unsigned long WriteAddr,unsigned int DataLent,unsigned char *DataBuffer);

#endif