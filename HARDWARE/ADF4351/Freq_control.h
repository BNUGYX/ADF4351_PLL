#ifndef _FREQ_CONTROL_H_
#define _FREQ_CONTROL_H_

#include "sys.h"

void Freq_control_init(void);			//PLL���Ƶ�ʿ���ϵͳ��ʼ��
void mode_select(void);						//����ģʽѡ��
void auto_scan_mode(void);				//ģʽһ���Զ�ɨ��ģʽ
void self_scan_mode(void);				//ģʽ�����ֶ�ɨ��ģʽ
void single_freq_mode(void);		//ģʽ�������õ�Ƶ


#endif