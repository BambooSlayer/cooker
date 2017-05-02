#include "strategy.h"
#include "AD.h"
#include "counter.h"
extern unsigned char CTRL_Msg_FROM_PC;   //ʹ�ý�������ͨ�����е���
#include "math.h"


#define se 0//�Ƿ�ʹ�ò���

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
uchar  flag_mode;    //����ָ�� ����ģʽָ��
float K_e1=2,K_e2=2;


int daishuhe,jieduizhi,daishuhe1,jieduizhi1,jdz;
int weizhi;
int weizhi_Buffer[40];


MOTOR motor_left;
MOTOR motor_right;

//float Kp=1.9,Ki=0.4,Kd=1;   
/*�������ٳ��򣬸�Tab��ֵ�����*/
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
      PWMDTY2=Tab*1.0/Max_Tab*250;  //��� 1k   �ٶ����70%    ��    ��ʼ�ٶ�Ϊ0
      PWMDTY1=0;
      //PWMDTY0=  62;
      PWMDTY0=Tab*1.0/Max_Tab*250;
      //��� 1k   �ٶ����70%    ��    ��ʼ�ٶ�Ϊ0 
    }
 #else 
    Tab = scan_PB();
    if (Tab > 250)
    {
      Tab =250;
    }
    
      
    PWMDTY3=0;
    //PWMDTY2= 62 ;
    PWMDTY2=Tab;//*1.0/Max_Tab*250;  //��� 1k   �ٶ����70%    ��    ��ʼ�ٶ�Ϊ0
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
  if(uk1>129)        uk1=129;//�����棬�������107--118--129��
		else if(uk1<107)   uk1=107;//������
	PWMDTY45=uk1;*/
}

