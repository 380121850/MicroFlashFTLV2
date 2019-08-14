/*************************************************
** 版权:   
** 功能:   ACLK = XT1 , MCLK = ~8MHz,SMCLK = ~8MHZ,DCO参考频率来自XT1
//               MSP430F5438
//              -----------------
//         /|\ |             XIN |-
//          |  |                 |
//          -- |RST          XOUT|-
//             |                 |
** 修改日志   
** 相关说明:   UCS配置需要注意一下方面：
             1.在默认状态下XT1是关闭状态的，如果模块时钟选择XT1提供时钟源，那么系统内部时钟
               REFO就会暂时替代XT1提供时钟，但是REFO精度和稳定性比XT1要差，因此要求比较高场所建议
              使用XT1.如果将XIN,XOUT 引脚设置为特殊引脚，那么晶体就会起振。
             2.DCORSELX的配置与设置的频率要保持统一，如果不统一就可能出现OFIFG为1，或者频率数值达不到所
               需要的频率值大小
             3.XT1DRIVE1 设置在为0的时候低驱动能力，将XT1失效，XT1停止振荡
*********************************************************************************/
#include "msp430x54x.h"
#include "Sys_Module.H"

unsigned long CurrenClock = 1048576;//当前时钟
/*************************************************
  函数(模块)名称:void Init_Clk(void)
  功能:	时钟初始化
  输入参数:         	              		
  输出参数:      				
  其它说明:       		
*************************************************/
void Init_Clk(void)
{
    static unsigned long TryTime = 200000;
    //P11DIR = BIT0 + BIT1 + BIT2;                    // ACLK ,MCLK ,MCLK  输出方向
    //P11SEL = BIT0 + BIT1 + BIT2;    
      
    P7SEL |= 0x03;                                   // 设置DCO范围  
     
    UCSCTL1 = DCORSEL_4;                             // 选择FLL参考源自REFO  
    UCSCTL2  = 0x79 + FLLD0;                         // Set DCO Multiplier for 8MHz
    UCSCTL4 = SELM_3 + SELA_0 + SELS_3;              // 配置 MCLK = DCOC,SMCLK =DCOC,ACLK=XT1
    
    CurrenClock = 8000000; //当前时钟 
    while ( SFRIFG1 & OFIFG )                        // 清除OFIFG,and  XT1OFFG ,DCOFFG
    {
        UCSCTL7 &= ~(  XT1LFOFFG + DCOFFG);
        SFRIFG1 &= ~OFIFG;
        if(0 == TryTime-- )
        {
            CurrenClock = 1048576; //当前时钟
            break;
        }
    } 
}