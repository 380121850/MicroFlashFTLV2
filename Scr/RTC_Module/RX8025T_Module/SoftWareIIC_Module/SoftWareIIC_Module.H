#ifndef  __SoftWareIIC_Module_H
#define  __SoftWareIIC_Module_H


#define Softdefine_try_time     3   //����������ʱ�����Դ�������ֹ������ѭ��

#define SoftWareIIC_SCL_BIT   BIT2    //Ҫ��  1��SCL����Ϊ���IO��
#define SoftWareIIC_SDA_BIT   BIT1    //      2��SDAΪ�������IO��
#define SoftWareIIC_DIR_Port  P10DIR  //      3����ʹ�õ�Դ���ƣ���VCC����Ϊ���IO��
#define SoftWareIIC_OUT_Port  P10OUT
#define SoftWareIIC_IN_Port   P10IN

#define SoftWareIIC_SCL_DIR_IN   SoftWareIIC_DIR_Port &= ~SoftWareIIC_SCL_BIT;
#define SoftWareIIC_SCL_DIR_OUT  SoftWareIIC_DIR_Port |= SoftWareIIC_SCL_BIT;

#define SoftWareIIC_SDA_DIR_IN   SoftWareIIC_DIR_Port &= ~SoftWareIIC_SDA_BIT;
#define SoftWareIIC_SDA_DIR_OUT  SoftWareIIC_DIR_Port |= SoftWareIIC_SDA_BIT;

#define SoftWareIIC_SCL_H     SoftWareIIC_OUT_Port |= SoftWareIIC_SCL_BIT;
#define SoftWareIIC_SCL_L     SoftWareIIC_OUT_Port &= ~SoftWareIIC_SCL_BIT;

#define SoftWareIIC_SDA_H     SoftWareIIC_OUT_Port |= SoftWareIIC_SDA_BIT;
#define SoftWareIIC_SDA_L     SoftWareIIC_OUT_Port &= ~SoftWareIIC_SDA_BIT;

static unsigned char Slave_Addres;

/******************************************************************************
  ����(ģ��)����:void SoftWareIIC_Init_Port(void)
  ����:	IIC��������
  �������: ��        	              		
  �������: ��          	 		   		 
  ����˵��: �ں������������˹���
*******************************************************************************/
void SoftWareIIC_Init(unsigned char Slave_Addr);

/******************************************************************************
  ����(ģ��)����:unsigned char SoftWareIIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  ����:	���ζ�/�����
  �������: Ҫ�������ݵ�ַ��������ݱ�����ֽ�ָ��        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SoftWareIIC_Single_Read(unsigned char R_addr,unsigned char *R_word);

/******************************************************************************
  ����(ģ��)����:unsigned char SoftWareIIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  ����:	������
  �������: ����ʼ��ַ�����ݱ���ָ�룬������        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SoftWareIIC_Multi_read(unsigned char R_addr,unsigned char *Save_addr,
                                unsigned int R_lent);

/******************************************************************************
  ����(ģ��)����:unsigned char SoftWareIIC_Single_Write(unsigned char W_addr,unsigned char Date_byte)
  ����:	IIC���ߵ��ֽ�д
  �������: Ҫд�ĵ�ַ��Ҫд������        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char SoftWareIIC_Single_Write(unsigned char W_addr,unsigned char Date_byte);

/******************************************************************************
  ����(ģ��)����:unsigned char SoftWareIIC_Page_Write(unsigned char W_addr,
                                              unsigned char *WriteData_Addr,
                                              unsigned char WriteData_Cnt)
  ����:	IICҳд
  �������: Ҫд�ĵ�ַ��Ҫд������ָ�룬Ҫд���ֽ���        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: Ҫд���ֽ��������ܴ��ڵ�ǰд�ֽڵ�ַ����ǰҳ����ַ֮�
*******************************************************************************/
unsigned char SoftWareIIC_Page_Write(unsigned char W_addr,unsigned char *WriteData_Addr,
                             unsigned char WriteData_Cnt);

#endif