/*------------------------�����㷨 --------------------------*/

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
   float curr_speed=0;                       //  ������ٶ�
   float curr_error=0;
   float error_value=0;                   
    
   curr_speed =  (*(channel->curr_spd))*V_Max / Feedback_Vmax ;//���鵽�͸�����ͬ
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
   float curr_speed=0;                       //  ������ٶ�
   float curr_error=0;
   float error_value=0;                   
   if(channel==2) 
   {
    
     //curr_speed =  (MYCoder.spdl)*V_Max / Feedback_Vmax ;//���鵽�͸�����ͬ
     curr_speed =  (*(motor_left.curr_spd))*V_Max / Feedback_Vmax ;//���鵽�͸�����ͬ
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
     curr_speed = (MYCoder.spdr)*V_Max / Feedback_Vmax ;//���鵽�͸�����ͬ
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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~���ǻ����ķֽ���---------------------
//                
//                      ����Ϊ�ϲ��㷨
//
//
//---------------------------------------------------------
/*------------------------����˵�� --------------------------
˲ʱ�ٶ�ֵ�ڽṹ��   MY_Coder��
AD�������ٶ�ֵ�� �ṹ��   ad_last
��ʾ���ݣ�ֻ�����������м�
�������͵�������showb�иģ���serialchart�ĸ�ʽ��

�㷨���м�����������
��������Ϊ5ms�����ڲ�������ʱ��Ϊ2ms��ע���㷨����ʱ������ʾ������ʱ��֮�Ͳ��ɳ���3ms
ִ�л�����ֱ�Ӹ�ֵ�ɣ�����д
*/

uchar duty2respect(uchar duty) //ռ�ձ�ת������(0~100)
{
  if( duty >100)
  {
    duty =100;
  } 

   return duty*5/2 ;     
}


void Speedset_closedloop()//�ٶȹ滮����
{
  //  V_Respect_MAX = scan_PB() ; --------------------------��ÿ�����������ʱ�����������
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
    PWMDTY2= motor_right.error_value;//��� 1k    ��   ��ת
    PWMDTY1=0;
    PWMDTY0= motor_left.error_value;  //��� 1k    ��  ��ת  

}

///-------------����Ԥ��
void pre_judge()
{
   // static uchar turn=0,straight=1;
    if ((abs(ad_last[0].last-ad_last[1].last) <8 )  )   // &&(turn == 1) 
    {
     //   turn = 0;
     //   straight=1;
        DDRK = 0x80; // PORTK input     //ֱ��
        PORTK_PK7=1;
      //  KP1 = 13;
      //  KD1 = 5.2; 

    }
    else if( (abs(ad_last[0].last-ad_last[1].last)> 15 ) )//&& (straight ==1)
    {
     //   turn = 1;
     //   straight=0;    
        DDRK = 0x80; // PORTK input     //���
        PORTK_PK7=0;
      //  KP1 = 5;
      //  KD1 = 5.6; 
    }
}

///---------------�������



void PID_duoji_init(void) 
{
   // ��ʼ��
   //�ٶ��������Χ0-250����0�ٵ�ȫ��
  #if 0
	 SpeedSet0=0;//�ٶ��趨ֵ����25%-75%�Ľ�Ծ���в��ԣ�����ٶ�,�����õ�λn��
	 SpeedDet0=0; //�ٶȼ��ֵ��Ŀǰ��Χ0000--4095��
	 e10=0;       //�ٶ�ƫ��
	 e20=0;
	 e30=0;
	 KP0=1;      //PID�趨
	 KI0=4;      //KI0/KD0=4
	 KD0=0; 	   //
	#endif
	 //���ֱ�����
	 SpeedSet1=0;//�ٶ��趨ֵ����25%-75%�Ľ�Ծ���в��ԣ�����ٶ�,�����õ�λn��
	 SpeedDet1=0; //�ٶȼ��ֵ��Ŀǰ��Χ0000--4095��
	 e11=0;       //�ٶ�ƫ��
	 e21=0;
	 e31=0;

	 uk1=118;    //��ʼ���趨��Ͳ����ٸı�
	 uk1f=118;
	 //�������������Χ0-250����0�ٵ�ȫ��
	 #if 0
	 SpeedSet2=0;//�ٶ��趨ֵ����25%-75%�Ľ�Ծ���в��ԣ�����ٶ�,�����õ�λn��
	 SpeedDet2=0; //�ٶȼ��ֵ��Ŀǰ��Χ0000--4095��
	 e12=0;       //�ٶ�ƫ��
	 e22=0;
	 e32=0;
	 KP2=10;      //PID�趨
	 KI2=0;
	 KD2=0; 
	 #endif 
}

void PID_douji_para()
{
  	 KP1=9;      //PID�趨64
//	 KI1=100;    //�൱��û��
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
   }//��20�е�С�񵴣���30�������ȼ�С��������������
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

	
	
		//Ѳ����ʹSpeedset1�ӽ���0��С�����м�״̬��  dianci3+dianci4-dianci5-dianci6==0
			duk1=((e11-e21)+/*e11/KI1+*/KD1*(e11-2*e21+e31))/KP1; //�ӱ����ɹ�
	//	duk1=KP1*(e11-e21)+KI1*e11/8+KD1*(e11-2*e21+e31)/15; //�ӱ����ɹ�
	//	duk1=((e11-e21)+(KD1)*(e11-2*e21+e31))/(KP1);
   // uk1f=uk1f+e11/KP1;    //���� 
    
    uk1f=uk1f+duk1;
     e31=e21;
	   e21=e11;
    
   //��ȡPID���ڵ����� 
    uk1=uk1f;//���������uk1ȡֵ��ΧӦ��Ϊ��107--129),duk1ȡֵ��-13 -- 13��	
	  //PD�������
	  if(uk1> Right_Max)        uk1=Right_Max;//�����棬�����
		else if(uk1<Left_Max)   uk1=Left_Max;//������  
	


 		
   // if(( dianci3 > dianci4 )&&( dianci5 > dianci6 )&&( dianci5 <75))//|(dianci1-dianci2)>60Ϊʲô�������һ˲��ķ��仯
    if((dianci3 > 0)&&(right_minus_flag==1))   //������
    {
      pre_right_m = 1;        //��Ԥ�б�־ 1
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
    else if((dianci6 > 0)&&( left_minus_flag==1))//������
    {
      pre_right_m = 2;        //��Ԥ�б�־  2
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
   // else if((right_minus_flag ==1) && (left_minus_flag ==1)) //������
  //  {
  //    right_minus_flag = 0;
  //    left_minus_flag =0;
  //    stop_flag =1;
  //  }
    else
    {  
      pre_right_m=0;
      PWMDTY45=uk1; //���Բ����ٶȲ�
      dis = 0;
/*  #if se  
      motor_left.v_respect  = Tab*1.0/Max_Tab*100 + (uk1 - 118) * motor_left.K_spd_e;
      motor_right.v_respect =  Tab*1.0/Max_Tab*100 - (uk1 - 118) * motor_right.K_spd_e;
  #endif   */
    }
}  


void  analyse( uchar num )
{
   //40�����ݵķ�������
//   int daishuhe,jieduizhi,daishuhe1,jieduizhi1,jdz;
   uint zhibiaomin,zhibiaomax;
   uint deta = 200;
   
   uint zhibiao_dai_min ;
   uchar i_flag;//
   zhibiaomin=800;
   zhibiaomax=1000;
   zhibiao_dai_min = 500;

   
   //ÿ��һ������ˢһ��
   for(i_flag=79;i_flag>0;i_flag--)      //39
       weizhi_Buffer[i_flag]=weizhi_Buffer[i_flag-1] ; 
    weizhi_Buffer[0]=dianci3+dianci4-dianci5-dianci6;//weizhi_Buffer[0]=weizhi; 
   //ǰ20�����ݴ���
   
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
         daishuhe=daishuhe+weizhi_Buffer[i_flag];//��ǰ20����������
         jieduizhi=jieduizhi+jdz;                //��ǰ20��������ֵ��
     }
      
      //��20�����ݴ���
     for(i_flag=40;i_flag<80;i_flag++) 
     { 
         jdz=weizhi_Buffer[i_flag];     
         if(weizhi_Buffer[i_flag]<0)  
             jdz=-weizhi_Buffer[i_flag];
         daishuhe1=daishuhe1+weizhi_Buffer[i_flag]; //���20����������
         jieduizhi1=jieduizhi1+jdz;                 //���20��������ֵ��
         
     }
     
     if( daishuhe1 < 0 )
     {
      daishuhe1 = 0- daishuhe1;
     }
     if(daishuhe < 0)
     {
      daishuhe = 0- daishuhe;
     }
     
    // if(daishuhe1<zhibiaomin&&jieduizhi1<zhibiaomin&&daishuhe>zhibiaomax&&jieduizhi>zhibiaomax) //����  ����ʱ�൱������ ����    
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
        DDRK = 0x80; // PORTK input        // ��
        PORTK_PK7=1;
        PID_douji_para();
     }
 //  else if(daishuhe<20&&jieduizhi>80&&daishuhe1<20&&jieduizhi1>80) //s��     
  //     flag_mode=4; 
  #if 0
   else if(daishuhe<zhibiaomin&&jieduizhi<zhibiaomin&&daishuhe1<zhibiaomin&&jieduizhi1<zhibiaomin) //ֱ�� 
     {
         
      //  KP1=7;
        DDRK = 0x80; // PORTK input        // ��
        PORTK_PK7=1
        
       //blutooth(0);
      } 
   #endif 
   }
   


                  
    //��������ڹ�һ�����㷨�� �ж���������ģʽ
    //�ж���Ŀ̫�࣬���ܳ��ֻ����������⣬���������ϵı���
  
