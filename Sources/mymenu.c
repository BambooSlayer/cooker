#include"mymenu.h"

//----------------------flag------------------
uchar menu_flag=0;
uchar obj_flag=0;
uchar unchange_flag=0;//change? 0 --un,1--ch
uchar AD_Working=0;//0 free，1 busy，2 calculate
extern unsigned char state_flag;
//----------------------data------------------
uchar Tab;
//unsigned char key;

extern int error_max,error_min;
extern int curr_e;

void Init_Menu()
{
  LCD_clear();
  draw_lineX(6,0,46);
  draw_lineX(7,0,46);
  display_string(2,0,"Vmax & Vmin"); 
  display_string(2,1,"Z-derction");
  display_string(2,2,"Laoma");  
  display_string(2,3,"Let's Rock!");     
  display_char(0,0,31);
//  display_chinese(0,1,mark);    
//  display_chinese(0,2,mark); 
}

uchar scan_PB()
{
  uchar temp;
  DDRB = 0x00;
  temp = PORTB;
  return(temp);

}

void Display_Menu()
{
  if((unchange_flag==1) || (AD_Working==1))
  {
    unchange_flag=0;
    if(menu_flag==0)//主菜单下4个项
    {
      display_char(0,0,' ');
      display_char(0,1,' ');
      display_char(0,2,' ');
      display_char(0,3,' ');
      display_char(0,obj_flag,31);   ///////////////////define the max projects of menu_0
    }
    else if(menu_flag==1)//Vmax Vmin
    {
      LCD_clear();
    //  display_string(2,0,"Vmax & Vmin ");
      display_string(1,1,"ch2->ch5 ");
      Display_num(0,2,ad_last[2].AD_res);
      Display_num(5,2,ad_last[2].max);
      Display_num(10,2,ad_last[2].min);
      
      Display_num(0,3,ad_last[3].AD_res);
      Display_num(5,3,ad_last[3].max);
      Display_num(10,3,ad_last[3].min);
      
      Display_num(0,4,ad_last[4].AD_res);
      Display_num(5,4,ad_last[4].max);
      Display_num(10,4,ad_last[4].min);
      
      Display_num(0,5,ad_last[5].AD_res);
      Display_num(5,5,ad_last[5].max);
      Display_num(10,5,ad_last[5].min);
      
      
      AD_get(0,5);
      Delay_ms(10);
    }
    else if(menu_flag==2)
    {
      LCD_clear();
      display_string(2,0,"Z-derction-max");       
      
      Display_num(0,2,ad_last[0].AD_res);
      Display_num(5,2,ad_last[0].max);
      Display_num(10,2,ad_last[0].min);
      
      Display_num(0,3,ad_last[1].AD_res);
      Display_num(5,3,ad_last[1].max);
      Display_num(10,3,ad_last[1].min);
      
      AD_get(0,1);
      Delay_ms(10); 
    }
    else if(menu_flag==3)
    {
      LCD_clear();
      display_string(2,0,"Laoma"); 
    //  display_string(0,3,"Tap:");
    //  Display_num(6,3,Tab); 
 
    }
    else if(menu_flag==4)
    {
      LCD_clear();
      state_flag = 1;  
    }                
  }  
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 25 PORTH_ISR(void)
{
    unchange_flag=1;
    Delay_ms(5);	
    if(BGET(3,PTH)==0) 
    {
      BSET(3,PIFH);
 //     key=3;
      if( (menu_flag==1) ||(menu_flag==2)||(menu_flag==3 ) )
      {
        menu_flag=0;
        AD_Working=0;
        Init_Menu();
      }
    } 
    else if(BGET(2,PTH)==0) 
    {
      BSET(2,PIFH);
 //     key=2;
      if(menu_flag==0)
      {
        menu_flag=obj_flag+1;
      }
      else if(menu_flag==1)//config
      {
        AD_Working=1;   
      }
      else if(menu_flag==2)//config
      {
        AD_Working=1;   
      }
      else if(menu_flag==3)//config
      {
        Tab=scan_PB();
        if (Tab > Max_Tab)
        {
          Tab = Max_Tab;
        }
      }
    } 
    else if(BGET(1,PTH)==0) 
    {
      BSET(1,PIFH);
//      key=1;
      if(menu_flag==0)
      {
        obj_flag=(obj_flag>0)?(obj_flag-1):3;
      }
    } 
    else if(BGET(0,PTH)==0) 
    {
      BSET(0,PIFH);
//      key=0;
      if(menu_flag==0)
      {
        obj_flag=(obj_flag<3)?(obj_flag+1):0; ///////////////////define the max projects of menu_0
      }
    }
    PIFH = 0xFF;
  //清除中断标志位
  //  display_char(2,0,key+0x30);     
}
#pragma CODE_SEG DEFAULT