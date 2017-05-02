#include "clock.h"
 
void chaopin(void)//64M--bus,128M--core
{  
    CLKSEL=0X00;   
    PLLCTL_PLLON=1;   
    SYNR =0xc0 | 0x07;                        
    REFDV=0xc0 | 0x01; 
    POSTDIV=0x00;       
    _asm(nop);          
    _asm(nop);
    _asm(nop);
    _asm(nop);
    while(!(CRGFLG_LOCK==1));   
    CLKSEL_PLLSEL =1;          		        
}
