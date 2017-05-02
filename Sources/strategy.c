#include "strategy.h"
#include "AD.h"
#include "counter.h"
extern unsigned char CTRL_Msg_FROM_PC;   //使用紧急数据通道进行调试
#include "math.h"


#define se 0//是否使用差速

#define Right_Max 131
#define Left_Max 105
#define Mid 118


unsigned int Feedback_Vmax=200;
unsigned char V_Respect_MAX;
uchar stop_flag =0 ;
uchar pre_right_m = 0;
uchar pre_flag = 0;
uchar dis = 0;


extern MY_Coderdata MYCoder;
extern uchar Tab;

extern uchar scan_PB();
extern uchar state_flag ;

extern int error_max,error_min;


float  e11,e21,e31,SpeedDet1,duk1,KP1,KI1,KD1,uk1f,SpeedSet1;
int  uk1;
uchar  flag_mode;    //数组指针 运行模式指针
float K_e1=2,K_e2=2;


int daishuhe,jieduizhi,daishuhe1,jieduizhi1,jdz;
int weizhi;
int weizhi_Buffer[40];


MOTOR motor_left;
MOTOR motor_right;

//float Kp=1.9,Ki=0.4,Kd=1;   
/*开环调速程序，给Tab赋值后调用*/
void Speedset_openloop()
{
  #if 1  
    Tab = scan_PB();
    if (Tab > Max_Tab)
    {
      Tab =Max_Tab;
    }
    
    else
    {

      PWMDTY3=0;
      //PWMDTY2= 62 ;
      PWMDTY2=Tab*1.0/Max_Tab*250;  //输出 1k   速度最大70%    右    初始速度为0
      PWMDTY1=0;
      //PWMDTY0=  62;
      PWMDTY0=Tab*1.0/Max_Tab*250;
      //输出 1k   速度最大70%    右    初始速度为0 
    }
 #else 
    Tab = scan_PB();
    if (Tab > 250)
    {
      Tab =250;
    }
    
      
    PWMDTY3=0;
    //PWMDTY2= 62 ;
    PWMDTY2=Tab;//*1.0/Max_Tab*250;  //输出 1k   速度最大70%    右    初始速度为0
    PWMDTY1=0;
    //PWMDTY0=  62;
    PWMDTY0=Tab;//*1.0/Max_Tab*250;
#endif


}
void douji_a(char a)
{
  
   /*
  uchar uk1;
  uk1=118+a;
  if(uk1>129)        uk1=129;//右满舵，给舵机（107--118--129）
		else if(uk1<107)   uk1=107;//左满舵
	PWMDTY45=uk1;*/
}

/*------------------------基础算法 --------------------------*/

void SpeedPID_Init()
{
  motor_left.Last_error=0;
  motor_left.Pre_error=0;
  motor_left.curr_spd=&MYCoder.spdl; 
  motor_left.error_value=0;
  motor_left.Kp=3.8;//3.5;
  motor_left.Ki=0.5;
  motor_left.Kd = 0;
  motor_left.K_spd_e = 1.0;

  
  motor_right.Last_error=0;
  motor_right.Pre_error=0;
  motor_right.curr_spd=&(MYCoder.spdr);  
  motor_right.error_value=0;
  motor_right.Kp=3.5;
  motor_right.Ki=0.5;
  motor_right.Kd = 0;
  motor_right.K_spd_e = 0.5;
}

void Speed_PID(uint tar_speed,MOTOR *channel)
{
   float curr_speed=0;                       //  最大反馈速度
   float curr_error=0;
   float error_value=0;                   
    
   curr_speed =  (*(channel->curr_spd))*V_Max / Feedback_Vmax ;//化归到和给定相同
   curr_error = tar_speed*1.0-curr_speed;
   error_value = channel->Kp*(curr_error-channel->Last_error)  
                  +channel->Ki*(curr_error) 
                    +channel->Kd*(curr_error-2*channel->Last_error+channel->Pre_error);
                  
                    
                    
   channel->error_value = channel->error_value + error_value;
   if(channel->error_value > V_Max )
   {
      channel->error_value= V_Max ;
   } 
   else if(channel->error_value<0)
   {
      channel->error_value=0;
   }
           
   channel->Pre_error = channel->Last_error;
   channel->Last_error = curr_error;       
} 


