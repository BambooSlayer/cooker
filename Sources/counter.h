#ifndef _COUNTER_H
#define _COUNTER_H

#include "inc.h"


void	Counter_Init(void );
void  SpeedGet(void);

struct Coderdata 
{
  unsigned int spdl;
  unsigned int spdr;
};

typedef struct Coderdata  MY_Coderdata ;





#endif
