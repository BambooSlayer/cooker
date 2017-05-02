#include "coder.h"

unsigned int ant_speed1;
unsigned int ant_speed2;
unsigned char unfinished1 =0;
unsigned char unfinished2 =0;
MY_Coder_data MY_Coder;

const unsigned char death_zone=0 ;
 

void Delay(uint z)     // 约为0.5ms
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

void	TIM_Init( void )
{
	TIOS	=	0x00;		//all input cap enable
	TCTL4	=	0x0a;		//active edge on channel 0 channel 1 is FALLING edge
	TSCR2	=	0x03;		//TIMEclock = BUSclock / 8
	TFLG1	=	0xFF;		//clear all flags
	TFLG2	=	0x80;		//clear TOF
//	TIE		=	0x01;		//enable channel0 INT
	TSCR1	=	0xA0;		//ENABLE TIM,FREEZE MODE STOP,DON'T FAST FLAG CLEAR	
}

void TIM_start1()
{
//	TIM_Init();
   	TIE		=	0x01;		//enable channel0 INT
//		TFLG1	=	0xFF;		//clear all flags
}

void TIM_off()
{
    TIE		=	0x00;
  	TFLG1	=	0xFF;		//clear all flags
//  TSCR1	=	0x00;
}

void TIM_start2()
{
//	TIM_Init();
  	TIE		=	0x02;		//enable channel1 INT
//		TFLG1	=	0xFF;		//clear all flags
}


//unsigned int speed_get(unsigned char channel)
void speed_get(unsigned char channel)
{
  if( channel ==1)
  {
    TIM_start1();
    Delay(1);  // 如果脉冲频率小于8K未动作，认为速度为0
   // TIM_off();
    if(unfinished1 ==2)
    {
      unfinished1 = 0;
      MY_Coder.spd1 = 200*250/ant_speed1< death_zone ?0:200*250/ant_speed1 ;
    //  return 200*31/ant_speed1< death_zone ?0:200*31/ant_speed1 ;  //death_zone为8K时大概对应的空转档位，以不会抖动为准
    } 
    else if(unfinished1 ==1)//等完发现只进了一次
    {
      TIM_off();
      unfinished1 = 0;
      MY_Coder.spd1 = 0;
    //  return 0;
    }
    else     //等了很久标志没有变
    {
      unfinished1 = 0;
      MY_Coder.spd1 = 0;
    //  TIM_off();
      //return 0;
    }
  }
  else if( channel ==2)
  {
    TIM_start2();
    Delay(1);  // 如果脉冲频率小于8K未动作，认为速度为0
   // TIM_off();
    if(unfinished2 ==2)
    {
      unfinished2 = 0;
      MY_Coder.spd2 = 200*250/ant_speed2< death_zone ?0:200*250/ant_speed2 ;
     // return 200*31/ant_speed2< death_zone ?0:200*31/ant_speed2 ;  //death_zone为8K时大概对应的空转档位，以不会抖动为准
    } 
    else if(unfinished2 ==1)
    {
      TIM_off();
      unfinished2 = 0;
      MY_Coder.spd2 = 0;
      //return 0;
    }
    else
    {
      unfinished2 = 0;
      MY_Coder.spd2 = 0;
    //  TIM_off();
      //return 0;
    }
  }

    
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED    //中断函数置于非分页区内
interrupt VectorNumber_Vtimch0 void	TIM_CAP0_ISR( void )
{
	static	unsigned int	LastCap = 0;	//last cap result
  static  unsigned char enter_times = 0;

	unsigned int		ThisCap;
  enter_times++;
  
   if(unfinished1 ==0 )
   {
      LastCap = 0;
      enter_times = 1;
   }
	
//	DisableInterrupts;
	
	if(	enter_times ==1 )
	{
  	LastCap = TC0;
  	unfinished1 =1; //未完成  	                                                        //上次由于两次沿时间太长，遗留一次
	} 
	else if(enter_times ==2 )
	{
    TIE		=	0x00;
  	TFLG1	=	0xFF;		//clear all flags
    ThisCap	=	TC0;		//read cap result
    switch( TFLG2_TOF )		//determine whether TIM OVERFLOW between two cap
    {
      case 0:
      		ant_speed1	=	ThisCap - LastCap;
      		break;
      case 1:
      		ant_speed1	=	ThisCap + 0xFFFF - LastCap;
      		TFLG2	=	0x80;	//the flag cannot fast clear 'cause didn't read TCNT
      		break;
    }
   // LastCap	=	ThisCap;	//remeber				
    enter_times = 0;
    unfinished1 =2;
        	  
	}

  TFLG1	|=	0x01;
//	EnableInterrupts;
}
#pragma CODE_SEG DEFAULT   //后续代码置于默认区域内

#pragma CODE_SEG __NEAR_SEG NON_BANKED    //中断函数置于非分页区内
interrupt VectorNumber_Vtimch1 void	TIM_CAP1_ISR( void )
{
	static	unsigned int	LastCap = 0;	//last cap result
  static  unsigned char enter_times = 0;

	unsigned int		ThisCap;
  enter_times++;
  
   if(unfinished2 ==0 )
   {
      LastCap = 0;
      enter_times = 1;
   }
	
//	DisableInterrupts;
	
	if(	enter_times ==1 )
	{
  	LastCap = TC1;
  	unfinished2 =1; //未完成  	                                                        //上次由于两次沿时间太长，遗留一次
	} 
	else if(enter_times ==2 )
	{
    TIE		=	0x00;
  	TFLG1	=	0xFF;		//clear all flags
    ThisCap	=	TC1;		//read cap result
    switch( TFLG2_TOF )		//determine whether TIM OVERFLOW between two cap
    {
      case 0:
      		ant_speed2	=	ThisCap - LastCap;
      		break;
      case 1:
      		ant_speed2	=	ThisCap + 0xFFFF - LastCap;
      		TFLG2	=	0x80;	//the flag cannot fast clear 'cause didn't read TCNT
      		break;
    }
   // LastCap	=	ThisCap;	//remeber				
    enter_times = 0;
    unfinished2 =2;
        	  
	}

  TFLG1	|=	0x02;
//	EnableInterrupts;
}
#pragma CODE_SEG DEFAULT   //后续代码置于默认区域内