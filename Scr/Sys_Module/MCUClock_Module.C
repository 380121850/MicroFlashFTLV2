/*************************************************
** ��Ȩ:   
** ����:   ACLK = XT1 , MCLK = ~8MHz,SMCLK = ~8MHZ,DCO�ο�Ƶ������XT1
//               MSP430F5438
//              -----------------
//         /|\ |             XIN |-
//          |  |                 |
//          -- |RST          XOUT|-
//             |                 |
** �޸���־   
** ���˵��:   UCS������Ҫע��һ�·��棺
             1.��Ĭ��״̬��XT1�ǹر�״̬�ģ����ģ��ʱ��ѡ��XT1�ṩʱ��Դ����ôϵͳ�ڲ�ʱ��
               REFO�ͻ���ʱ���XT1�ṩʱ�ӣ�����REFO���Ⱥ��ȶ��Ա�XT1Ҫ����Ҫ��Ƚϸ߳�������
              ʹ��XT1.�����XIN,XOUT ��������Ϊ�������ţ���ô����ͻ�����
             2.DCORSELX�����������õ�Ƶ��Ҫ����ͳһ�������ͳһ�Ϳ��ܳ���OFIFGΪ1������Ƶ����ֵ�ﲻ����
               ��Ҫ��Ƶ��ֵ��С
             3.XT1DRIVE1 ������Ϊ0��ʱ���������������XT1ʧЧ��XT1ֹͣ��
*********************************************************************************/
#include "msp430x54x.h"
#include "Sys_Module.H"

unsigned long CurrenClock = 1048576;//��ǰʱ��
/*************************************************
  ����(ģ��)����:void Init_Clk(void)
  ����:	ʱ�ӳ�ʼ��
  �������:         	              		
  �������:      				
  ����˵��:       		
*************************************************/
void Init_Clk(void)
{
    static unsigned long TryTime = 200000;
    //P11DIR = BIT0 + BIT1 + BIT2;                    // ACLK ,MCLK ,MCLK  �������
    //P11SEL = BIT0 + BIT1 + BIT2;    
      
    P7SEL |= 0x03;                                   // ����DCO��Χ  
     
    UCSCTL1 = DCORSEL_4;                             // ѡ��FLL�ο�Դ��REFO  
    UCSCTL2  = 0x79 + FLLD0;                         // Set DCO Multiplier for 8MHz
    UCSCTL4 = SELM_3 + SELA_0 + SELS_3;              // ���� MCLK = DCOC,SMCLK =DCOC,ACLK=XT1
    
    CurrenClock = 8000000; //��ǰʱ�� 
    while ( SFRIFG1 & OFIFG )                        // ���OFIFG,and  XT1OFFG ,DCOFFG
    {
        UCSCTL7 &= ~(  XT1LFOFFG + DCOFFG);
        SFRIFG1 &= ~OFIFG;
        if(0 == TryTime-- )
        {
            CurrenClock = 1048576; //��ǰʱ��
            break;
        }
    } 
}