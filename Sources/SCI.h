#ifndef __SCI_H_
#define __SCI_H_


#include "inc.h"
#include "AD.h"
//��������
void SCI_Init (void);//���ڳ�ʼ��
void SCI_Transmit_char(unsigned char c);//�����ַ�
void SCI_Transmit_str(char ch[]);//�����ַ���
//void blutooth(unsigned int led); //���
void showb(void);//��������һ����ʽ���͵�PC
unsigned char SCI_Receive(void);//���ڽ���

extern unsigned char CTRL_Msg_FROM_PC;

//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

#endif