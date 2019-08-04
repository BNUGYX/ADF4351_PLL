#include "Freq_control.h"
#include "ADF4351.h"
#include "key.h"
#include "lcd.h"
#include "delay.h"
#include "beep.h"

//ϵͳ��ʼ��
void Freq_control_init(void)
{
	delay_init();	    //��ʱ������ʼ��	  
	BEEP_Init();			//��������ʼ��
	KEY_Init();				//���ذ�����ʼ��
	MyKEY_Init();			//4��4������̳�ʼ��
	LCD_Init();			  //LCD��ʼ��
	ADF4351Init();		//ADF4351ģ���ʼ��
	LCD_ShowString(60,100,120,24,24,"Frequency:");
}

//����ģʽѡ��
void mode_select(void)
{
	u8 mode = Scan_KEY_Board();
	switch(mode)
	{
		case 10:auto_scan_mode();	break;			//A
		case 11:self_scan_mode();	break;			//B
		case 12:single_freq_mode(); break;		//C	
		default:  break;
	}
}

//ģʽһ���Զ�ɨ��ģʽ
void auto_scan_mode(void)
{
	u16 freq = 900;			//Ƶ����ʼֵ��90.0MHz
	u8 key_value = 16;
	
	while(1)
	{
		ADF4351WriteFreq(freq++);
		if(freq >= 1101)		//����110MHz
		{
			freq = 900;
		}
		delay_ms(10);				//ÿһ��Ƶ���פ��ʱ��
		key_value = Scan_KEY_Board();
		if(key_value == 15)
		{
			break;
		}
	}
	
}	

//ģʽ�����ֶ�ɨ��ģʽ
void self_scan_mode(void)
{

}

//ģʽ�������õ�Ƶ
void single_freq_mode(void)
{

}	


//		key = KEY_Scan(0);
//		key_value = Scan_KEY_Board();
//		switch(key)
//		{
//			case KEY0_PRES:LCD_ShowString(200,300,56,24,24,"KEY0");break;
//			case KEY1_PRES:LCD_ShowString(200,300,56,24,24,"KEY1");break;
//			case WKUP_PRES:LCD_ShowString(200,300,56,24,24,"WK_UP");break;
//			default:break;
//		}
//		switch(key_value)
//		{
//			case 0:LCD_ShowString(200,300,56,24,24,"0");break;
//			case 1:LCD_ShowString(200,300,56,24,24,"1");break;
//			case 2:LCD_ShowString(200,300,56,24,24,"2");break;
//			case 3:LCD_ShowString(200,300,56,24,24,"3");break;
//			case 4:LCD_ShowString(200,300,56,24,24,"4");break;
//			case 5:LCD_ShowString(200,300,56,24,24,"5");break;
//			case 6:LCD_ShowString(200,300,56,24,24,"6");break;
//			case 7:LCD_ShowString(200,300,56,24,24,"7");break;
//			case 8:LCD_ShowString(200,300,56,24,24,"8");break;
//			case 9:LCD_ShowString(200,300,56,24,24,"9");break;
//			case 10:LCD_ShowString(200,300,56,24,24,"A");break;
//			case 11:LCD_ShowString(200,300,56,24,24,"B");break;
//			case 12:LCD_ShowString(200,300,56,24,24,"C");break;
//			case 13:LCD_ShowString(200,300,56,24,24,"D");break;
//			case 14:LCD_ShowString(200,300,56,24,24,"*");break;
//			case 15:LCD_ShowString(200,300,56,24,24,"#");break;
//			default:break;
//		}
