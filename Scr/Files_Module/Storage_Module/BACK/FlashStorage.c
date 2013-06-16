//************************************************************************
//
//  Lierda, Inc                      SST25VF 
//            
//************************************************************************
#include "msp430x54x.h"
#include "FlashStorage.H"

    
unsigned char FlashCheckByte(unsigned long FlashMaxAddr,unsigned char CheckByte,
                             unsigned int CheckMod)
{
    unsigned long FlashAddr;
    if(CheckMod)
      FlashAddr=0;
    else
      FlashAddr=FlashMaxAddr;
    
    for(unsigned long i=0;i<FlashMaxAddr;i++)
    {
        if( FlashModule_ReadByte(FlashAddr) == CheckByte )
        {
            if(CheckMod)
              FlashAddr++;
            else
              FlashAddr--;
        }
        else  return 0;
    }
    return 1;
}



unsigned char FlashWrite00(unsigned long FlashMaxAddr,unsigned int CheckMod)
{
    unsigned long FlashAddr;
    if(CheckMod)
      FlashAddr=0;
    else
      FlashAddr=FlashMaxAddr;
    
    for(unsigned long i=0;i<FlashMaxAddr;i++)
    {
        FlashModule_WriteByte(FlashAddr,0x00);;
        if(CheckMod)
          FlashAddr++;
        else
          FlashAddr--;
    }
    return 1;
}


unsigned char FlashTest(unsigned long FlashMaxSize)
{
    FlashModule_Init();
    FlashModule_ChipErase();//Ð¾Æ¬²Á³ý
    if( FlashCheckByte(FlashMaxSize,0xff,1) )
    {
        FlashWrite00(FlashMaxSize,1);
        if( FlashCheckByte(FlashMaxSize,0x00,1) )
          return 1;
        else 
          return 0;
    }
    else return 0;
}