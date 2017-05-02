#ifndef NOKIA_5110_H
#define NOKIA_5110_H

#include<stdio.h>
#include<math.h>


#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

extern const uchar mark[];
extern const uchar dianya[];
extern const uchar sheding[];

void LCD_Init(void);
void write_command(uchar cmd);
void write_data(uchar dat);
void set_pos(uchar x,uchar y);
void display_char(uchar x,uchar y,uchar c); // x:列 y:行
void display_string(uint x,uint y,uchar *str);
void dispaly_float(uchar x,uchar y,float f);
void LCD_clear(void);
void display_chinese(uchar x,uchar y, const uchar *str);
void display_chinese_string(uchar x,uchar y, uchar start, uchar n,const uchar *str);

//void show_char(uchar x,uchar y,uchar c);
//void show_string(uchar x,uchar y,uchar *str);

void draw_dot(uchar x,uchar y);//有缺陷，可以用于纵坐标重复的点,x,y表示坐标
void draw_line(int x1,int y1,int x2,int y2);
void draw_lineX(uchar x,uchar y1,uchar y2);
void draw_lineY(uchar x1,uchar x2,uchar y);
//void draw_circle(uchar x0,uchar y0,uchar r);

//特殊用途函数
void Display_num(uchar x,uchar y,uint num);
void Display_f(uchar x,uchar y,uint num) ;
void Display_signednum(uchar x,uchar y,int num);

#endif