/*
void Speed_PID(uint tar_speed,uchar channel)
{
   float curr_speed=0;                       //  最大反馈速度
   float curr_error=0;
   float error_value=0;                   
   if(channel==2) 
   {
    
     //curr_speed =  (MYCoder.spdl)*V_Max / Feedback_Vmax ;//化归到和给定相同
     curr_speed =  (*(motor_left.curr_spd))*V_Max / Feedback_Vmax ;//化归到和给定相同
     curr_error = tar_speed*1.0-curr_speed;
     error_value = Kp*(curr_error-motor_left.Last_error)  
                    +Ki*(curr_error) 
                      +Kd*(curr_error-2*motor_left.Last_error+motor_left.Pre_error);
                    
                      
                      
     motor_left.error_value = motor_left.error_value + error_value;
     if(motor_left.error_value > V_Max )
     {
        motor_left.error_value= V_Max ;
     } 
     else if(motor_left.error_value<0)
     {
        motor_left.error_value=0;
     }
             
     motor_left.Pre_error = motor_left.Last_error;
     motor_left.Last_error = curr_error;
   }    
   
   else if(channel ==1)
   {
     curr_speed = (MYCoder.spdr)*V_Max / Feedback_Vmax ;//化归到和给定相同
     curr_error = tar_speed*1.0-curr_speed;
     error_value = Kp*(curr_error-motor_left.Last_error)  
                    +Ki*(curr_error) 
                      +Kd*(curr_error-2*motor_left.Last_error+motor_left.Pre_error);
                    
                      
                      
     motor_right.error_value = motor_right.error_value + error_value;
     if(motor_right.error_value > V_Max )
     {
        motor_right.error_value= V_Max ;
     } 
     else if(motor_right.error_value<0)
     {
        motor_right.error_value=0;
     }
             
     motor_right.Pre_error = motor_right.Last_error;
     motor_right.Last_error = curr_error;   
   } 
    
}      */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~我是华丽的分界线---------------------
//                
//                      以下为上层算法
//
//
//---------------------------------------------------------
/*------------------------几点说明 --------------------------
瞬时速度值在结构体   MY_Coder中
AD采样的速度值在 结构体   ad_last
显示数据，只需在主函数中加
蓝牙发送的数据在showb中改，按serialchart的格式改

算法自行加在主函数中
采样周期为5ms，由于采样共用时长为2ms，注意算法运行时间与显示、蓝牙时间之和不可超过3ms
执行机构先直接赋值吧，正在写
*/

uchar duty2respect(uchar duty) //占空比转换部分(0~100)
{
  if( duty >100)
  {
    duty =100;
  } 

   return duty*5/2 ;     
}


void Speedset_closedloop()//速度规划部分
{
  //  V_Respect_MAX = scan_PB() ; --------------------------最好开启，到比赛时可以整体调速
    V_Respect_MAX = 100;
    if (V_Respect_MAX > 100)     //PWMPER
    {
      V_Respect_MAX =100;        //
    }
    
//   motor_left.v_respect = duty2respect(CTRL_Msg_FROM_PC);//CTRL_Msg_FROM_PC*10 ;    // (0~250)
//   motor_right.v_respect = duty2respect(CTRL_Msg_FROM_PC);
    Tab = scan_PB();
    if (Tab > Max_Tab)
    {
      Tab = Max_Tab;
    }

   motor_left.v_respect = duty2respect(Tab*1.0/Max_Tab*100);
   motor_right.v_respect = duty2respect(Tab*1.0/Max_Tab*100);
   
      if(state_flag == 2)
   {
       motor_left.v_respect = 0;
       motor_right.v_respect = 0;
   }
   
    Speed_PID(motor_left.v_respect,&motor_left);
    Speed_PID(motor_right.v_respect,&motor_right);
 //   Speed_PID(motor_left.v_respect,2);
 //   Speed_PID(motor_right.v_respect,1);
    
    PWMDTY3=0;
    PWMDTY2= motor_right.error_value;//输出 1k    右   正转
    PWMDTY1=0;
    PWMDTY0= motor_left.error_value;  //输出 1k    左  正转  

}

