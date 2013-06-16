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
  ����(ģ��)����:void HardWareIIC_Init(unsigned char Slave_Addr)
  ����:	IIC��ʼ���������������ôӻ���ַ
  �������: �ӻ���ַ        	              		
  �������: ��          	 		   		 
  ����˵��: ����Ĵӻ���ַ����Ҫ����һλ�ٷ����ַ�Ĵ���
*******************************************************************************/
void HardWareIIC_Init(unsigned char Slave_Addr)
{
    P10SEL |= BIT1 + BIT2;
    //P10REN |= BIT1 + BIT2;                  //����������
    UCB3CTL1 |= UCSWRST;                      // ��λUCSWRST
    UCB3CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C ����, ͬ�� ģʽ
    UCB3CTL1 |= UCSSEL__SMCLK;                // SMCLK
    //UCB3BR0 = 96;                           // fSCL = 8M/80 = ~100kHz
    //UCB3BR1 = 0;
    UCB3BRW = 20;
    UCB3I2CSA = Slave_Addr>>1;                // �ӻ���ַ0x64
    UCB3CTL1 &= ~UCSWRST;                     // ���UCSWRST
}


/******************************************************************************
  ����(ģ��)����:unsigned char HardWareIIC_ReadInit( unsigned char ReadAddr )
  ����:	����ʼ������
  �������: ����ַ        	              		
  �������: ��          	 		   		 
  ����˵��: �������ֽڶ�����ֽ�ǰǰ��׶Σ�������ʼλ�ʹӻ���ַλ�����Ͷ���ַ
            �����ظ���ʼλ��������Ϊ����ģʽ
*******************************************************************************/
unsigned char HardWareIIC_ReadInit( unsigned char ReadAddr )
{
    unsigned char try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    
    UCB3IFG &= ~ UCRXIFG;
    
    //try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    
    UCB3CTL1 |= (UCTXSTT + UCTR);     // ������ʼλ��ȷ��Ϊд 
    while((UCB3IFG & UCTXIFG)==0)     // �ж��Ƿ������ 
    {
       if(!try_time--)
        {
            return 0;
        }
    }
    UCB3TXBUF = ReadAddr;             // ���͵�ַ

    //try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    while (UCB3CTL1 & UCTXSTT)        // �ж���ʼλ�Ƿ�Ӧ��
    {
        if(!try_time--)
        {
            return 0;
        }
    }
    
    //try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ�
    while((UCB3IFG & UCTXIFG)==0)    // �ж��Ƿ������ 
    {
       if(!try_time--)
        {
            return 0;
        }
    }
    
    UCB3CTL1 &= ~UCTR;                     // ȷ��Ϊ����
    UCB3CTL1 |=UCTXSTT;                    // ���·�����ʼλ
    //try_time=HardWaredefine_try_time;      //�����������ͨѶ����ʱ���ԵĴ�
    while( (UCB3CTL1 & UCTXSTT) )          // �ж���ʼλ�Ƿ�Ӧ��
    {
        if(!try_time--)
        {
            return 0;
        }
    }
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  ����:	���ζ�/�����
  �������: Ҫ�������ݵ�ַ��������ݱ�����ֽ�ָ��        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: �в�ѯ��ʱ����ֹ��Ӳ��������������״̬��
*******************************************************************************/
unsigned char HardWareIIC_SingleRead(unsigned char ReadAddr,unsigned char *ReadData)
{
    unsigned char try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    if( HardWareIIC_ReadInit( ReadAddr ) )
    {
        UCB3CTL1 |= (UCTXSTP + UCTXNACK);      // ����ֹͣλ��NACKλ 
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
  ����(ģ��)����:unsigned char IIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  ����:	������
  �������: ����ʼ��ַ�����ݱ���ָ�룬������        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char HardWareIIC_MultiRead(unsigned char ReadAddr,unsigned char *ReadBuff,
                                unsigned int ReadLent)
{
    static unsigned char try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    if( HardWareIIC_ReadInit( ReadAddr ) )
    {
        for(unsigned int i=0;i<ReadLent;i++)
        {
            try_time=HardWaredefine_try_time;
            while((UCB3IFG  & UCRXIFG)==0)           // �����ж�
            {
                if(!try_time--)
                {
                    return 0;
                }
            }
            *ReadBuff++ = UCB3RXBUF;        
        }
        UCB3CTL1 |=UCTXSTP + UCTXNACK;            // ����ֹͣλ��NACKλ 
        return 1;
    }
    else return 0;
}


/******************************************************************************
  ����(ģ��)����:unsigned char HardWareIIC_WriteInit( unsigned char WriteAddr )
  ����:	д��ʼ������
  �������: ��ַ        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char HardWareIIC_WriteInit( unsigned char WriteAddr )
{
    unsigned char try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
      
    UCB3CTL1 |= UCTXSTT + UCTR;               // ������ʼλ��ȷ��Ϊ����ģʽ
    
    try_time=HardWaredefine_try_time;         //�����������ͨѶ����ʱ���ԵĴ���
    while((UCB3IFG & UCTXIFG)==0)             // �ж��Ƿ������ 
    {
        if(!try_time--)
        {
            return 0;
        }
    }
    UCB3TXBUF = WriteAddr;                    // ���͸�λ��ַ
    
    try_time=HardWaredefine_try_time;         //�����������ͨѶ����ʱ���ԵĴ���
    while (UCB3CTL1 & UCTXSTT)                // �ж���ʼλ�Ƿ�Ӧ��
    {
        if(!try_time--)
        {
            return 0;
        }
    } 
    return 1;
    
}


/******************************************************************************
  ����(ģ��)����:unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,
                                                       unsigned char WriteData)
  ����:	IIC���ߵ��ֽ�д
  �������: Ҫд�ĵ�ַ��Ҫд������        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,unsigned char WriteData)
{
    unsigned char try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
      
    if( HardWareIIC_WriteInit( WriteAddr ) )
    {
        //try_time=HardWaredefine_try_time;         //�����������ͨѶ����ʱ���ԵĴ���
        while((UCB3IFG & UCTXIFG)==0)             // �ж��Ƿ������ 
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        
        UCB3TXBUF = WriteData;                    // ��������
        //try_time=HardWaredefine_try_time;         //�����������ͨѶ����ʱ���ԵĴ���
        while((UCB3IFG & UCTXIFG)==0)             // �ж��Ƿ������ 
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        
        UCB3CTL1 |= UCTXSTP;                      // ����ֹͣλ
        //try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
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
  ����(ģ��)����:unsigned char HardWareIIC_MultiWrite(unsigned char WriteAddr,
                                     unsigned char *WriteBuffer,
                                     unsigned char DataLent)
  ����:	���ֽ�д����
  �������: Ҫд�ĵ�ַ��Ҫд������ָ�룬Ҫд���ֽ���        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char HardWareIIC_MultiWrite(unsigned char WriteAddr,
                                     unsigned char *WriteBuffer,
                                     unsigned char DataLent)
{
    unsigned char try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
      
    if( HardWareIIC_WriteInit( WriteAddr ) )
    {
        for(unsigned int i=0;i<DataLent;i++)
        {
            try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
            while((UCB3IFG & UCTXIFG)==0)             // �ж��Ƿ������ 
            {
                if(!try_time--)
                {
                    return 0;
                }
            }
            UCB3TXBUF = *WriteBuffer++;                 // ��������
        }
        try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
        while((UCB3IFG & UCTXIFG)==0)             // �ж��Ƿ������ 
        {
            if(!try_time--)
            {
                return 0;
            }
        }
        
        UCB3CTL1 |= UCTXSTP;                      // ����ֹͣλ
        try_time=HardWaredefine_try_time; //�����������ͨѶ����ʱ���ԵĴ���
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