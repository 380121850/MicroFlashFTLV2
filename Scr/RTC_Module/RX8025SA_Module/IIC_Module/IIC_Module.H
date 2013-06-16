#ifndef  __IIC_Module_H
#define  __IIC_Module_H


#define define_try_time     3   //����������ʱ�����Դ�������ֹ������ѭ��

#define IIC_SCL_BIT   BIT2    //Ҫ��  1��SCL����Ϊ���IO��
#define IIC_SDA_BIT   BIT1    //      2��SDAΪ�������IO��
#define IIC_DIR_Port  P10DIR  //      3����ʹ�õ�Դ���ƣ���VCC����Ϊ���IO��
#define IIC_OUT_Port  P10OUT
#define IIC_IN_Port   P10IN

#define IIC_SCL_DIR_IN   IIC_DIR_Port &= ~IIC_SCL_BIT;
#define IIC_SCL_DIR_OUT  IIC_DIR_Port |= IIC_SCL_BIT;

#define IIC_SDA_DIR_IN   IIC_DIR_Port &= ~IIC_SDA_BIT;
#define IIC_SDA_DIR_OUT  IIC_DIR_Port |= IIC_SDA_BIT;

#define IIC_SCL_H     IIC_OUT_Port |= IIC_SCL_BIT;
#define IIC_SCL_L     IIC_OUT_Port &= ~IIC_SCL_BIT;

#define IIC_SDA_H     IIC_OUT_Port |= IIC_SDA_BIT;
#define IIC_SDA_L     IIC_OUT_Port &= ~IIC_SDA_BIT;

static unsigned char Slave_Addres;

/******************************************************************************
  ����(ģ��)����:void IIC_Init_Port(void)
  ����:	IIC��������
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: �ں������������˹���
*******************************************************************************/
void IIC_Init(unsigned char Slave_Addr);

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  ����:	���ζ�/�����
  �������: Ҫ�������ݵ�ַ��������ݱ�����ֽ�ָ��        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IIC_Single_Read(unsigned char R_addr,unsigned char *R_word);

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  ����:	������
  �������: ����ʼ��ַ�����ݱ���ָ�룬������        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IIC_Multi_read(unsigned char R_addr,unsigned char *Save_addr,
                                unsigned int R_lent);

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Single_Write(unsigned char W_addr,unsigned char Date_byte)
  ����:	IIC���ߵ��ֽ�д
  �������: Ҫд�ĵ�ַ��Ҫд������        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char IIC_Single_Write(unsigned char W_addr,unsigned char Date_byte);

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
                             unsigned char WriteData_Cnt);

#endif