///-------------赛道预判
void pre_judge()
{
   // static uchar turn=0,straight=1;
    if ((abs(ad_last[0].last-ad_last[1].last) <8 )  )   // &&(turn == 1) 
    {
     //   turn = 0;
     //   straight=1;
        DDRK = 0x80; // PORTK input     //直道
        PORTK_PK7=1;
      //  KP1 = 13;
      //  KD1 = 5.2; 

    }
    else if( (abs(ad_last[0].last-ad_last[1].last)> 15 ) )//&& (straight ==1)
    {
     //   turn = 1;
     //   straight=0;    
        DDRK = 0x80; // PORTK input     //弯道
        PORTK_PK7=0;
      //  KP1 = 5;
      //  KD1 = 5.6; 
    }
}

///---------------舵机调节



void PID_duoji_init(void) 
{
   // 初始化
   //速度输出：范围0-250，从0速到全速
  #if 0
	 SpeedSet0=0;//速度设定值，对25%-75%的阶跃进行测试，最高速度,可设置档位n，
	 SpeedDet0=0; //速度检测值（目前范围0000--4095）
	 e10=0;       //速度偏差
	 e20=0;
	 e30=0;
	 KP0=1;      //PID设定
	 KI0=4;      //KI0/KD0=4
	 KD0=0; 	   //
	#endif
	 //舵机直道输出
	 SpeedSet1=0;//速度设定值，对25%-75%的阶跃进行测试，最高速度,可设置档位n，
	 SpeedDet1=0; //速度检测值（目前范围0000--4095）
	 e11=0;       //速度偏差
	 e21=0;
	 e31=0;

	 uk1=118;    //初始化设定后就不用再改变
	 uk1f=118;
	 //舵机弯道输出：范围0-250，从0速到全速
	 #if 0
	 SpeedSet2=0;//速度设定值，对25%-75%的阶跃进行测试，最高速度,可设置档位n，
	 SpeedDet2=0; //速度检测值（目前范围0000--4095）
	 e12=0;       //速度偏差
	 e22=0;
	 e32=0;
	 KP2=10;      //PID设定
	 KI2=0;
	 KD2=0; 
	 #endif 
}

void PID_douji_para()
{
  	 KP1=9;      //PID设定64
//	 KI1=100;    //相当于没用
     KI1 =630;
	   KD1=11;
}

#define led5 CTRL_Msg_FROM_PC

void PID_now(void) 
{
   switch(led5 & 0xc0)
   {
    case 0x00:
       KP1=(led5&0x3f)/4.0;
      // motor_left.K_spd_e =  (led5&0x3f)/10;
      // motor_right.K_spd_e =  (led5&0x3f)/10;
       break;
    case 0x40:
       KI1=(led5&0x3f)*10.0;      //           0000 0000
       break;
    case 0xc0:
       KD1=(led5&0x3f)/4.0;
       break;
   }

    #if 0 
    if(led5>=0x30&&led5<0x40) 
    {
         KP1=led5-0x30;
       //  PWMDTY45=118;
    }
    if(led5>=0x40&&led5<0x50) 
    {
         KI1=led5-0x40;
       //  PWMDTY45=134;
    }
     if(led5>=0x50&&led5<0x60) 
    {
         KD1=led5-0x50;
        //  PWMDTY45=131;
    }
   
     if(led5>=0x60&&led5<0x70) 
    {
         dangwei=led5-0x60;
        // dianjisudu=25*dangwei;
        //  PWMDTY45=118;
    }
     if(led5>=0x70&&led5<0x80) 
    {
         //dongdao=led5-0x60;
         KI0= led5-0x70;
        //  PWMDTY45=134;
    }
    #endif
   
}
            
