#include "inc.h"
#include "mydelay.h"

void Delay_ms(uint z)     //1ms
{ 
  uint x; 
  while(z--)
  {
    
    for(x=0;x<1455;x++)
      {
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
        _asm(nop);  _asm(nop);  _asm(nop);  _asm(nop);
       }
  }

}