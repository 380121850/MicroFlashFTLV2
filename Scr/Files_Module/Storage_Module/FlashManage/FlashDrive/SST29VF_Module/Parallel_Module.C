//************************************************************************
//
//  Lierda, Inc                      SST25VF010 
//            
//************************************************************************
#include "msp430x54x.h"
#include "Parallel_Module.h"

void Parallel_Init(void)
{   
    DataPort_DIR_IN;
    
    AddrPort0_DIR_OUT;
    AddrPort1_DIR_OUT;
    AddrPort2_DIR_OUT;
}

void WrteDataPort(unsigned char WriteData)
{
    DataPort_DIR_OUT;
    DataPort_OUT = WriteData;
}

unsigned char ReadDataPort(void)
{
    DataPort_DIR_IN;
    return DataPort_IN;
}


// ---------------------------------------------------------
// 方法2：按位直接映射
// 优点：直观，高效
// 缺点：
// ---------------------------------------------------------
static unsigned char ByteInvert2(unsigned char chSrc )
{ 
     unsigned char chDst=0;
     
     if ( chSrc & 0x80 ) chDst |= 0x01; 
     if ( chSrc & 0x40 ) chDst |= 0x02; 
     if ( chSrc & 0x20 ) chDst |= 0x04;
     if ( chSrc & 0x10 ) chDst |= 0x08;
     if ( chSrc & 0x08 ) chDst |= 0x10;
     if ( chSrc & 0x04 ) chDst |= 0x20; 
     if ( chSrc & 0x02 ) chDst |= 0x40; 
     if ( chSrc & 0x01 ) chDst |= 0x80;
     
     return(chDst);
} 


void WrteAddrPort(unsigned long WriteData)
{
    AddrPort0_OUT = ByteInvert2( (unsigned char) WriteData );
    
    WriteData = WriteData>>8;
    AddrPort1_OUT = (unsigned char) WriteData;
    
    WriteData = WriteData>>8;
    AddrPort2_OUT &= ~(BIT5+BIT6+BIT7);
    AddrPort2_OUT |= ((unsigned char) WriteData)&(BIT5+BIT6+BIT7);
}