#define dianci3 ad_last[2].last
#define dianci4 ad_last[3].last
#define dianci5 ad_last[4].last
#define dianci6 ad_last[5].last

void PD_duoji()     
{
  //uk1f=118;

   if( dianci3 + dianci4 > dianci5 + dianci6) 
   {
       SpeedDet1= dianci3 + dianci4 - dianci5 - dianci6;
       e11 = (SpeedSet1-SpeedDet1);   
      if ( dis == 1)
      {
        e11 = -1*error_max + (-1*error_max - e11 );
      }
   }//除20有点小振荡，除30即比例度减小，但产生反作用
   else 
   {                                   
       SpeedDet1 = dianci5 + dianci6 - dianci3 - dianci4;
       e11=(SpeedSet1+SpeedDet1);
        if (dis == 2)
        {
          e11 = error_min + ( error_min - e11 ) ;
        } 
   }   
   
   if( e11 < -1*0.5*error_max   )
   {
      e11 =  -1*0.5*error_max+ K_e1*( -1*0.5*error_max-e11);
   } else if( e11> 0.5*error_min)
   {
      e11 =   0.5*error_min + K_e2*( 0.5*error_min - e11 ) ;
   }

	
	
		//巡线则使Speedset1接近于0或小车靠中间状态即  dianci3+dianci4-dianci5-dianci6==0
			duk1=((e11-e21)+/*e11/KI1+*/KD1*(e11-2*e21+e31))/KP1; //加比例成功
	//	duk1=KP1*(e11-e21)+KI1*e11/8+KD1*(e11-2*e21+e31)/15; //加比例成功
	//	duk1=((e11-e21)+(KD1)*(e11-2*e21+e31))/(KP1);
   // uk1f=uk1f+e11/KP1;    //可用 
    
    uk1f=uk1f+duk1;
     e31=e21;
	   e21=e11;
    
   //获取PID调节的数据 
    uk1=uk1f;//这里产生的uk1取值范围应该为（107--129),duk1取值（-13 -- 13）	
	  //PD溢出调节
	  if(uk1> Right_Max)        uk1=Right_Max;//右满舵，给舵机
		else if(uk1<Left_Max)   uk1=Left_Max;//左满舵  
	


 		
   // if(( dianci3 > dianci4 )&&( dianci5 > dianci6 )&&( dianci5 <75))//|(dianci1-dianci2)>60为什么还会出现一瞬间的反变化
    if((dianci3 > 0)&&(right_minus_flag==1))   //左入弯
    {
      pre_right_m = 1;        //左预判标志 1
     // right_minus_flag = 0;     
      PWMDTY45=uk1; //uk1
      dis = 1;
 #if se  
      motor_left.v_respect  = Tab*1.0/Max_Tab*100 + (103 - 118) * motor_left.K_spd_e;
      motor_right.v_respect =  Tab*1.0/Max_Tab*100 - (103 - 118) * motor_right.K_spd_e;
 #endif  
     // PID_duoji_init();
     
    }
    //else if(( dianci6 > dianci5 )&&( dianci4 > dianci3 )&&( dianci4 <80))//|(dianci2-dianci1)>60
    else if((dianci6 > 0)&&( left_minus_flag==1))//右入弯
    {
      pre_right_m = 2;        //右预判标志  2
    //  left_minus_flag =0;
      PWMDTY45= uk1;
      dis = 2;
      
  #if se  
      motor_left.v_respect  = Tab*1.0/Max_Tab*100 + (134 - 118) * motor_left.K_spd_e;
      motor_right.v_respect =  Tab*1.0/Max_Tab*100 - (134 - 118) * motor_right.K_spd_e;
  #endif     
     // PID_duoji_init();
    }
    else if((pre_right_m == 1) && (dianci3 ==0))
    {
      PWMDTY45=Left_Max;
      dis = 3;
  #if se  
      motor_left.v_respect  = Tab*1.0/Max_Tab*100 + (103 - 118) * motor_left.K_spd_e;
      motor_right.v_respect =  Tab*1.0/Max_Tab*100 - (103 - 118) * motor_right.K_spd_e;
  #endif
    } 
    else if((pre_right_m == 2) &&(dianci6 ==0))
    {
      PWMDTY45= Right_Max;
      dis = 4;
  #if se  
      motor_left.v_respect  = Tab*1.0/Max_Tab*100 + (134 - 118) * motor_left.K_spd_e;
      motor_right.v_respect =  Tab*1.0/Max_Tab*100 - (134 - 118) * motor_right.K_spd_e;
  #endif     
    }
   // else if((right_minus_flag ==1) && (left_minus_flag ==1)) //调试用
  //  {
  //    right_minus_flag = 0;
  //    left_minus_flag =0;
  //    stop_flag =1;
  //  }
    else
    {  
      pre_right_m=0;
      PWMDTY45=uk1; //可以不加速度差
      dis = 0;
/*  #if se  
      motor_left.v_respect  = Tab*1.0/Max_Tab*100 + (uk1 - 118) * motor_left.K_spd_e;
      motor_right.v_respect =  Tab*1.0/Max_Tab*100 - (uk1 - 118) * motor_right.K_spd_e;
  #endif   */
    }
}  