/*   if(dianci7>80|dianci8>80)  //7��8�Ŵ������ж�
   { 
        if(dianci7-dianci8<40) 
             flag_mode=1;// ʮ����
        else flag_mode=5; //ֱ����
   }*/
   //5,6�Ŵ������ж�
 
  // if(daishuhe<zhibiaomin&&jieduizhi<zhibiaomin&&daishuhe1>zhibiaomax&&jieduizhi1>zhibiaomax) //����  ����ʱ�൱�ڳ��� ���� 
    //   flag_mode=2; 
          
 //  else if(daishuhe>80&&jieduizhi>80&&daishuhe1>80&&jieduizhi1>80) //��� 
 //      flag_mode=7;
 //  else   flag_mode=8;  */  //���������ֹͣ�����ܲ�����
  
}



/*
void PD_duoji(void)     
{
  //uk1f=118;
   if(dianci3+dianci4>dianci5+dianci6) 
   {
      SpeedDet1=dianci3+dianci4-dianci5-dianci6;
       e11=(SpeedSet1-SpeedDet1);   
   }//��20�е�С�񵴣���30�������ȼ�С��������������
   else 
   {
       SpeedDet1=dianci5+dianci6-dianci3-dianci4;
       e11=(SpeedSet1+SpeedDet1); 
   }   
   
		//Ѳ����ʹSpeedset1�ӽ���0��С�����м�״̬��  dianci3+dianci4-dianci5-dianci6==0
	//	duk1=KP1*(e11-e21)+KI1*e11/8+KD1*(e11-2*e21+e31)/15; //�ӱ����ɹ�
		duk1=((e11-e21)+(KD1)*(e11-2*e21+e31))/(KP1);
   // uk1f=uk1f+e11/KP1;    //����
    
    
    uk1f=uk1f+duk1;
     e31=e21;
	   e21=e11;
    
   //��ȡPID���ڵ����� 
    uk1=uk1f;//���������uk1ȡֵ��ΧӦ��Ϊ��107--129),duk1ȡֵ��-11 -- 11��	
	  //PD�������
	  if(uk1>134)        uk1=134;//�����棬�������107--118--129��
		else if(uk1<103)   uk1=103;//������  
		
    if((dianci3>dianci4)&&(dianci5>dianci6)&&(dianci5<75))//|(dianci1-dianci2)>60Ϊʲô�������һ˲��ķ��仯
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
   
		
 //if((dianci3>dianci4&&dianci3<50))//|(dianci1-dianci2)>60Ϊʲô�������һ˲��ķ��仯
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

   //   PWMDTY45=uk1;  //��ʱ��ʹ��PID����
//	}
}       */