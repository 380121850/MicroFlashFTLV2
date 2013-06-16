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
#include "IIC_Module.H"
static unsigned char Slave_Addres=0x64;
/******************************************************************************
  ����(ģ��)����:void Delay(unsigned int m)
  ����:	I2C������ʱ����
  �������: ��ʱ��        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
void Delay(unsigned int m)
{
   while(m-- > 0);
}

/******************************************************************************
  ����(ģ��)����:void IIC_Init_Port(void)
  ����:	IIC��������
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: �ں������������˹���
*******************************************************************************/
void IIC_Init(unsigned char Slave_Addr)
{
    Slave_Addres=Slave_Addr;
    IIC_SCL_L;
    IIC_SCL_DIR_OUT;
    IIC_SDA_H;
    IIC_SDA_DIR_OUT;  
    IIC_SCL_H;       //��ʱSDA=1,SCL=1
    Delay(100);     //�ϵ����Ҫ�ʵ���ʱ�ȴ�IIC׼������
}

/******************************************************************************
  ����(ģ��)����:void IIC_Start(void)
  ����:	���������ź�
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: SCL���ڸߵ�ƽ�ڼ䣬��SDA�����½���ʱ����I2C����
            ������Ҳ���������ظ���ʼ����
*******************************************************************************/
void IIC_Start(void)
{
    IIC_SDA_DIR_OUT; //SDA�����״̬
    IIC_SCL_L;
    IIC_SCL_DIR_OUT;
    IIC_SDA_H;
    IIC_SDA_DIR_OUT;  
    IIC_SCL_H;       //��ʱSDA=1,SCL=1
    Delay(1);
    IIC_SDA_L;
    Delay(1);
    IIC_SCL_L;
}