void  analyse( uchar num )
{
   //40个数据的分析处理
//   int daishuhe,jieduizhi,daishuhe1,jieduizhi1,jdz;
   uint zhibiaomin,zhibiaomax;
   uint deta = 200;
   
   uint zhibiao_dai_min ;
   uchar i_flag;//
   zhibiaomin=800;
   zhibiaomax=1000;
   zhibiao_dai_min = 500;

   
   //每来一次数据刷一次
   for(i_flag=79;i_flag>0;i_flag--)      //39
       weizhi_Buffer[i_flag]=weizhi_Buffer[i_flag-1] ; 
    weizhi_Buffer[0]=dianci3+dianci4-dianci5-dianci6;//weizhi_Buffer[0]=weizhi; 
   //前20个数据处理
   
   if(num== N)
   {
   daishuhe=0;
   jieduizhi=0;
   daishuhe1=0;
   jieduizhi1=0;
      
     for(i_flag=0;i_flag<40;i_flag++) 
     {   
         jdz=weizhi_Buffer[i_flag];     
         if(weizhi_Buffer[i_flag]<0)  
             jdz=-weizhi_Buffer[i_flag];
         daishuhe=daishuhe+weizhi_Buffer[i_flag];//求前20个数代数和
         jieduizhi=jieduizhi+jdz;                //求前20个数绝对值和
     }
      
      //后20个数据处理
     for(i_flag=40;i_flag<80;i_flag++) 
     { 
         jdz=weizhi_Buffer[i_flag];     
         if(weizhi_Buffer[i_flag]<0)  
             jdz=-weizhi_Buffer[i_flag];
         daishuhe1=daishuhe1+weizhi_Buffer[i_flag]; //求后20个数代数和
         jieduizhi1=jieduizhi1+jdz;                 //求后20个数绝对值和
         
     }
     
     if( daishuhe1 < 0 )
     {
      daishuhe1 = 0- daishuhe1;
     }
     if(daishuhe < 0)
     {
      daishuhe = 0- daishuhe;
     }
     
    // if(daishuhe1<zhibiaomin&&jieduizhi1<zhibiaomin&&daishuhe>zhibiaomax&&jieduizhi>zhibiaomax) //入弯  下坡时相当于入弯 减速    
    if((jieduizhi > zhibiaomax) && (jieduizhi1<zhibiaomin))
     {
        flag_mode=3;
       // KP1=3;
       // blutooth(3);
        DDRK = 0x80; // PORTK input
        PORTK_PK7=0;
        KP1 = 6;
        KD1 = 5.6;
     } 
     if(daishuhe1<zhibiao_dai_min && daishuhe<zhibiao_dai_min)
     {
        flag_mode=0;
        DDRK = 0x80; // PORTK input        // 灭
        PORTK_PK7=1;
        PID_douji_para();
     }
 //  else if(daishuhe<20&&jieduizhi>80&&daishuhe1<20&&jieduizhi1>80) //s弯     
  //     flag_mode=4; 
  #if 0
   else if(daishuhe<zhibiaomin&&jieduizhi<zhibiaomin&&daishuhe1<zhibiaomin&&jieduizhi1<zhibiaomin) //直道 
     {
         
      //  KP1=7;
        DDRK = 0x80; // PORTK input        // 灭
        PORTK_PK7=1
        
       //blutooth(0);
      } 
   #endif 
   }
   


                  
    //设计类似于归一化的算法来 判断数据运行模式
    //判断数目太多，可能出现混淆，具体检测，看在赛道上的表现
  
/*   if(dianci7>80|dianci8>80)  //7，8号传感器判断
   { 
        if(dianci7-dianci8<40) 
             flag_mode=1;// 十字弯
        else flag_mode=5; //直角弯
   }*/
   //5,6号传感器判断
 
  // if(daishuhe<zhibiaomin&&jieduizhi<zhibiaomin&&daishuhe1>zhibiaomax&&jieduizhi1>zhibiaomax) //出弯  爬坡时相当于出弯 加速 
    //   flag_mode=2; 
          
 //  else if(daishuhe>80&&jieduizhi>80&&daishuhe1>80&&jieduizhi1>80) //弯道 
 //      flag_mode=7;
 //  else   flag_mode=8;  */  //意外情况，停止，可能不存在
  
}



