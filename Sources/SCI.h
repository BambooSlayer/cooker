#ifndef __SCI_H_
#define __SCI_H_


#include "inc.h"
#include "AD.h"
//声明函数
void SCI_Init (void);//串口初始化
void SCI_Transmit_char(unsigned char c);//发送字符
void SCI_Transmit_str(char ch[]);//发送字符串
//void blutooth(unsigned int led); //拆分
void showb(void);//将数据以一定格式发送到PC
unsigned char SCI_Receive(void);//串口接收

extern unsigned char CTRL_Msg_FROM_PC;

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

#endif