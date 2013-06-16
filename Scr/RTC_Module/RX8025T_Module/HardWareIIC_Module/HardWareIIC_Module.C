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
#include "HardWareIIC_Module.H"

/******************************************************************************
  函数(模块)名称:void HardWareIIC_Init(unsigned char Slave_Addr)
  功能:	IIC初始化函数，可以设置从机地址
  输入参数: 从机地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 传入的从机地址，需要右移一位再放入地址寄存器
*******************************************************************************/
void HardWareIIC_Init(unsigned char Slave_Addr)
{
    P10SEL |= BIT1 + BIT2;
    //P10REN |= BIT1 + BIT2;                  //不建议设置
    UCB3CTL1 |= UCSWRST;                      // 置位UCSWRST
    UCB3CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C 主机, 同步 模式
    UCB3CTL1 |= UCSSEL__SMCLK;                // SMCLK
    //UCB3BR0 = 96;                           // fSCL = 8M/80 = ~100kHz
    //UCB3BR1 = 0;
    UCB3BRW = 20;
    UCB3I2CSA = Slave_Addr>>1;                // 从机地址0x64
    UCB3CTL1 &= ~UCSWRST;                     // 清除UCSWRST
}


/******************************************************************************
  函数(模块)名称:unsigned char HardWareIIC_ReadInit( unsigned char ReadAddr )
  功能:	读初始化函数
  输入参数: 读地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 产生单字节读或多字节前前面阶段：发送起始位和从机地址位，发送读地址
            发送重复起始位，并设置为接收模式
*******************************************************************************/
unsigned char HardWareIIC_ReadInit( unsigned char ReadAddr )
{
    unsigned char try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
    
    UCB3IFG &= ~ UCRXIFG;
    
    //try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
    
    UCB3CTL1 |= (UCTXSTT + UCTR);     // 发送起始位，确定为写 
    while((UCB3IFG & UCTXIFG)==0)     // 判断是否发送完毕 
    {
       if(!try_time--)
        {
            return 0;
        }
    }
    UCB3TXBUF = ReadAddr;             // 发送地址

    //try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
    while (UCB3CTL1 & UCTXSTT)        // 判断起始位是否被应答
    {
        if(!try_time--)
        {
            return 0;
        }
    }
    
    //try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错
    while((UCB3IFG & UCTXIFG)==0)    // 判断是否发送完毕 
    {
       if(!try_time--)
        {
            return 0;
        }
    }
    
    UCB3CTL1 &= ~UCTR;                     // 确定为接收
    UCB3CTL1 |=UCTXSTT;                    // 重新发送起始位
    //try_time=HardWaredefine_try_time;      //用来定义出现通讯错误时尝试的错
    while( (UCB3CTL1 & UCTXSTT) )          // 判断起始位是否被应答
    {
        if(!try_time--)
        {
            return 0;
        }
    }
    return 1;
}

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  功能:	单次读/随机读
  输入参数: 要读的数据地址，输出数据保存的字节指针        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 有查询超时，防止因硬件错误陷入死锁状态。
*******************************************************************************/
unsigned char HardWareIIC_SingleRead(unsigned char ReadAddr,unsigned char *ReadData)
{
    unsigned char try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
    if( HardWareIIC_ReadInit( ReadAddr ) )
    {
        UCB3CTL1 |= (UCTXSTP + UCTXNACK);      // 发送停止位和NACK位 
        while (UCB3CTL1 & UCTXSTP)             // Ensure stop condition got sent
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        
        while((UCB3IFG  & UCRXIFG)==0)
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        *ReadData = UCB3RXBUF; 
        return 1; 
    }
    else return 0; 
}

