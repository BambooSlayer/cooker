#ifndef INC_H
#define INC_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#define	BSET(bit,Register) ((Register)|=(1<<(bit))) // set
#define	BCLR(bit,Register) ((Register)&=~(1<<(bit))) // clear
#define	BGET(bit,Register) ((Register)>>(bit)&1) // get


#define N 0//PIT_counter

#define Max_Tab 32//×î´óµµÎ»
#define V_Max  250 //<250

#endif