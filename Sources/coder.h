#ifndef _CODER_H
#define _CODER_H

#include "inc.h"


void	TIM_Init( void );
void speed_get(unsigned char channel);

struct Coder_data 
{
  unsigned int spd1;
  unsigned int spd2;
};

typedef struct Coder_data  MY_Coder_data ;
extern MY_Coder_data MY_Coder;




#endif
