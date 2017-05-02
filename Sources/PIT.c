#include "PIT.h"
#include "nokia_5110.h"
#include "AD.h"
#include "coder.h"
#include"counter.h"
#include"strategy.h"


extern MY_Coderdata MYCoder;
extern uchar state_flag;

unsigned char data_finished = 0;
void PITint(void)     //定时时间=（PITMTLD+1)*(PITLD+1)/fBUS=100*40000/(64*10^6)=1/16s=1000/16=250/4=62.5ms
{
   PITCFLMT=0X00;     //PIT模块禁止 
   PITTF=0X01;        //向超时标志寄存器写，0通道超时
   PITMUX=0X00;        //16位定时器采用微时间基准0
   PITMTLD0=63;        //8位递减计数器装入初值
   PITLD0=5000;       //16位，，，，，，，，  1000 1ms ;改为5000则定时5ms 
   PITINTE=0X01;       // 0通道中断使能
   PITCE=0X01;         //0通道使能
   PITCFLMT=0X80;      // PIT模块使能
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED    //中断函数置于非分页区内
interrupt 66 void PIT0_ISR() //通道0的中断向量为66 其余为67，68，69
{
  static uchar PIT_COUNTER = 0;
   EnableInterrupts      //在低优先级中断里再开一次总中断可以
  //  PITCE=0X00;
   PITTF=0X01;              //清除标志位
   
   if(PIT_COUNTER == N)
   {
     PIT_COUNTER=0;
   } 
   else
   {
     PIT_COUNTER ++;
   }
   
   
 //*******磁场检测模块************************AD检测   
    AD_get(0,5);  //
    SpeedGet();
    if(state_flag == 1)
    {
      Speedset_openloop();
      //Speedset_closedloop()
    } 
    else if(state_flag == 2)
    {
      PWMDTY3=0;   
      PWMDTY2=0;  //输出 1k   速度最大70%    右    初始速度为0
      PWMDTY1=0;   
      PWMDTY0=0;    

    }
;

    
    PD_duoji();   //stop ==1  
//    pre_judge();
   // analyse( PIT_COUNTER );



    
    data_finished = 1;

    //speed_get(1);
    //speed_get(2);
   
      // if(ii==4)
      // {
     //     AD_average()  ; //求两次的平均值
      //    PD_duoji() ; //40ms一次修改        s
      //    ii=0       ;
      //    AD_sumstart() ;//累加和清0 
      // } 
      
    
    
   //***************flash写入子程序***********************
   //时间规划时可以，将磁场检测与Flash写入放在一起
   
/*   FLAG++; 
   if(FLAG==4) 
   {          
        FLAG=0;
   }   */
//   PITCE=0X01; 
///EnableInterrupts  
}
#pragma CODE_SEG DEFAULT   //后续代码置于默认区域内