#include "SCI.h"
#include "coder.h"
#include "counter.h"
#include "strategy.h"
#include "PIT.h"
#include "math.h"
extern uchar dis;

extern unsigned int ant_speed1  ;
extern unsigned int ant_speed2  ;
unsigned char led1,led2,led3,led4;
unsigned char CTRL_Msg_FROM_PC=50;
extern unsigned char state_flag ;
void Init_Menu();

extern MY_Coderdata MYCoder;
extern MOTOR motor_left;

extern float KP1,KI1,KD1;

extern int daishuhe,jieduizhi,daishuhe1,jieduizhi1,uk1;

//uchar cancel_AD  = 0;

void sudu(unsigned int x);

void Start_delay(unsigned int n) 
{
  unsigned int i,j;
  while(n --)
  {
    for(i = 0;i < 1000;i++)
    for(j = 0;j < 1000; j++); 
  }
}
//***************************************函数***************************************************
/*
************************************************************************************************
*函数：void SCI_Init(void)
*参数：0
*返回：0
*功能：串口初试化
************************************************************************************************
*/
void SCI_Init(void) 
{                                   
  /*--UART0--*/
  
  SCI0CR2_RE = 1;//允许接收              
  SCI0CR2_TE = 1;//允许发送
  SCI0CR2_RIE= 1;//接收数据寄存器满,中断允许 
  
  //SCI0BDH=0x00;      
  //SCI0BDL=0x9C;//24M   9600
  //SCI0BDL=0x0d;//24M   115200
  //SCI0BDL=24000000/(115200*16);//24M  115200   
  //SCI0BDL=24000000/(57600*16);//24M  57600   
  //SCI0BDL=36000000/(57600*16);//36M  57600     
  //SCI0BDL=0xEA;//36M 9600
  //SCI0BDL=0x75;//36M  19200   
  //SCI0BDL=0x3A;//36M  38400 
  //SCI0BDL = 64000000/(9600*16); //xxM/(波特率*16)   
  //SCI0BDH=0x01;//40M 9600 
  //SCI0BDL=0x04;
  //SCI0BDH=0x01;//64M 9600     
  //SCI0BDL=0xA0;
  
  SCI0BDH=0x00;//64M 57600     
  SCI0BDL=0x45;
  
  /*--UART1--*/ 
}
/*
************************************************************************************************
*函数：void SCI_Transmit_char( unsigned char c)
*参数：c:要发送的字符
*返回：0
*功能：串口发送字符
************************************************************************************************
*/
void SCI_Transmit_char( unsigned char c)
{
  while(!(SCI0SR1&0x80));//keep waiting when not empty  
  SCI0DRL = c;
}
/*
************************************************************************************************
*函数：void SCI_Transmit_str(char ch[])
*参数：ch[]:要发送的字符串
*返回：0
*功能：串口发送字符串
************************************************************************************************
*/
void SCI_Transmit_str(char ch[])
{
  unsigned char ptr = 0;
  while(ch[ptr])
  {
	  SCI_Transmit_char((unsigned char)ch[ptr++]);
  } 
}
/*
************************************************************************************************
*函数：void  blutooth(unsigned int  led) 
*参数：led
*返回：0
*功能：数据拆分
************************************************************************************************
*/
void  blutooth(unsigned int  led) 
{               
      switch(led)				           //显示十位
		 {
		   case 0:  SCI_Transmit_str("0");	 break;
		   case 1:  SCI_Transmit_str("1");	 break;
		   case 2:  SCI_Transmit_str("2");	 break;
		   case 3:  SCI_Transmit_str("3");	 break;
		   case 4:  SCI_Transmit_str("4");	 break;
		   case 5:  SCI_Transmit_str("5");	 break;
		   case 6:  SCI_Transmit_str("6");	 break;
		   case 7:  SCI_Transmit_str("7");	 break;
		   case 8:  SCI_Transmit_str("8");	 break;
		   case 9:  SCI_Transmit_str("9");	 break;
		  
		 }
}
/*
************************************************************************************************
*函数：void show()
*参数：:要发送速度值
*返回：0
*功能：串口发送字符串 ,规定格式
************************************************************************************************
*/
void showb(void)//发送一行数据
{   
   /*     Start_delay(1);
        sudu(ad_last[0].last);   //AD1
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");
        
        Start_delay(1);
        sudu(ad_last[1].last);   //AD2
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1); 
        SCI_Transmit_str(",");      */
# if 0      
      //  Start_delay(1);
        sudu(MYCoder.spdl);   //AD3
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");
        
     //   Start_delay(1);
        sudu(MYCoder.spdr);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
 #endif
 
 # if 0      
      //  Start_delay(1);
        sudu(daishuhe);   //AD3
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");
        
     //   Start_delay(1);
        sudu(jieduizhi);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
        
        sudu(daishuhe1);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
        
        sudu(jieduizhi1);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
        
        sudu(abs(jieduizhi-jieduizhi1));   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
        
        
 #endif 
 
 #if 0
 
         sudu(motor_left.K_spd_e*10);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
        
        sudu(motor_right.K_spd_e*10);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
 
 #endif 
 
 
  # if 0      
        
     //   Start_delay(1);
        sudu(KP1);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
        
        sudu(KI1);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
        
        sudu(KD1*10);   //AD4
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");     
        
 #endif 
        
  /*      Start_delay(1);
        sudu(ad_last[4].last);   //AD5
        blutooth(led4);
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");  
 
        Start_delay(1);                
        sudu(ad_last[5].last);   //AD6
        blutooth(led4);
        blutooth(led3);             
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");      */

# if 0  
     
        sudu(MYCoder.spdl);   //编码器1
     //   Start_delay(2);
        blutooth(led4);    
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");
        
        sudu(MYCoder.spdr);   //编码器1
     //   Start_delay(2);
        blutooth(led4);    
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");
 
 
#endif   

# if 1  
     
        sudu(ad_last[0].last);   //编码器1
     //   Start_delay(2);
        blutooth(led4);    
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");
        
        sudu(ad_last[1].last);   //编码器1
     //   Start_delay(2);
        blutooth(led4);    
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(",");
        
        sudu(abs(ad_last[0].last-ad_last[1].last));   //编码器1
     //   Start_delay(2);
        blutooth(led4);    
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(","); 
#endif
#if 0

        sudu(abs(dis));   //编码器1
     //   Start_delay(2);
        blutooth(led4);    
        blutooth(led3);
        blutooth(led2);
        blutooth(led1);
        SCI_Transmit_str(","); 
#endif
    

 
        SCI_Transmit_str("\r\n"); //换行                
}