/******************************************************************************
  ����(ģ��)����:void IIC_Stop(void)
  ����:	����ֹͣ�ź�
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
void IIC_Stop(void)
{
    IIC_SDA_DIR_OUT; //SDA�����״̬
    IIC_SCL_L;
    Delay(1);
    IIC_SDA_L;
    Delay(1);
    IIC_SCL_H;
    Delay(1);
    IIC_SDA_H; 
}

/******************************************************************************
  ����(ģ��)����:void ReadAck(void)
  ����:	��Ӧ���ź�
  �������: ��        	              		
  �������: Ӧ���źŻ��Ӧ���ź�          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char ReadAck(void)
{
    unsigned char Ack_Flag = 0;
    IIC_SCL_L;
    IIC_SDA_DIR_IN; //SDA������״̬
    Delay(2);
    IIC_SCL_H;
    if ((IIC_IN_Port & IIC_SDA_BIT))
    {
      Ack_Flag = 1;
    }
    IIC_SCL_L;
    IIC_SDA_DIR_OUT; //SDA�����״̬
    IIC_SDA_H;
    return   Ack_Flag;
}

/******************************************************************************
  ����(ģ��)����:void WriteAck(unsigned char ACK)
  ����:	���Ӧ����Ӧ���ź�
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
void WriteAck(unsigned char ACK)
{
    if(ACK)  //�����Ӧ�� 1
    {
        IIC_SDA_DIR_OUT; //SDA�����״̬
        IIC_SDA_H;
        Delay(2);
        IIC_SCL_H;
        Delay(2);
        IIC_SCL_L;
        //IIC_SDA_DIR_IN; //SDA������״̬
    }
    else   //���Ӧ�� 0
    {
        IIC_SDA_DIR_OUT; //SDA�����״̬  
        IIC_SDA_L;
        Delay(2);
        IIC_SCL_H;
        Delay(2);
        IIC_SCL_L;
        //IIC_SDA_DIR_IN; //SDA������״̬
    }
}



/******************************************************************************
  ����(ģ��)����:void IIC_Send_Byte(unsigned char Date_Byte)
  ����:	��I2C���߷���һ������
  �������: Ҫ���͵��ֽ�        	              		
  �������: ��          	 		   		 
  ����˵��: 
*******************************************************************************/
void IIC_Send_Byte(unsigned char Date_Byte)
{
    unsigned int i;
    for (i=0;i<8;i++)
    {
      if(Date_Byte & 0x80)
      {
        IIC_SDA_H;
      }
      else
      {
        IIC_SDA_L;
      }
      IIC_SCL_H;
      Delay(2);
      IIC_SCL_L;
      Date_Byte = Date_Byte << 1;
    } 
}

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Receive_Byte(void)
  ����:	��I2C���߽���һ���ֽ�
  �������: ��        	              		
  �������: ���յ����ֽ�          	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IIC_Receive_Byte(void)
{
    unsigned int i;
    unsigned char R_word = 0x00;
    IIC_SDA_DIR_IN;
    for(i=0;i<8;i++)
    {
      Delay(5);
      R_word = R_word << 1;
      IIC_SCL_H;
      if(IIC_IN_Port & IIC_SDA_BIT)
      {
        R_word |= 0x01;
      }
      Delay(2);
      IIC_SCL_L;
     }
     IIC_SDA_DIR_OUT;
     return  R_word;
}

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  ����:	���ζ�/�����
  �������: Ҫ�������ݵ�ַ��������ݱ�����ֽ�ָ��        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IIC_Single_Read(unsigned char R_addr,unsigned char *R_word)
{
    unsigned char try_time=define_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    while(1)
    {
        if(!try_time--)    //������ֹ��Ӳ�����������ԭ����������ѭ��
        {
           return 0;
        }
        IIC_Start();       //����1������һ�������ź�
        IIC_Send_Byte(Slave_Addres&(~BIT0));   //����2�����Ϳ�������
        if(ReadAck())
          continue;        //û��Ӧ�𣬺���������䣬���¿�ʼ
                           //Ӧ����ʼ���͵�ַ
        IIC_Send_Byte(R_addr);
        if(ReadAck())         
          continue;        //û��Ӧ�𣬺���������䣬���¿�ʼ
                           //Ӧ����ʼ���������ź�
        IIC_Start();       //�����ź�
        IIC_Send_Byte(Slave_Addres|BIT0);   //���Ϳ�������
        if(ReadAck())         
          continue;        //û��Ӧ�𣬺���������䣬���¿�ʼ
        break;             //Ӧ��������ѭ������ʼ��������
    }
    *R_word=IIC_Receive_Byte(); //�������Ͻ���һ���ֽ�����
    WriteAck(1);               //���� ��Ӧ���ź�
    IIC_Stop();                //����ֹͣ�ź�
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  ����:	������
  �������: ����ʼ��ַ�����ݱ���ָ�룬������        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IIC_Multi_read(unsigned char R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
{
    unsigned char try_time=define_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    while(1)
    {
        if(!try_time--)    //������ֹ��Ӳ�����������ԭ����������ѭ��
        {
           return 0;
        }
        IIC_Start();           //����1������һ�������ź�
        IIC_Send_Byte(Slave_Addres&(~BIT0));       //����2�����Ϳ�������
        if(ReadAck())
          continue;        //û��Ӧ�𣬺���������䣬���¿�ʼ
                           //Ӧ����ʼ���͵�ַ
        IIC_Send_Byte(R_addr);
        if(ReadAck())         
          continue;        //û��Ӧ�𣬺���������䣬���¿�ʼ
                           //Ӧ����ʼ���������ź�
        IIC_Start();       //�����ź�
        IIC_Send_Byte(Slave_Addres|BIT0);   //���Ϳ�������
        if(ReadAck())         
          continue;        //û��Ӧ�𣬺���������䣬���¿�ʼ
        break;             //Ӧ��������ѭ������ʼ��������
    }
    while(R_lent--)
    {
        *Save_addr++=IIC_Receive_Byte(); //�������Ͻ���һ���ֽ�����
        if(R_lent==0)
        {
            WriteAck(1);               //���� ��Ӧ���ź�
            IIC_Stop();                //����ֹͣ�ź�
            break;
        }
        WriteAck(0);               //����Ӧ���ź�
    }
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Single_Write(unsigned char W_addr,unsigned char Date_byte)
  ����:	IIC���ߵ��ֽ�д
  �������: Ҫд�ĵ�ַ��Ҫд������        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IIC_Single_Write(unsigned char W_addr,unsigned char Date_byte)
{
    unsigned char try_time=define_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    while(1)
    {
        if(!try_time--)    //������ֹ��Ӳ�����������ԭ����������ѭ��
        {
           return 0;
        }
        IIC_Start();                           //����1����������һ�������ź�
        IIC_Send_Byte(Slave_Addres&(~BIT0));   //����2���������ʹӻ���ַ����дλ
        if(ReadAck())
          continue;        //����3���������մӻ�Ӧ��û��Ӧ�𣬺���������䣬���¿�ʼ
                           //Ӧ����ʼ���͵�ַ
        IIC_Send_Byte(W_addr);     //����4���������Ͷ�д��ַ
        if(ReadAck())         
          continue;               //����5���������մӻ�Ӧ�� û��Ӧ�𣬺���������䣬���¿�ʼ
        IIC_Send_Byte(Date_byte); //����6�������������� //Ӧ����ʼ����Ҫд�������
        if(ReadAck())     //����7�������մӻ�Ӧ��
          continue;      //û��Ӧ�𣬺���������䣬���¿�ʼ
        break;           //Ӧ��������ѭ��
    }
    IIC_Stop();          //����8����������ֹͣ�ź�,����д����
    Delay(5000);         //��Ҫ��ʱһ��ʱ�䣬�ȴ�д�������
    return 1;
}

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Page_Write(unsigned char W_addr,
                                              unsigned char *WriteData_Addr,
                                              unsigned char WriteData_Cnt)
  ����:	IICҳд
  �������: Ҫд�ĵ�ַ��Ҫд������ָ�룬Ҫд���ֽ���        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: Ҫд���ֽ��������ܴ��ڵ�ǰд�ֽڵ�ַ����ǰҳ����ַ֮�
*******************************************************************************/
unsigned char IIC_Page_Write(unsigned char W_addr,unsigned char *WriteData_Addr,
                             unsigned char WriteData_Cnt)
{
    unsigned char try_time=define_try_time; //�����������ͨѶ����ʱ���ԵĴ���
    while(1)
    {
        if(!try_time--)    //������ֹ��Ӳ�����������ԭ����������ѭ��
        {
           return 0;
        }
        IIC_Start();                           //����1����������һ�������ź�
        IIC_Send_Byte(Slave_Addres&(~BIT0));   //����2���������ʹӻ���ַ����дλ
        if(ReadAck())
          continue;        //����3���������մӻ�Ӧ��û��Ӧ�𣬺���������䣬���¿�ʼ
                           //Ӧ����ʼ���͵�ַ
        IIC_Send_Byte(W_addr);     //����4���������Ͷ�д��ַ
        if(ReadAck())         
          continue;               //����5���������մӻ�Ӧ�� û��Ӧ�𣬺���������䣬���¿�ʼ
        while(WriteData_Cnt--)
        {
            IIC_Send_Byte(*(WriteData_Addr++));
            if(!ReadAck())  
              continue;   //�յ�Ӧ��(0)���������������   
            return 0;     //û�յ�Ӧ�𣬴��󷵻�
        }
        break;
    }   
    IIC_Stop();          //����8����������ֹͣ�ź�,����д����
    Delay(5000);         //��Ҫ��ʱһ��ʱ�䣬�ȴ�д�������
    return 1;
}