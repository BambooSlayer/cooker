#include "PIT.h"
#include "nokia_5110.h"
#include "AD.h"
#include "coder.h"
#include"counter.h"
#include"strategy.h"


extern MY_Coderdata MYCoder;
extern uchar state_flag;

unsigned char data_finished = 0;
void PITint(void)     //��ʱʱ��=��PITMTLD+1)*(PITLD+1)/fBUS=100*40000/(64*10^6)=1/16s=1000/16=250/4=62.5ms�P
{
   PITCFLMT=0X00;     //PITģ���ֹ 
   PITTF=0X01;        //��ʱ��־�Ĵ���д��0ͨ����ʱ
   PITMUX=0X00;        //16λ��ʱ������΢ʱ���׼0
   PITMTLD0=63;        //8λ�ݼ�������װ���ֵ
   PITLD0=5000;       //16λ����������������  1000 1ms ;��Ϊ5000��ʱ5ms 
   PITINTE=0X01;       // 0ͨ���ж�ʹ��
   PITCE=0X01;         //0ͨ��ʹ��
   PITCFLMT=0X80;      // PITģ��ʹ��
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED    //�жϺ������ڷǷ�ҳ����
interrupt 66 void PIT0_ISR() //ͨ��0���ж�����Ϊ66 ����Ϊ67��68��69
{
  static uchar PIT_COUNTER = 0;
   EnableInterrupts      //�ڵ����ȼ��ж����ٿ�һ�����жϿ���
  //  PITCE=0X00;
   PITTF=0X01;              //�����־λ
   
   if(PIT_COUNTER == N)
   {
     PIT_COUNTER=0;
   } 
   else
   {
     PIT_COUNTER ++;
   }
   
   
 //*******�ų����ģ��************************AD���   
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
      PWMDTY2=0;  //��� 1k   �ٶ����70%    ��    ��ʼ�ٶ�Ϊ0
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
     //     AD_average()  ; //�����ε�ƽ��ֵ
      //    PD_duoji() ; //40msһ���޸�        s
      //    ii=0       ;
      //    AD_sumstart() ;//�ۼӺ���0 
      // } 
      
    
    
   //***************flashд���ӳ���***********************
   //ʱ��滮ʱ���ԣ����ų������Flashд�����һ��
   
/*   FLAG++; 
   if(FLAG==4) 
   {          
        FLAG=0;
   }   */
//   PITCE=0X01; 
///EnableInterrupts  
}
#pragma CODE_SEG DEFAULT   //������������Ĭ��������