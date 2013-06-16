#ifndef  __RX8025SA_Module_H
#define  __RX8025SA_Module_H

#define RX8025SA_addr 0x64   //8025SA的地址

#define Sec_addr      0x00   //秒寄存器地址
#define Minu_addr     0x10   //分
#define Hour_addr     0x20   //时

#define Week_addr     0x30   //星期(00~06)
#define Day_addr      0x40   //日
#define Mon_addr      0x50   //月
#define Year_addr     0x60   //年

#define Offset_addr   0x70   //偏移量调整寄存器
#define AlarmW0_addr  0x80   //定时设置分寄存器
#define AlarmW1_addr  0x90   //定时设置时寄存器
#define AlarmW2_addr  0xA0   //定时设置星期寄存器

#define AlarmD0_addr  0xB0   //定时设置分寄存器
#define AlarmD1_addr  0xC0   //定时设置时寄存器

#define Reser_addr    0xD0   //保留寄存器
#define REGE_Addr     0xE0   //控制寄存器1
        #define WALE_BIT     BIT7   //WALE位，星期、时，分报警设定位
        #define DALE_BIT     BIT6   //DALE位，时、分报警遍位
        #define SET24_BIT    BIT5   //12小时制或24小时制
        #define TEST_BIT     BIT3   //测试位，这一位不应写入1
        #define CT2_BIT      BIT2   //中断周期模式选择
        #define CT1_BIT      BIT1
        #define CT0_BIT      BIT0
     

#define REGF_Addr     0xF0          //控制寄存器2
        #define VDSL_BIT      BIT7   //
        #define VDET_BIT      BIT6   //
        #define XST_BIT       BIT5     //
        #define PON_BIT       BIT4    //
        #define CTFG_BIT      BIT2  //
        #define WAFG_BIT      BIT1
        #define DAFG_BIT      BIT0

/******************************************************************************
  函数(模块)名称:void RX8025S_Init(unsigned char Slave_Addr)
  功能:	8025SA初始化函数
  输入参数: 8025SA的从机地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 初始化后，会调用函数读取状态标志寄存器，如果标志出错，则重新初始化.
*******************************************************************************/
void RX8025S_Init(unsigned char Slave_Addr);


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
void Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour);


#endif