void sudu(unsigned int x) 
{
		   led4=x/1000;     //提取各位数值                               
		   led3=x%1000/100; //提取各位数值 
		   led2=x%100/10;   //提取各位数值 
		   led1=x%10;       //提取各位数值 
}
/*
************************************************************************************************
*函数nsigned char SCI_Receive(void)
*参数：0
*返回：接收到的字符
*功能：串口接收
************************************************************************************************
*/
/*unsigned char SCI_Receive(void)
{
  unsigned char ch;
  if(SCI0SR1_RDRF==1)//判断是否接收到数据
  {
    SCI0SR1_RDRF = 0;
    ch = SCI0DRL;
  }
  return(ch);
}  */
#pragma CODE_SEG __NEAR_SEG NON_BANKED    //中断函数置于非分页区内
interrupt VectorNumber_Vsci0 void SCI0_ISR() 
{
 // EnableInterrupts      //在低优先级中断里再开一次总中断可以
  if(SCI0SR1_RDRF==1)//判断是否接收到数据
  {
    SCI0SR1_RDRF = 0;
    CTRL_Msg_FROM_PC = SCI0DRL;
  }
  
  if( state_flag ==1 && CTRL_Msg_FROM_PC ==0x00)
  {
      state_flag = 2; 
    //  cancel_AD = 1;//在main。c，AD.c中引用
    
  } 
  else if(state_flag == 2)
  {
    if(CTRL_Msg_FROM_PC == 0xFF)
    {
       state_flag = 1;     
    } 
    else
    {
        PID_now();
    }
  }

}
#pragma CODE_SEG DEFAULT   //后续代码置于默认区域内

