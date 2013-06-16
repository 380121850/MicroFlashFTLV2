#ifndef  __SoftWareSPI_Module_H
#define  __SoftWareSPI_Module_H

//ͨѶ�ӿ� SI,SO,SCK
#define SoftWareSPI_SI_BIT     BIT1    //Ҫ��  1��CE����Ϊ���IO��
#define SoftWareSPI_SO_BIT     BIT2    //      2��HOLDΪ���IO��
#define SoftWareSPI_SCK_BIT    BIT3    //      2��WPΪ���IO��

//�ӿڼĴ���
#define SoftWareSPI_DIR_Port  P3DIR    //      3����ʹ�õ�Դ���ƣ���VCC����Ϊ���IO��
#define SoftWareSPI_OUT_Port  P3OUT
#define SoftWareSPI_IN_Port   P3IN

#define SoftWareSPI_SI_OUT   SoftWareSPI_DIR_Port |=SoftWareSPI_SI_BIT;
#define SoftWareSPI_SI_IN    SoftWareSPI_DIR_Port &=~SoftWareSPI_SI_BIT;
#define SoftWareSPI_SI_H     SoftWareSPI_OUT_Port |= SoftWareSPI_SI_BIT;
#define SoftWareSPI_SI_L     SoftWareSPI_OUT_Port &=~SoftWareSPI_SI_BIT;

#define SoftWareSPI_SO_OUT   SoftWareSPI_DIR_Port |=SoftWareSPI_SO_BIT;
#define SoftWareSPI_SO_IN    SoftWareSPI_DIR_Port &=~SoftWareSPI_SO_BIT;
#define SoftWareSPI_SO_Read  (SoftWareSPI_IN_Port & SoftWareSPI_SO_BIT)
#define SoftWareSPI_SO_H     SoftWareSPI_OUT_Port |= SoftWareSPI_SO_BIT;
#define SoftWareSPI_SO_L     SoftWareSPI_OUT_Port &=~SoftWareSPI_SO_BIT;

#define SoftWareSPI_SCK_OUT   SoftWareSPI_DIR_Port |=SoftWareSPI_SCK_BIT;
#define SoftWareSPI_SCK_IN    SoftWareSPI_DIR_Port &=~SoftWareSPI_SCK_BIT;
#define SoftWareSPI_SCK_H     SoftWareSPI_OUT_Port |= SoftWareSPI_SCK_BIT;
#define SoftWareSPI_SCK_L     SoftWareSPI_OUT_Port &=~SoftWareSPI_SCK_BIT;


/******************************************************************************
  ����(ģ��)����:void SoftWareSPI_Init(void)
  ����:	SPI���߷���һ���ֽ�����
  �������:���͵�����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/ 
void SoftWareSPI_Init(void);

/******************************************************************************
  ����(ģ��)����:void SoftWareSPI_Send_Byte(unsigned char Send_Data)
  ����:	SPI���߷���һ���ֽ�����
  �������:���͵�����         	              		
  �������: ��         	 		   		 
  ����˵��: 
*******************************************************************************/ 
void SoftWareSPI_Send_Byte(unsigned char Send_Data);

/******************************************************************************
  ����(ģ��)����:void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڷ��ͺ���
  �������:��Ҫ���͵����ݵĵ�ַָ�룬���ݳ���         	              		
  �������: ��         	 		   		 
  ����˵��: ֻ��������ݷ��ͳ�ȥ
*******************************************************************************/
void SoftWareSPI_Send_Mass(unsigned char *Data_point,unsigned int Data_Lent);

/******************************************************************************
  ����(ģ��)����:unsigned char SPI_Recei_Byte(void)
  ����:	SPI���߽���һ���ֽ�����
  �������:  ��        	              		
  �������:  ���յ�������       	 		   		 
  ����˵��: 
*******************************************************************************/ 
unsigned char SoftWareSPI_Recei_Byte(void);

/******************************************************************************
  ����(ģ��)����:void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned char Data_Lent)
  ����:	SPI���ֽڽ��պ���
  �������:��ŵĵ�ַָ�룬�Լ���Ҫ���յ����ݳ���         	              		
  �������: ���յ����ֽ�����         	 		   		 
  ����˵��: ֻ�����ȡ����
*******************************************************************************/
void SoftWareSPI_Recei_Mass(unsigned char *Data_point,unsigned int Data_Lent);


#endif