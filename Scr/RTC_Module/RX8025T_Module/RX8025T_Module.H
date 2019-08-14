#ifndef  __RX8025T_Module_H
#define  __RX8025T_Module_H

#define RX8025T_addr 0x64   //8025SA的地址

#define Sec_addr      0x00   //秒寄存器地址
#define Minu_addr     0x01   //分
#define Hour_addr     0x02   //时

#define Week_addr     0x03   //星期(00~06)
#define Day_addr      0x04   //日
#define Mon_addr      0x05   //月
#define Year_addr     0x06   //年

#define RAM_addr      0x07   //任意读写
#define AlarmW0_addr  0x08   //定时设置分寄存器
#define AlarmW1_addr  0x09   //定时设置时寄存器
#define AlarmW2_addr  0x0A   //定时设置星期寄存器

#define TimerCounter0_addr  0x0B   //定时计数器0
#define TimerCounter1_addr  0x0C   //定时计数器1

#define Extern_addr    0x0D   //扩展寄存器
        #define TEST_BIT       BIT7   //
        #define WADA_BIT       BIT6   //
        #define USEL_BIT       BIT5   //时间更新标志位
        #define TE_BIT         BIT4   //固定周期定时中断标志位
        #define FSEL1_BIT      BIT3   //闹钟中断标志痊
        #define FSEL0_BIT      BIT2
        #define TESL1_BIT      BIT1
        #define TESL0_BIT      BIT0

#define Flag_Addr      0x0E    //标志寄存器1
        //#define _BIT       BIT7   //
        //#define _BIT       BIT6   //
        #define UF_BIT       BIT5   //时间更新标志位
        #define TF_BIT       BIT4   //固定周期定时中断标志位
        #define AF_BIT       BIT3   //闹钟中断标志痊
        //#define _BIT      BIT2
        #define VLF_BIT       BIT1
        #define VDET_BIT      BIT0
     

#define CTL_Addr     0x0F          //控制寄存器2
        #define CSEL1_BIT      BIT7   //
        #define CSEL0_BIT      BIT6   //
        #define UIE_BIT        BIT5     //
        #define TIE_BIT        BIT4    //
        #define AIE_BIT        BIT3  //
        //#define _BIT      BIT2
        //#define _BIT      BIT1
        #define REST_BIT      BIT0

/******************************************************************************
  函数(模块)名称:unsigned char Read_Staus(void)
  功能:	8025T读状态函数,判断低电压位标志位
  输入参数: 无        	              		
  输出参数: 1，低电压位正常。0，低电压位被置位或读寄存器出错          	 		   		 
  其它说明: 读取8025的状态标志位，如果标志位错，则返回失败
*******************************************************************************/
unsigned char Read_Staus(void);

/******************************************************************************
  函数(模块)名称:void RX8025S_Init(unsigned char Slave_Addr)
  功能:	8025SA初始化函数
  输入参数: 8025SA的从机地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 初始化后，会调用函数读取状态标志寄存器，如果标志出错，则重新初始化.
*******************************************************************************/
unsigned char RX8025T_Init(unsigned char Slave_Addr);


/******************************************************************************
  函数(模块)名称:unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent)
  功能:	读全部寄存器
  输入参数:         	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent);

/******************************************************************************
  函数(模块)名称:unsigned char Read_Time(unsigned char *Time_Buffer)
  功能:	读全部时间、日期函数
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_Time(unsigned char *Time_Buffer);

/******************************************************************************
  函数(模块)名称:unsigned char Read_Time(unsigned char *Time_Buffer)
  功能:	读全部时间、日期函数
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_Date(unsigned char *Date_Buffer);

/******************************************************************************
  函数(模块)名称:unsigned char Read_AllTime(unsigned char *DATE_Buffer)
  功能:	读全部时间、日期函数
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_AllTime(unsigned char *DATE_Buffer);

/******************************************************************************
  函数(模块)名称:unsigned char Write_AllTime(unsigned char *DATE_Buffer)
  功能:	把全部时间写入
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Write_AllTime(unsigned char *DATE_Buffer);

/******************************************************************************
  函数(模块)名称:void Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour)
  功能:	时间设置函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour);

/******************************************************************************
  函数(模块)名称:unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year)
  功能:	时间设置函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year);

/******************************************************************************
  函数(模块)名称:unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
  功能:	日设置函数
  输入参数: BCD码格式        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay);

unsigned char En_Alarm(void);
/******************************************************************************
  函数(模块)名称:unsigned char En_Alarm(void)
  功能:	打开闹钟功能
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Dia_Alarm(void);

/******************************************************************************
  函数(模块)名称:unsigned char Clearn_AlarmFlag(void)
  功能:	清闹钟标志位
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Clearn_AlarmFlag(void);
#endif