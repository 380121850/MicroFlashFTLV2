//************************************************************************
//
//                 MSP430F5XX
//             -----------------
//         /|\|              XIN|-  
//          | |                 |    ------
//          --|RST          XOUT|-  |  8   |
//            |                 |   |  0   |
//            |        SCL P10.1|-->|  2   |
//            |                 |   |  5   |
//            |        SDA P10.2|-->|  S   |
//                                  |      |
//                                  |______|
//  Lierda, Inc                      IIC 
//            
//************************************************************************
#include "msp430x54x.h"
#include "IIC_Module.H"
#include "RX8025SA_Module.H"


        
/******************************************************************************
  函数(模块)名称:unsigned char Read_Staus(void)
  功能:	8025SA读状态函数
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 读取8025SA的状态标志位，如果标志位错，则返回失败
*******************************************************************************/
void Re_Init(void)
{
    unsigned char ReadTemp[2];
    ReadTemp[0]=SET24_BIT;//选定为24小时制
    ReadTemp[1]=XST_BIT;  //时钟位置1
    IIC_Page_Write(REGE_Addr|BIT0,ReadTemp,2);//写入
}        
        
        
/******************************************************************************
  函数(模块)名称:unsigned char Read_Staus(void)
  功能:	8025SA读状态函数
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 读取8025SA的状态标志位，如果标志位错，则返回失败
*******************************************************************************/
unsigned char Read_Staus(void)
{
    unsigned char ReadTemp[2];
    IIC_Multi_read(REGE_Addr,ReadTemp,2);   //读取E、F两个控制寄存器，以判断当前状态
    if((ReadTemp[0]&TEST_BIT)||(ReadTemp[1]&PON_BIT)||(ReadTemp[1]&XST_BIT==0))
    {
        return 0;
    }
    else return 1;
}

/******************************************************************************
  函数(模块)名称:void RX8025S_Init(unsigned char Slave_Addr)
  功能:	8025SA初始化函数
  输入参数: 8025SA的从机地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 初始化后，会调用函数读取状态标志寄存器，如果标志出错，则重新初始化.
*******************************************************************************/
void RX8025S_Init(unsigned char Slave_Addr)
{
    IIC_Init(RX8025SA_addr);             //设定8025SA的地址并初始化通讯接口；
    if(!Read_Staus())
    {
        Re_Init();    //重新初始化
    }
}

/******************************************************************************
  函数(模块)名称:unsigned char Read_Time(unsigned char *Time_Buffer)
  功能:	读全部时间、日期函数
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_Time(unsigned char *Time_Buffer)
{
    if(Read_Staus())
    {
        return IIC_Multi_read(Sec_addr,Time_Buffer,3);    //读取时间
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char Read_Time(unsigned char *Time_Buffer)
  功能:	读全部时间、日期函数
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_Date(unsigned char *Date_Buffer)
{
    if(Read_Staus())
    {
        return IIC_Multi_read(Day_addr,Date_Buffer,3);    //读取日期
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char Read_AllTime(unsigned char *DATE_Buffer)
  功能:	读全部时间、日期函数
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_AllTime(unsigned char *DATE_Buffer)
{
    if(Read_Staus())
    {
        return IIC_Multi_read(Sec_addr,DATE_Buffer,7);    //读取
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char Write_AllTime(unsigned char *DATE_Buffer)
  功能:	把全部时间写入
  输入参数: 至少7个字节大小的缓冲区首地址        	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Write_AllTime(unsigned char *DATE_Buffer)
{
    if(Read_Staus())
    {
        return IIC_Page_Write(Sec_addr|BIT0,DATE_Buffer,7);//把时间写入
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:void Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour)
  功能:	时间设置函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Set_Time(unsigned char Second,unsigned char Minute,unsigned char Hour)
{
    unsigned char WriteTemp[3];
    WriteTemp[0]=Second;
    WriteTemp[1]=Minute;
    WriteTemp[2]=Hour;
    return IIC_Page_Write(Sec_addr|BIT0,WriteTemp,3);
}

/******************************************************************************
  函数(模块)名称:unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year)
  功能:	时间设置函数
  输入参数: BCD码格式，24小时制        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Set_Date(unsigned char Day,unsigned char Mon,unsigned char Year)
{
    unsigned char WriteTemp[3];
    WriteTemp[0]=Day;
    WriteTemp[1]=Mon;
    WriteTemp[2]=Year;
    return IIC_Page_Write(Day_addr|BIT0,WriteTemp,3);
}


/******************************************************************************
  函数(模块)名称:unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
  功能:	日设置函数
  输入参数: BCD码格式        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
void Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
{
    unsigned char ReadTemp[2];
    IIC_Multi_read(REGE_Addr,ReadTemp,2);   //读取E、F两个控制寄存器，以判断当前状态
    if(ReadTemp[0]&DALE_BIT)
    {
        ReadTemp[0]&=~DALE_BIT;//当前闹钟功能已经开启，则先关闭，再进行设定
        IIC_Single_Write(REGE_Addr|BIT0,ReadTemp[0]);
    }
    IIC_Single_Write(AlarmD0_addr|BIT0,AlarmMinute);//设定闹钟时间
    IIC_Single_Write(AlarmD1_addr|BIT0,AlarmHour);
    
    ReadTemp[0] |= DALE_BIT;//开启闹钟功能
    IIC_Page_Write(REGE_Addr|BIT0,ReadTemp,2);//写入
    IIC_Multi_read(REGE_Addr,ReadTemp,2);   //读取E、F两个控制寄存器，以判断当前状态
}