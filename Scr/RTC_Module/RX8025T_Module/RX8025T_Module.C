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
#include "stdio.h"
#include "msp430x54x.h"
#include "IIC_Module.H"
#include "RX8025T_Module.H"

/******************************************************************************
  函数(模块)名称:unsigned char GetWeekDay(unsigned char year,unsigned char month,unsigned char day)
  功能:	计算 2000-2099 内任意一天是星期几
  输入参数:
          year : 00-99 
          month: 01-12 
          day  : 01-31              		
  输出参数: 星期几         	 		   		 
  其它说明: 星期天用0表示
*******************************************************************************/
unsigned char GetWeekDay(unsigned char year,unsigned char month,unsigned char day)  
{
    const unsigned char WeekData[]={0,3,3,6,1,4,6,2,5,0,3,5};
    return ((6+WeekData[month-1]+year+day+year/4)%7); 
} 
        
/******************************************************************************
  函数(模块)名称:unsigned char Re_Init(void)
  功能:	
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 把RTC设置为24小时制
*******************************************************************************/
unsigned char Re_Init(void)
{
  unsigned char const REGBuffer[16] = {0x00,0x00,0x00,//0点0分0时开始 秒、分、时
                                 0x01,          //星期
                                 0x27,0x04,0x11,//日、月、年
                                 0x00,          //RAM
                                 0x00,0x00,0x00,//闹钟分、时、日
                                 0x00,0x00,     //计数器
                                 0x00,          //扩展寄存器
                                 0x00,          //标志位寄存器
                                 0x40 };        //控制寄存器
    return IIC_MultiWrite(Sec_addr,(unsigned char *)REGBuffer,16);
}        
        
        
/******************************************************************************
  函数(模块)名称:unsigned char Read_Staus(void)
  功能:	8025T读状态函数,判断低电压位标志位
  输入参数: 无        	              		
  输出参数: 1，低电压位正常。0，低电压位被置位或读寄存器出错          	 		   		 
  其它说明: 读取8025的状态标志位，如果标志位错，则返回失败
*******************************************************************************/
unsigned char Read_Staus(void)
{
    unsigned char Flag_REGData;  
    if(IIC_SingleRead(Flag_Addr,&Flag_REGData))//读取E标志位寄存器，以判断当前状态
    {
        if((Flag_REGData & VLF_BIT))
        {
            return 0;
        }
        else return 1;
    }
    else return 0;//读状态寄存器出错
}

/******************************************************************************
  函数(模块)名称:unsigned char RX8025T_Init(unsigned char Slave_Addr)
  功能:	8025SA初始化函数
  输入参数: 8025SA的从机地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 初始化后，会调用函数读取状态标志寄存器，如果标志出错，则重新初始化.
*******************************************************************************/
unsigned char RX8025T_Init(unsigned char Slave_Addr)
{
    IIC_Init(RX8025T_addr);             //设定8025T的地址并初始化通讯接口；
    if(!Read_Staus())
    {
        return Re_Init();    //重新初始化
    }
    return 1;
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
        return IIC_MultiRead(Sec_addr,Time_Buffer,3);    //读取时间
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent)
  功能:	读全部寄存器
  输入参数:         	              		
  输出参数: 1，读取成功；0，读取失败          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Read_Lent(unsigned char *Time_Buffer,unsigned int Lent)
{
    return IIC_MultiRead(Sec_addr,Time_Buffer,16);    //读取时间
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
        return IIC_MultiRead(Day_addr,Date_Buffer,3);    //读取日期
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
        return IIC_MultiRead(Sec_addr,DATE_Buffer,7);    //读取
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
        return IIC_MultiWrite(Sec_addr,DATE_Buffer,7);//把时间写入
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
    return IIC_MultiWrite(Sec_addr,WriteTemp,3);
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
    return IIC_MultiWrite(Day_addr,WriteTemp,3);
}

/******************************************************************************
  函数(模块)名称:unsigned char En_Alarm(void)
  功能:	打开闹钟功能
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Dia_Alarm(void)
{
    return IIC_SingleWrite(CTL_Addr,0x40);
}

/******************************************************************************
  函数(模块)名称:unsigned char En_Alarm(void)
  功能:	打开闹钟功能
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char En_Alarm(void)
{
    return IIC_SingleWrite(CTL_Addr,0x48);
}

/******************************************************************************
  函数(模块)名称:unsigned char Clearn_AlarmFlag(void)
  功能:	清闹钟标志位
  输入参数:         	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Clearn_AlarmFlag(void)
{
    unsigned char ReadTemp;
    if( IIC_SingleRead(Flag_Addr,&ReadTemp) )
    {
        return IIC_SingleWrite(Flag_Addr,ReadTemp & (~AF_BIT) ); 
    }
    else return 0;   
}

/******************************************************************************
  函数(模块)名称:unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour)
  功能:	日设置函数
  输入参数: BCD码格式        	              		
  输出参数:          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char Set_Alarm(unsigned char AlarmMinute,unsigned char AlarmHour,unsigned char AlarmDay)
{
    unsigned char WriteTemp[3];
    WriteTemp[0]=AlarmMinute;
    WriteTemp[1]=AlarmHour;
    WriteTemp[2]=AlarmDay;
    return IIC_MultiWrite(AlarmW0_addr,WriteTemp,3);
}