/*
void PD_duoji(void)     
{
  //uk1f=118;
   if(dianci3+dianci4>dianci5+dianci6) 
   {
      SpeedDet1=dianci3+dianci4-dianci5-dianci6;
       e11=(SpeedSet1-SpeedDet1);   
   }//除20有点小振荡，除30即比例度减小，但产生反作用
   else 
   {
       SpeedDet1=dianci5+dianci6-dianci3-dianci4;
       e11=(SpeedSet1+SpeedDet1); 
   }   
   
		//巡线则使Speedset1接近于0或小车靠中间状态即  dianci3+dianci4-dianci5-dianci6==0
	//	duk1=KP1*(e11-e21)+KI1*e11/8+KD1*(e11-2*e21+e31)/15; //加比例成功
		duk1=((e11-e21)+(KD1)*(e11-2*e21+e31))/(KP1);
   // uk1f=uk1f+e11/KP1;    //可用
    
    
    uk1f=uk1f+duk1;
     e31=e21;
	   e21=e11;
    
   //获取PID调节的数据 
    uk1=uk1f;//这里产生的uk1取值范围应该为（107--129),duk1取值（-11 -- 11）	
	  //PD溢出调节
	  if(uk1>134)        uk1=134;//右满舵，给舵机（107--118--129）
		else if(uk1<103)   uk1=103;//左满舵  
		
    if((dianci3>dianci4)&&(dianci5>dianci6)&&(dianci5<75))//|(dianci1-dianci2)>60为什么还会出现一瞬间的反变化
    {
      PWMDTY45=103;   
      PID_init();
    }
    else if((dianci6>dianci5)&&(dianci4>dianci3)&&(dianci4<80))//|(dianci2-dianci1)>60
    {
      PWMDTY45=134;
      PID_init();
    }
    else   
      PWMDTY45=uk1;		
   
		
 //if((dianci3>dianci4&&dianci3<50))//|(dianci1-dianci2)>60为什么还会出现一瞬间的反变化
 // {
   // PWMDTY45=103;
   // uk1=103;
 // }
     //else if((dianci6>dianci5&&dianci6<50))//|(dianci2-dianci1)>60
  // {
  // PWMDTY45=134;
   // uk1=134;
  // }    

//else{

   //   PWMDTY45=uk1;  //此时才使用PID调节
//	}
}       */