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
//            |        SDA P10.2|-->|  T   |
//                                  |      |
//                                  |______|
//  Lierda, Inc                      IIC 
//            
//************************************************************************
#include "msp430x54x.h"
#include "SoftWareIIC_Module.H"
static unsigned char Slave_Addres=0x64;
/******************************************************************************
  函数(模块)名称:void Delay(unsigned int m)
  功能:	I2C总线延时函数
  输入参数: 延时数        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
void Delay(unsigned int m)
{
   while(m-- > 0);
}

/******************************************************************************
  函数(模块)名称:void SoftWareIIC_Init_Port(void)
  功能:	IIC启动函数
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 在函数里面启动了供电
*******************************************************************************/
void SoftWareIIC_Init(unsigned char Slave_Addr)
{
    Slave_Addres=Slave_Addr;
    SoftWareIIC_SCL_L;
    SoftWareIIC_SCL_DIR_OUT;
    SoftWareIIC_SDA_H;
    SoftWareIIC_SDA_DIR_OUT;  
    SoftWareIIC_SCL_H;       //此时SDA=1,SCL=1
    Delay(1000);     //上电后，需要适当延时等待IIC准备就绪
}

/******************************************************************************
  函数(模块)名称:void SoftWareIIC_Start(void)
  功能:	总线启动信号
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: SCL处于高电平期间，当SDA出现下降沿时启动I2C总线
            本函数也用来产生重复起始条件
*******************************************************************************/
void SoftWareIIC_Start(void)
{
    SoftWareIIC_SDA_DIR_OUT; //SDA线输出状态
    SoftWareIIC_SCL_L;
    SoftWareIIC_SCL_DIR_OUT;
    SoftWareIIC_SDA_H;
    SoftWareIIC_SDA_DIR_OUT;  
    SoftWareIIC_SCL_H;       //此时SDA=1,SCL=1
    Delay(10);
    SoftWareIIC_SDA_L;
    Delay(10);
    SoftWareIIC_SCL_L;
}

