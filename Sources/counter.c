#include"counter.h"

MY_Coderdata MYCoder,Old_Coder[3];
unsigned int Num_r=0,Num_l=0;
void Counter_Init(void)        //code1     
{         
  TIOS	=	0x00;		//all input cap enable
	TCTL4	=	0x05;		//active edge on channel 0 is Rising edge
	TSCR2	=	0x06;		//TIMEclock = BUSclock / 4
	TFLG1	=	0xFF;		//clear all flags
	TFLG2	=	0x80;		//clear TOF
//	TIE		=	0x01;		//enable channel0 INT
	TSCR1	=	0xA0;		//ENABLE TIM,FREEZE MODE STOP,DON'T FAST FLAG CLEAR	
}

void SpeedGet()
{
   TIE	=	0x00;
   Old_Coder[2].spdr = MYCoder.spdr;
   Old_Coder[1].spdr = Old_Coder[2].spdr;
   Old_Coder[0].spdr = Old_Coder[1].spdr;
   
   Old_Coder[2].spdl = MYCoder.spdl;
   Old_Coder[1].spdl = Old_Coder[2].spdl;
   Old_Coder[0].spdl = Old_Coder[1].spdl;
   
   MYCoder.spdr = (Num_r+ Old_Coder[0].spdr + Old_Coder[1].spdr+ Old_Coder[2].spdr)>>2;
   MYCoder.spdl = (Num_l+ Old_Coder[0].spdl + Old_Coder[1].spdl+ Old_Coder[2].spdl)>>2;
   Num_l = 0;
   Num_r = 0;
   TIE =	0x03;    //rightwheel  
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED    //中断函数置于非分页区内
interrupt VectorNumber_Vtimch1 void COUNTER0_ISR( void )
{
  TFLG1	|=	0x01;
  Num_l++; 
}
#pragma CODE_SEG DEFAULT   //后续代码置于默认区域内

#pragma CODE_SEG __NEAR_SEG NON_BANKED    //中断函数置于非分页区内
interrupt VectorNumber_Vtimch0 void COUNTER1_ISR( void )
{
  TFLG1	|=	0x02;
  Num_r++; 
}
#pragma CODE_SEG DEFAULT   //后续代码置于默认区域内
