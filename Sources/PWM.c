#include "PWM.h"
uchar f;
/*****************PWM��ʼ��***********************/
void pwm_init(void)
{
  PWMPRCLK=0X77;  //Ԥ��Ƶ   ��Ƶ   A=B=E/128=500khz    
  PWMSCLA=0x01;  //���Ʒ�Ƶ    SA=A/(2*PWMSCLA)=A/2=250KHZ  A���ڶ��
  PWMSCLB=0x01;    //ͬ��      SB=250khz
  PWMCLK=0x30;   //ѡ��ʱ��Դ  SA SB    45ѡ��SA,   23B,01A
  PWMPOL=0xFF;    //������ʼ��ƽ
  PWMCAE=0xFF;    //���ƾ��У��󣩶���
  PWMCTL=0x40;    //����ͨ���ļ���������ģʽ    45ͨ������
  //��� 1k������Ƶ�� PWMDTY PWMPER       PWMCNTX�����Ĵ���   ���F=SA/(2*PWMPER45)=50HZ
                                                             //���F=B/(2*PWPMERN)=1khz
  PWMDTY3=0;  //���fre =  1k  
  PWMPER3=250;
  PWMDTY2=0;  //��� 1k     ��ʼ�ٶ�Ϊ0
  PWMPER2=250;
  PWMDTY1=0;  //��� 1k   
  PWMPER1=250;
  PWMDTY0=0;  //��� 1k     ��ʼ�ٶ�Ϊ0
  PWMPER0=250; 
  
  PWMDTY45=118; //��� 50hz    ���   һ��ʼ0��ƫת  ���ƫת  106- 118-130 ,+12/-12
  PWMPER45=2500;
  
   //ȫ��ʹ�� PWME
  PWME_PWME3=1;   //���ͨ��
  PWME_PWME2=1;
  PWME_PWME1=1;   //���ͨ��
  PWME_PWME0=1;
  PWME_PWME5=1;   //���ͨ��
      
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
		            	 break;    //���� �ٶ�50%  0ƫ��
		            	 
		   case 0x31:   PWMDTY3=0;
		                PWMDTY2=62; 
		                PWMDTY1=0;
		                PWMDTY0=62; 
		                f=0;              
		            	 break;    //ǰ�� 100%
		            	 
		   case 0x32:  //PWMDTY45=111;
		   
		               PWMDTY3=0;
		               PWMDTY2=30;
		               PWMDTY1=0;
		               PWMDTY0=30;
		   
		            		              	          
		               break;    //��ת
		    
		   case 0x33:  PWMDTY3=0;
		               PWMDTY2=90;
		               PWMDTY1=0;
		               PWMDTY0=90;
		               f=1;		               		               
		               break;    //����  100%
		    
		   case 0x34:  //PWMDTY45=125;
		   
		                PWMDTY3=0;
		               PWMDTY2=60;
		               PWMDTY1=0;
		               PWMDTY0=60;
		   		             	 
		               break;		 //��ת  
		   
		   case 0x35: PWMDTY3=0;
		              PWMDTY2=0;
		              PWMDTY1=0;
		              PWMDTY0=0;
		              PWMDTY45=118;	 
		              break;//ֹͣ
		 }
}*/
