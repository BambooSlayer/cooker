#include "AD.h"
//unsigned int  dianji1,dianji2,dianji3,dianji4;//最大值4096   AD采样数据
//unsigned int dianci1,dianci2,dianci3,dianci4,dianci5,dianci6;

#define AD_LAST_MAX 100

uchar left_minus_flag  =0;
uchar right_minus_flag  =0;
uchar ch_start  ,ch_final ;
uchar ismid = 0;
uchar min_of_middle = 0;
int error_max,error_min;
extern uchar  pre_right_m;
#define CHANNEL_NUM 6

extern unsigned char state_flag;
MY_AD ad_last[6];
unsigned char finish_ad=0;
  int curr_e;
  unsigned int a1,a2,a3,a4,b1,b2,b3,b4;
 
//extern uchar cancel_AD ; //定义在SCI.c
  float ii;
  unsigned int bb;
/*{
              { ad_last[0].max=0; ad_last[0].min=4095;},
              { ad_last[1].max=0; ad_last[1].min=4095;},
              { ad_last[2].max=0; ad_last[2].min=4095;},
              { ad_last[3].max=0; ad_last[3].min=4095;},
              { ad_last[4].max=0; ad_last[4].min=4095;},
              { ad_last[5].max=0; ad_last[5].min=4095;} 
};    */

void Init_AD(void)
{
  ad_last[0].max=0;
  ad_last[0].min=4095;
  ad_last[1].max=0; 
  ad_last[1].min=4095;
  ad_last[2].max=0; 
  ad_last[2].min=4095;
  ad_last[3].max=0; 
  ad_last[3].min=4095;
  ad_last[4].max=0; 
  ad_last[4].min=4095;
  ad_last[5].max=0; 
  ad_last[5].min=4095;
   
}

void AD_start(void)//共用十路，四路为电机驱动电流检测AD0-AD3，六路电磁检测AD4-AD9
{
        ATD0CTL0=0X05;    //选择回绕通道为5
    	  ATD0CTL1=0x40+0x10;	  //外部触发源禁止，12位精度，采样前不放电      
        ATD0CTL2=0X40+0x02;   	//自动清零，下降沿触发  ，0x42时允许转换完成产生中断     
        ATD0CTL3=0XB0;    //右对齐，转换序列长度6 ,先进先出模式 0xB4    
        ATD0CTL4=0XC5;    //不知如何设置，可能有问题，采样周期10，F(ATDCLK)=F(BUS)/(2*(10101+1))=64/44MHZ     
        ATD0CTL5=0x30;    //连续转换模式，多通道转换，选择0X30通道0（0000）作为起始     
      	ATD0DIEN=0x00;    //禁止数字输入       	
}  
  
void AD_get(uchar ch_s,uchar ch_f)
{
    //六路磁场转换
    if( (ch_f - ch_s <= CHANNEL_NUM-1) && (ch_f>ch_s) && (ch_f< CHANNEL_NUM ))
    {
      
      ch_start = ch_s;
      ch_final = ch_f;
      AD_start();
      if(state_flag !=2 )
      {
        while(!finish_ad);
      } 
      finish_ad=0;
    }
}
#pragma CODE_SEG NON_BANKED 
interrupt 22 void AD_ISR() 
{
  static unsigned long temp[6]=0;
  static unsigned char times=0;
  unsigned char i,i2,i5;

  
 // DisableInterrupts
// times=0;
  //while( times<16)
  //{
  temp[0]= ATD0DR0+temp[0];
  temp[1]= ATD0DR1+temp[1];
  temp[2]= ATD0DR2+temp[2];  
  temp[3]= ATD0DR3+temp[3];  
  temp[4]= ATD0DR4+temp[4];  
  temp[5]= ATD0DR5+temp[5];
  times++;
    if(times==16)
    {
      times=0;
      ATD0CTL2=0X40;//stop
      for(i=ch_start; i<ch_final+1;i++)
      {
          if (state_flag==0)
          {        
            if((temp[i]>>4) < ad_last[i].min)
            {
              ad_last[i].min = temp[i]>>4;
            }
            else if( (temp[i]>>4) > ad_last[i].max )
            {
              ad_last[i].max = temp[i]>>4;
            }
            ad_last[i].AD_res = temp[i]>>4;
            temp[i]=0;
            finish_ad=1;
          }
          else if(state_flag==1)
          {
            ad_last[i].AD_res = temp[i]>>4;
            
            if(ad_last[i].AD_res >= ad_last[i].min && ad_last[i].AD_res<=ad_last[i].max)
            {
             // ii =  (ad_last[i].AD_res- ad_last[i].min) * AD_LAST_MAX * 1.0;
              
             // ad_last[i].last =ii/(ad_last[i].max - ad_last[i].min); 
             ii = (ad_last[i].AD_res-ad_last[i].min)*1.0*100 ;
             bb = (ad_last[i].max-ad_last[i].min);
             ad_last[i].last=ii/bb; 
             
          //    left_minus_flag=0;
          //    right_minus_flag=0;
            }
            else if(ad_last[i].AD_res > ad_last[i].max )
            {
              ad_last[i].last =  AD_LAST_MAX;

            }
           else if(ad_last[i].AD_res < ad_last[i].min)
            {
              if (i ==2 )
              {
             // left_minus_flag = 1;
                i2=1;
          //    right_minus_flag=0;
              } 

              if (i ==5)
              {
           //   right_minus_flag = 1;
                i5=1;
         //     left_minus_flag=0;
              }

              ad_last[i].last = 0;
            }
             temp[i]=0;
             finish_ad=1;
          }   
          

      }//for
      if(state_flag == 1)
      {
            min_of_middle = (ad_last[2].last<ad_last[5].last)? 2:5;
            if(i2==1&&i5!=1 && ismid ==1)
            {
               ismid = 0;
               left_minus_flag=1;
               right_minus_flag=0;
            }
             else if(i2!=1&&i5==1 && ismid ==1 )
            {
               ismid = 0;
               left_minus_flag=0;
               right_minus_flag=1; 
            } 
            else if((i2!=1&&i5!=1) && (ad_last[3].last > ad_last[min_of_middle].last) && (ad_last[4].last > ad_last[min_of_middle].last))
            {
               ismid = 1;
               left_minus_flag=0;
               right_minus_flag=0;
            } 
            
            
            i2=0;i5=0;
      } 
      else if(state_flag == 0)
      {
            
            curr_e = (ad_last[2].AD_res + ad_last[3].AD_res) -(ad_last[4].AD_res+ad_last[5].AD_res);
            if( error_max< curr_e )
            {
              error_max = curr_e ;
              a1 = ad_last[2].AD_res;
              a2 = ad_last[3].AD_res;
              a3 = ad_last[4].AD_res;
              a4 = ad_last[5].AD_res;
            }
            
            if( error_min > curr_e)
            {
              error_min = curr_e;
              b1 = ad_last[2].AD_res;
              b2 = ad_last[3].AD_res;
              b3 = ad_last[4].AD_res;
              b4 = ad_last[5].AD_res;        
            }
            
            
      }
      
/*      if( left_minus_flag == 1 && right_minus_flag == 1)
      {
        left_minus_flag = 0;
        right_minus_flag = 0;
      }  */
      
    }
  //}
 // EnableInterrupts 
 
 //ATD0CTL2=0X40;
}
