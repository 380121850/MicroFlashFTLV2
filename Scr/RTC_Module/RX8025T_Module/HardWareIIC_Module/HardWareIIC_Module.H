#ifndef  __HardWareIIC_Module_H
#define  __HardWareIIC_Module_H


#define HardWaredefine_try_time     128   //����������ʱ�����Դ�������ֹ������ѭ��

/******************************************************************************
  ����(ģ��)����:void HardWareIIC_Init(unsigned char Slave_Addr)
  ����:	IIC��ʼ���������������ôӻ���ַ
  �������: �ӻ���ַ        	              		
  �������: ��          	 		   		 
  ����˵��: ����Ĵӻ���ַ����Ҫ����һλ�ٷ����ַ�Ĵ���
*******************************************************************************/
void HardWareIIC_Init(unsigned char Slave_Addr);

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Single_Read(unsigned int R_addr,unsigned char *R_word)
  ����:	���ζ�/�����
  �������: Ҫ�������ݵ�ַ��������ݱ�����ֽ�ָ��        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char HardWareIIC_SingleRead(unsigned char ReadAddr,unsigned char *ReadData);

/******************************************************************************
  ����(ģ��)����:unsigned char IIC_Multi_read(unsigned int R_addr,unsigned char *Save_addr,
                                unsigned int R_lent)
  ����:	������
  �������: ����ʼ��ַ�����ݱ���ָ�룬������        	              		
  �������: 1 ���ɹ� 0 ��ʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char HardWareIIC_MultiRead(unsigned char ReadAddr,unsigned char *ReadBuff,
                                unsigned int ReadLent);

/******************************************************************************
  ����(ģ��)����:unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,
                                                       unsigned char WriteData)
  ����:	IIC���ߵ��ֽ�д
  �������: Ҫд�ĵ�ַ��Ҫд������        	              		
  �������: 1 д�ɹ� 0 дʧ��         	 		   		 
  ����˵��: 
*******************************************************************************/
unsigned char HardWareIIC_SingleWrite(unsigned char WriteAddr,unsigned char WriteData);

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
                                     unsigned char DataLent);

#endif