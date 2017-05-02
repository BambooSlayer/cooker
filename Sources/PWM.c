#include "PWM.h"
uchar f;
/*****************PWM初始化***********************/
void pwm_init(void)
{
  PWMPRCLK=0X77;  //预分频   分频   A=B=E/128=500khz    
  PWMSCLA=0x01;  //控制分频    SA=A/(2*PWMSCLA)=A/2=250KHZ  A用于舵机
  PWMSCLB=0x01;    //同上      SB=250khz
  PWMCLK=0x30;   //选择时钟源  SA SB    45选择SA,   23B,01A
  PWMPOL=0xFF;    //控制起始电平
  PWMCAE=0xFF;    //控制居中（左）对齐
  PWMCTL=0x40;    //控制通道的级联及工作模式    45通道级联
  //输出 1k，设置频率 PWMDTY PWMPER       PWMCNTX计数寄存器   舵机F=SA/(2*PWMPER45)=50HZ
                                                             //电机F=B/(2*PWPMERN)=1khz
  PWMDTY3=0;  //输出fre =  1k  
  PWMPER3=250;
  PWMDTY2=0;  //输出 1k     初始速度为0
  PWMPER2=250;
  PWMDTY1=0;  //输出 1k   
  PWMPER1=250;
  PWMDTY0=0;  //输出 1k     初始速度为0
  PWMPER0=250; 
  
  PWMDTY45=118; //输出 50hz    舵机   一开始0度偏转  舵机偏转  106- 118-130 ,+12/-12
  PWMPER45=2500;
  
   //全部使能 PWME
  PWME_PWME3=1;   //电机通道
  PWME_PWME2=1;
  PWME_PWME1=1;   //电机通道
  PWME_PWME0=1;
  PWME_PWME5=1;   //舵机通道
      
}
/*void control(uchar Re_data) 
{
      
     	 switch(Re_data)				           
		 {
		   case 0x30:   if(f==0)
		                {
		                   PWMDTY3=0;
		                   PWMDTY2=60;    //125
		                   PWMDTY1=0;
		                   PWMDTY0=60;  
		                } 
		                else
		                {
		                   PWMDTY3=60;
		                   PWMDTY2=0;
		                   PWMDTY1=60;
		                   PWMDTY0=0;
		                }
		               // PWMDTY45=118;
		            	 break;    //正常 速度50%  0偏角
		            	 
		   case 0x31:   PWMDTY3=0;
		                PWMDTY2=62; 
		                PWMDTY1=0;
		                PWMDTY0=62; 
		                f=0;              
		            	 break;    //前进 100%
		            	 
		   case 0x32:  //PWMDTY45=111;
		   
		               PWMDTY3=0;
		               PWMDTY2=30;
		               PWMDTY1=0;
		               PWMDTY0=30;
		   
		            		              	          
		               break;    //左转
		    
		   case 0x33:  PWMDTY3=0;
		               PWMDTY2=90;
		               PWMDTY1=0;
		               PWMDTY0=90;
		               f=1;		               		               
		               break;    //后退  100%
		    
		   case 0x34:  //PWMDTY45=125;
		   
		                PWMDTY3=0;
		               PWMDTY2=60;
		               PWMDTY1=0;
		               PWMDTY0=60;
		   		             	 
		               break;		 //右转  
		   
		   case 0x35: PWMDTY3=0;
		              PWMDTY2=0;
		              PWMDTY1=0;
		              PWMDTY0=0;
		              PWMDTY45=118;	 
		              break;//停止
		 }
}*/
