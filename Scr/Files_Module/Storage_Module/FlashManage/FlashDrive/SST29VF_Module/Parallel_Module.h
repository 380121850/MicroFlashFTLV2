#ifndef  __Parallel_Module_H
#define  __Parallel_Module_H

#define DataPort_DIR  P4DIR
#define DataPort_OUT  P4OUT
#define DataPort_IN   P4IN
#define DataPort_DIR_IN   DataPort_DIR &= ~(BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);
#define DataPort_DIR_OUT  DataPort_DIR |=  (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

#define AddrPort0_DIR  P8DIR
#define AddrPort0_OUT  P8OUT
#define AddrPort0_IN   P8IN
#define AddrPort0_DIR_OUT   AddrPort0_DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

#define AddrPort1_DIR  P6DIR
#define AddrPort1_OUT  P6OUT
#define AddrPort1_IN   P6IN
#define AddrPort1_DIR_OUT   AddrPort1_DIR |= (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7);

#define AddrPort2_DIR  P7DIR
#define AddrPort2_OUT  P7OUT
#define AddrPort2_IN   P7IN
#define AddrPort2_DIR_OUT   AddrPort2_DIR |= (BIT5+BIT6+BIT7);


void Parallel_Init(void);
void WrteDataPort(unsigned char WriteData);
unsigned char ReadDataPort(void);
void WrteAddrPort(unsigned long WriteData);

#endif