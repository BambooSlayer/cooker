#ifndef _AD_H
#define _AD_H

#include "inc.h"
#include "mydelay.h"

struct AD_data 
{
  unsigned int max;
  unsigned int min;
  unsigned int AD_res;
  unsigned long last;
};
typedef struct AD_data MY_AD;
extern MY_AD ad_last[6];

extern uchar left_minus_flag ;
extern uchar right_minus_flag ;
//extern unsigned int  dianji1,dianji2,dianji3,dianji4;//���ֵ4096   AD��������
//extern unsigned int dianci1,dianci2,dianci3,dianci4,dianci5,dianci6;

void AD_start(void);//��·AD���AD0-AD5
void AD_get(uchar ch_s,uchar ch_f);
void Init_AD(void);
//void AD_get_n(uchar n); //��·��ż��    
//void atd_init(void);
//void AD_get1(void);

#endif