#ifndef _STR_H
#define _STR_H

#include"inc.h"

#define uchar unsigned char 
#define uint unsigned int  
#define ulong unsigned long 

void Speedset_openloop(void);
void douji_a(char a);
void Speedset_closedloop(void);
void SpeedPID_Init();

void PD_duoji(void);
void PID_duoji_init(void);
void PID_now(void) ;
void PID_douji_para(void);

void  analyse( uchar num);

void pre_judge();

typedef struct motor
{
  unsigned char v_respect;
  float Last_error;
  float Pre_error;
  unsigned int *curr_spd;
  float error_value;
  float Kp;
  float Ki;
  float Kd;
  
  float K_spd_e;
  
} MOTOR ;



#endif