/******************************************************************************
  函数(模块)名称:void SoftWareIIC_Stop(void)
  功能:	总线停止信号
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
void SoftWareIIC_Stop(void)
{
    SoftWareIIC_SDA_DIR_OUT; //SDA线输出状态
    SoftWareIIC_SCL_L;
    Delay(10);
    SoftWareIIC_SDA_L;
    Delay(10);
    SoftWareIIC_SCL_H;
    Delay(10);
    SoftWareIIC_SDA_H; 
}

/******************************************************************************
  函数(模块)名称:void ReadAck(void)
  功能:	读应答信号
  输入参数: 无        	              		
  输出参数: 应答信号或非应答信号          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char ReadAck(void)
{
    unsigned char Ack_Flag = 0;
    SoftWareIIC_SCL_L;
    SoftWareIIC_SDA_DIR_IN; //SDA线输入状态
    Delay(20);
    SoftWareIIC_SCL_H;
    if ((SoftWareIIC_IN_Port & SoftWareIIC_SDA_BIT))
    {
      Ack_Flag = 1;
    }
    SoftWareIIC_SCL_L;
    SoftWareIIC_SDA_DIR_OUT; //SDA线输出状态
    SoftWareIIC_SDA_H;
    return   Ack_Flag;
}

/******************************************************************************
  函数(模块)名称:void WriteAck(unsigned char ACK)
  功能:	输出应答或非应答信号
  输入参数: 无        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
void WriteAck(unsigned char ACK)
{
    if(ACK)  //输出非应答 1
    {
        SoftWareIIC_SDA_DIR_OUT; //SDA线输出状态
        SoftWareIIC_SDA_H;
        Delay(20);
        SoftWareIIC_SCL_H;
        Delay(20);
        SoftWareIIC_SCL_L;
        //SoftWareIIC_SDA_DIR_IN; //SDA线输入状态
    }
    else   //输出应答 0
    {
        SoftWareIIC_SDA_DIR_OUT; //SDA线输出状态  
        SoftWareIIC_SDA_L;
        Delay(20);
        SoftWareIIC_SCL_H;
        Delay(20);
        SoftWareIIC_SCL_L;
        //SoftWareIIC_SDA_DIR_IN; //SDA线输入状态
    }
}



/******************************************************************************
  函数(模块)名称:void SoftWareIIC_Send_Byte(unsigned char Date_Byte)
  功能:	向I2C总线发送一个节字
  输入参数: 要发送的字节        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
void SoftWareIIC_Send_Byte(unsigned char Date_Byte)
{
    unsigned int i;
    for (i=0;i<8;i++)
    {
      if(Date_Byte & 0x80)
      {
        SoftWareIIC_SDA_H;
      }
      else
      {
        SoftWareIIC_SDA_L;
      }
      SoftWareIIC_SCL_H;
      Delay(20);
      SoftWareIIC_SCL_L;
      Date_Byte = Date_Byte << 1;
    } 
}

/******************************************************************************
  函数(模块)名称:unsigned char SoftWareIIC_Receive_Byte(void)
  功能:	从I2C总线接收一个字节
  输入参数: 无        	              		
  输出参数: 接收到的字节          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SoftWareIIC_Receive_Byte(void)
{
    unsigned int i;
    unsigned char R_word = 0x00;
    SoftWareIIC_SDA_DIR_IN;
    for(i=0;i<8;i++)
    {
      Delay(5);
      R_word = R_word << 1;
      SoftWareIIC_SCL_H;
      if(SoftWareIIC_IN_Port & SoftWareIIC_SDA_BIT)
      {
        R_word |= 0x01;
      }
      Delay(20);
      SoftWareIIC_SCL_L;
     }
     SoftWareIIC_SDA_DIR_OUT;
     return  R_word;
}

/******************************************************************************
  函数(模块)名称:unsigned char SoftWareIIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  功能:	单次读/随机读
  输入参数: 要读的数据地址，输出数据保存的字节指针        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SoftWareIIC_Single_Read(unsigned char R_addr,unsigned char *R_word)
{
    unsigned char try_time=Softdefine_try_time; //用来定义出现通讯错误时尝试的错误
    while(1)
    {
        if(!try_time--)    //用来防止因硬件错误或其它原因程序进入死循环
        {
           return 0;
        }
        SoftWareIIC_Start();       //步骤1：产生一个启动信号
        SoftWareIIC_Send_Byte(Slave_Addres&(~BIT0));   //步骤2：发送控制命令
        if(ReadAck())
          continue;        //没有应答，忽略下面语句，重新开始
                           //应答，则开始发送地址
        SoftWareIIC_Send_Byte(R_addr);
        if(ReadAck())         
          continue;        //没有应答，忽略下面语句，重新开始
                           //应答，则开始发送启动信号
        SoftWareIIC_Start();       //启动信号
        SoftWareIIC_Send_Byte(Slave_Addres|BIT0);   //发送控制命令
        if(ReadAck())         
          continue;        //没有应答，忽略下面语句，重新开始
        break;             //应答，则跳出循环，开始接收数据
    }
    *R_word=SoftWareIIC_Receive_Byte(); //从总线上接收一个字节数据
    WriteAck(1);               //发送 非应答信号
    SoftWareIIC_Stop();                //发送停止信号
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char SoftWareIIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  功能:	连续读
  输入参数: 读起始地址，数据保存指针，读长度        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SoftWareIIC_Multi_read(unsigned char R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
{
    unsigned char try_time=Softdefine_try_time; //用来定义出现通讯错误时尝试的错误
    while(1)
    {
        if(!try_time--)    //用来防止因硬件错误或其它原因程序进入死循环
        {
           return 0;
        }
        SoftWareIIC_Start();           //步骤1：产生一个启动信号
        SoftWareIIC_Send_Byte(Slave_Addres&(~BIT0));       //步骤2：发送控制命令
        if(ReadAck())
          continue;        //没有应答，忽略下面语句，重新开始
                           //应答，则开始发送地址
        SoftWareIIC_Send_Byte(R_addr);
        if(ReadAck())         
          continue;        //没有应答，忽略下面语句，重新开始
                           //应答，则开始发送启动信号
        SoftWareIIC_Start();       //启动信号
        SoftWareIIC_Send_Byte(Slave_Addres|BIT0);   //发送控制命令
        if(ReadAck())         
          continue;        //没有应答，忽略下面语句，重新开始
        break;             //应答，则跳出循环，开始接收数据
    }
    while(R_lent--)
    {
        *Save_addr++=SoftWareIIC_Receive_Byte(); //从总线上接收一个字节数据
        if(R_lent==0)
        {
            WriteAck(1);               //发送 非应答信号
            SoftWareIIC_Stop();                //发送停止信号
            break;
        }
        WriteAck(0);               //发送应答信号
    }
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char SoftWareIIC_Single_Write(unsigned char W_addr,unsigned char Date_byte)
  功能:	IIC总线单字节写
  输入参数: 要写的地址，要写的数据        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char SoftWareIIC_Single_Write(unsigned char W_addr,unsigned char Date_byte)
{
    unsigned char try_time=Softdefine_try_time; //用来定义出现通讯错误时尝试的错误
    while(1)
    {
        if(!try_time--)    //用来防止因硬件错误或其它原因程序进入死循环
        {
           return 0;
        }
        SoftWareIIC_Start();                           //步骤1：主机产生一个启动信号
        SoftWareIIC_Send_Byte(Slave_Addres&(~BIT0));   //步骤2：主机发送从机地址及读写位
        if(ReadAck())
          continue;        //步骤3：主机接收从机应答；没有应答，忽略下面语句，重新开始
                           //应答，则开始发送地址
        SoftWareIIC_Send_Byte(W_addr);     //步骤4：主机发送读写地址
        if(ReadAck())         
          continue;               //步骤5：主机接收从机应答 没有应答，忽略下面语句，重新开始
        SoftWareIIC_Send_Byte(Date_byte); //步骤6：主机发送数据 //应答，则开始发送要写入的数据
        if(ReadAck())     //步骤7：机接收从机应答
          continue;      //没有应答，忽略下面语句，重新开始
        break;           //应答，则跳出循环
    }
    SoftWareIIC_Stop();          //步骤8：主机发送停止信号,启动写周期
    Delay(5000);         //需要延时一段时间，等待写周期完成
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char SoftWareIIC_Page_Write(unsigned char W_addr,
                                              unsigned char *WriteData_Addr,
                                              unsigned char WriteData_Cnt)
  功能:	IIC页写
  输入参数: 要写的地址，要写的数据指针，要写的字节数        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 要写的字节数，不能大于当前写字节地址到当前页最大地址之差。
*******************************************************************************/
unsigned char SoftWareIIC_Page_Write(unsigned char W_addr,unsigned char *WriteData_Addr,
                             unsigned char WriteData_Cnt)
{
    unsigned char try_time=Softdefine_try_time; //用来定义出现通讯错误时尝试的错误
    while(1)
    {
        if(!try_time--)    //用来防止因硬件错误或其它原因程序进入死循环
        {
           return 0;
        }
        SoftWareIIC_Start();                           //步骤1：主机产生一个启动信号
        SoftWareIIC_Send_Byte(Slave_Addres&(~BIT0));   //步骤2：主机发送从机地址及读写位
        if(ReadAck())
          continue;        //步骤3：主机接收从机应答；没有应答，忽略下面语句，重新开始
                           //应答，则开始发送地址
        SoftWareIIC_Send_Byte(W_addr);     //步骤4：主机发送读写地址
        if(ReadAck())         
          continue;               //步骤5：主机接收从机应答 没有应答，忽略下面语句，重新开始
        while(WriteData_Cnt--)
        {
            SoftWareIIC_Send_Byte(*(WriteData_Addr++));
            if(!ReadAck())  
              continue;   //收到应答(0)，则继续发送数据   
            return 0;     //没收到应答，错误返回
        }
        break;
    }   
    SoftWareIIC_Stop();          //步骤8：主机发送停止信号,启动写周期
    Delay(5000);         //需要延时一段时间，等待写周期完成
    return 1;
}