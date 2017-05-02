#include "inc.h"
#include "clock.h"
#include "mymenu.h"
#include "mydelay.h"
#include "AD.h"
#include "PIT.h"
#include "PWM.h"
#include "SCI.h"
#include "coder.h"
#include "strategy.h"
#include "counter.h"
extern int error_max,error_min;
extern int curr_e;
extern unsigned int a1,a2,a3,a4,b1,b2,b3,b4;

//-------------------flag---------------
unsigned char state_flag=0;  ///////////
extern unsigned char data_finished ;
extern float KP1,KI1,KD1;
extern MOTOR motor_left;
extern MOTOR motor_right;
extern uchar left_minus_flag,right_minus_flag ,pre_right_m;
extern uint uk1;
extern uchar dis;
extern float e11;


//extern uchar cancel_AD ; 

void PortJ_Init(void)//stop the car
 {
    DDRJ = 0XFC; // PJ0判断行同步脉冲到达      // J0J1为输入
    PPSJ = 0x03; // Port J Polarity Select Register-rising EDGE    
 //   PPSJ = 0x00; // Port J Polarity Select Register-falling EDGE   下降沿触发   
    PIEJ = 0X03; // VIDEO SYNC INTERRUPT DISABLED,BUT NOT IN MAIN()    
    PERJ = 0xff;//中断使能 
 }

void Interrupt_Priority_Set(void)
{
//------J口外部中断   7 
    INT_CFADDR=0xC0;//将干簧管，按键中断设为第七优先级
    INT_CFDATA7=0x07;//
//------atd   5   
    INT_CFADDR=0xD0;
    INT_CFDATA1=0x05;
    
//------将TIM_CH1的优先级设定为  6  
    INT_CFADDR=0xE0;
    INT_CFDATA6=0x06;
//------将TIM_CH0的优先级设定为  6     
    INT_CFADDR=0xE0;
    INT_CFDATA7=0x06;
//-------将PIT的优先级设定为  4
    INT_CFADDR=0x70;
    INT_CFDATA5=0x04;//将PIT设为第五优先级    
    
    
//-------将SCI0的优先级设定为  7
    INT_CFADDR=0xD0;
    INT_CFDATA6=0x07;
    
}

void init_key()
{
  DDRH = 0x00; // PORTH input
  PTIH = 0X00; // KEY,PH0--4
  PERH = 0xff; // PORTH pull up 
  PPSH = 0x00; // Port H Polarity Select Register-falling edge
  PIEH = 0x0F; // PORTH interrut disable,
}



void Init_pre()
{
  chaopin(); 
  LCD_Init(); 
  init_key();
  PortJ_Init();
  Interrupt_Priority_Set();
  Counter_Init();
//  PITint();
  pwm_init();
  
  SCI_Init();
  Init_AD();

  
}


void Init_later()
{
  PITint();
  pwm_init();  
  PID_duoji_init();
  SpeedPID_Init();
  
  
}


 
void test()
{
   DDRK = 0x80; // PORTK input
   PORTK_PK7=1;
   Delay_ms(1000); 
   PORTK_PK7=0; 
   Delay_ms(1000); 
}

void Run()
{
 //     Display_num(0,0,ad_last[0].AD_res);
 //     Display_num(6,0,ad_last[0].last);
 //     Display_num(0,1,ad_last[1].AD_res);
 //     Display_num(6,1,ad_last[1].last);
#if 0
      Display_num(0,2,ad_last[2].AD_res);
      Display_num(6,2,ad_last[2].last);
      Display_num(0,3,ad_last[3].AD_res);
      Display_num(6,3,ad_last[3].last);
      Display_num(0,4,ad_last[4].AD_res);
      Display_num(6,4,ad_last[4].last);
      Display_num(0,5,ad_last[5].AD_res);
      Display_num(6,5,ad_last[5].last);
      Display_num(0,1,abs(uk1));
      
      display_char(6,0, dis+0x30);
      

#endif      


#if 0 
   
      Display_f(0,2,motor_left.K_spd_e*10);
      Display_f(0,3,motor_left.K_spd_e*10);
#endif

#if 0   
      Display_f(0,2,KP1*10);
     // Display_num(0,3,KI1);
      Display_f(0,4,KD1*10);
      //Delay_ms(200);
      
#endif

#if 1 
      display_string(0,1,"error_max");
      Display_num(0,2,error_max);
     // Display_num(0,3,KI1);
      Display_num(5,2,abs(error_min));
      //Delay_ms(200);
      display_string(0,5,"e11");
      Display_signednum(0,6,e11);
      Display_signednum(6,6,uk1);
      display_char(6,3, dis+0x30); 

      
#endif
}



void main(void) 
{
  /* put your own code here */ 
   PID_douji_para();                 
   Init_pre();  
   DDRK = 0x80; // PORTK input
   PORTK_PK7=1;
   Init_Menu();
   
   EnableInterrupts;
   // state_flag =1 ;
  
  while(1)
  {
    if(state_flag==0)
    {
      Display_Menu();


    }
    else if(state_flag==1)
    {
          Delay_ms(3000);    
          LCD_clear();
          Init_later();
          error_max = abs((a1-ad_last[2].min)*1.0*100 / (ad_last[2].max-ad_last[2].min) 
                       + ((a2-ad_last[3].min)*1.0*100 / (ad_last[3].max-ad_last[3].min))
                         - ((a3-ad_last[4].min)*1.0*100 / (ad_last[4].max-ad_last[4].min))
                           -((a4-ad_last[5].min)*1.0*100 / (ad_last[5].max-ad_last[5].min)));
          error_min =  abs((b1-ad_last[2].min)*1.0*100 / (ad_last[2].max-ad_last[2].min) 
                       + ((b2-ad_last[3].min)*1.0*100 / (ad_last[3].max-ad_last[3].min))
                         - ((b3-ad_last[4].min)*1.0*100 / (ad_last[4].max-ad_last[4].min))
                           -((b4-ad_last[5].min)*1.0*100 / (ad_last[5].max-ad_last[5].min)));                  
          
              
      while(1)
      {
        if(state_flag==1 )
        {

          if(data_finished )
          {
            data_finished=0;
            showb();
            Run();  
          }
        } 
        else if(state_flag==2)
        {
          PITCFLMT=0X00;     //PIT模块禁止 
          PWMDTY3=0;
          PWMDTY2=0;
          PWMDTY1=0;
          PWMDTY0=0;         
          while(!(state_flag == 1))
          {
            Init_later();
            Delay_ms(3000);
          }
           
        }


       }
     }
    } 
    
  }  

#pragma CODE_SEG __NEAR_SEG NON_BANKED    //中断函数置于非分页区内
void interrupt VectorNumber_Vportj PORTJ_ISR(void)
{
  if(PIFJ_PIFJ0==0)
  {
   PIFJ_PIFJ0 = 1;//清标志位
   //PJ0中断进入，中断处理
   state_flag=2;
  PWMDTY3=0;
  PWMDTY2=0;
  PWMDTY1=0;
  PWMDTY0=0; 
  }
  if(PIFJ_PIFJ1==0)
  {
    PIFJ_PIFJ1 = 1;//清标志位
   //PJ1中断进入，中断处理
    state_flag=2;
    PWMDTY3=0;
    PWMDTY2=0;
    PWMDTY1=0;
    PWMDTY0=0;
  }
}
#pragma CODE_SEG DEFAULT   //后续代码置于默认区域内