/******************************************************************************
  函数(模块)名称:unsigned char IIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  功能:	连续读
  输入参数: 读起始地址，数据保存指针，读长度        	              		
  输出参数: 1 读成功 0 读失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_MultiRead(unsigned char ReadAddr,unsigned char *ReadBuff,
                                unsigned int ReadLent)
{
    static unsigned char try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
    if( HardWareIIC_ReadInit( ReadAddr ) )
    {
        for(unsigned int i=0;i<ReadLent;i++)
        {
            try_time=HardWaredefine_try_time;
            while((UCB3IFG  & UCRXIFG)==0)           // 接收中断
            {
                if(!try_time--)
                {
                    return 0;
                }
            }
            *ReadBuff++ = UCB3RXBUF;        
        }
        UCB3CTL1 |=UCTXSTP + UCTXNACK;            // 发送停止位和NACK位 
        return 1;
    }
    else return 0;
}


/******************************************************************************
  函数(模块)名称:unsigned char HardWareIIC_WriteInit( unsigned char WriteAddr )
  功能:	写初始化函数
  输入参数: 地址        	              		
  输出参数: 无          	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_WriteInit( unsigned char WriteAddr )
{
    unsigned char try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
      
    UCB3CTL1 |= UCTXSTT + UCTR;               // 发送起始位，确定为发送模式
    
    try_time=HardWaredefine_try_time;         //用来定义出现通讯错误时尝试的错误
    while((UCB3IFG & UCTXIFG)==0)             // 判断是否发送完毕 
    {
        if(!try_time--)
        {
            return 0;
        }
    }
    UCB3TXBUF = WriteAddr;                    // 发送高位地址
    
    try_time=HardWaredefine_try_time;         //用来定义出现通讯错误时尝试的错误
    while (UCB3CTL1 & UCTXSTT)                // 判断起始位是否被应答
    {
        if(!try_time--)
        {
            return 0;
        }
    } 
    return 1;
    
}


/******************************************************************************
  函数(模块)名称:unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,
                                                       unsigned char WriteData)
  功能:	IIC总线单字节写
  输入参数: 要写的地址，要写的数据        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,unsigned char WriteData)
{
    unsigned char try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
      
    if( HardWareIIC_WriteInit( WriteAddr ) )
    {
        //try_time=HardWaredefine_try_time;         //用来定义出现通讯错误时尝试的错误
        while((UCB3IFG & UCTXIFG)==0)             // 判断是否发送完毕 
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        
        UCB3TXBUF = WriteData;                    // 发送数据
        //try_time=HardWaredefine_try_time;         //用来定义出现通讯错误时尝试的错误
        while((UCB3IFG & UCTXIFG)==0)             // 判断是否发送完毕 
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        
        UCB3CTL1 |= UCTXSTP;                      // 发送停止位
        //try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
        while (UCB3CTL1 & UCTXSTP)               // Ensure stop condition got sent
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        return 1;
    }
    else return 0;
}

/******************************************************************************
  函数(模块)名称:unsigned char HardWareIIC_MultiWrite(unsigned char WriteAddr,
                                     unsigned char *WriteBuffer,
                                     unsigned char DataLent)
  功能:	多字节写函数
  输入参数: 要写的地址，要写的数据指针，要写的字节数        	              		
  输出参数: 1 写成功 0 写失败         	 		   		 
  其它说明: 
*******************************************************************************/
unsigned char HardWareIIC_MultiWrite(unsigned char WriteAddr,
                                     unsigned char *WriteBuffer,
                                     unsigned char DataLent)
{
    unsigned char try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
      
    if( HardWareIIC_WriteInit( WriteAddr ) )
    {
        for(unsigned int i=0;i<DataLent;i++)
        {
            try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
            while((UCB3IFG & UCTXIFG)==0)             // 判断是否发送完毕 
            {
                if(!try_time--)
                {
                    return 0;
                }
            }
            UCB3TXBUF = *WriteBuffer++;                 // 发送数据
        }
        try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
        while((UCB3IFG & UCTXIFG)==0)             // 判断是否发送完毕 
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        
        UCB3CTL1 |= UCTXSTP;                      // 发送停止位
        try_time=HardWaredefine_try_time; //用来定义出现通讯错误时尝试的错误
        while (UCB3CTL1 & UCTXSTP)               // Ensure stop condition got sent
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        return 1;
    }
    else return 0;
}