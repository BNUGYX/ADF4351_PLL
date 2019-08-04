#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "beep.h"

//��Ӣ���Դ�����
//������ʼ������
//KEY0:PE4	KEY1:PE3	WK_UP:PA0
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE4,3

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
}

//���4��4�������
//���ߣ���c1-c4:PG3-6;	��L1-L4:PG7-10
void MyKEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);		//���ø�
	GPIO_ResetBits(GPIOG,GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10);	//���õ�
}

//ϵͳ����������
//���ذ���ֵ
//	0��û���κΰ�������
//	1��KEY0����
//	2��KEY1����
//	3��KEY3���� WK_UP
//mode:0,��֧��������;1,֧��������;
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		BEEP_ON();		
		delay_ms(50);//ȥ���� 
		BEEP_OFF();
		key_up=0;
		if(KEY0==0)
		{
			while(KEY0==0);		//�ȴ������ɿ�
			return KEY0_PRES;
		}
		else if(KEY1==0)
		{
			while(KEY1==0);		//�ȴ������ɿ�
			return KEY1_PRES;
		}
		else if(WK_UP==1)
		{
			while(WK_UP==1);		//�ȴ������ɿ�
			return WKUP_PRES;
		}
	}
	else if(KEY0==1&&KEY1==1&&WK_UP==0)	key_up=1; 	    
 	return 0;// �ް�������
}

u8 f_m = 1; 
extern u8 change_appare;

u8 K_S(void)
{	 
	unsigned int  KeyValue = 16;		//û�а������µĳ�ֵ
	
	//ɨ���һ��
	GPIO_SetBits(GPIOG,GPIO_Pin_3);
	GPIO_ResetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);//0x01
	switch(GPIO_ReadInputData(GPIOG)&0x7f8)//1111 1111 000
	{
		case 0x88:KeyValue=1;		  break;   //PC   0x11
		case 0x108:KeyValue=4;		break;
		case 0x208:KeyValue=7;		break;
		case 0x408:KeyValue=14;		break;   //*
		default: break;
	}
	//ɨ��ڶ���
	GPIO_SetBits(GPIOG,GPIO_Pin_4);
	GPIO_ResetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6);
	switch(GPIO_ReadInputData(GPIOG)&0x7f8)
	{
		case 0x90:KeyValue=2;break;
		case 0x110:KeyValue=5;break;
		case 0x210:KeyValue=8;break;
		case 0x410:KeyValue=0;break;
		default: break;
	}
	//ɨ�������
	GPIO_SetBits(GPIOG,GPIO_Pin_5);
	GPIO_ResetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6);
	switch(GPIO_ReadInputData(GPIOG)&0x7f8)
	{
		case 0xa0:KeyValue=3;break;
		case 0x120:KeyValue=6;break;
		case 0x220:KeyValue=9;break;
		case 0x420:KeyValue=15;break; //#
		default: break;
	}
	//ɨ�������
	GPIO_SetBits(GPIOG,GPIO_Pin_6);
	GPIO_ResetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	switch(GPIO_ReadInputData(GPIOG)&0x7f8)
	{
		case 0xc0:KeyValue=10;break;  //A
		case 0x140:KeyValue=11;break; //B
		case 0x240:KeyValue=12;break; //C
		case 0x440:KeyValue=13;break; //D
		default: break;
	}
	GPIO_SetBits(GPIOG,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);		//���ø�
	GPIO_ResetBits(GPIOG,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10); //���õ�
	while((GPIO_ReadInputData(GPIOG)&0x7f8)!=0x78);					//�ȴ������ɿ�
	return KeyValue;
}

//���°�������1�����򷵻�0
u8 KEY_Down(void)
{
	if((GPIO_ReadInputData(GPIOG)&0x7f8)!=0x78)
	{ 
		return 1;//keydown
	}
		else return 0;
}

//����
u8 Scan_KEY_Board(void)
 {
		if(KEY_Down())
		{
			BEEP_ON();
			delay_ms(50);
			BEEP_OFF();
			if(KEY_Down())
			{
				return K_S();
			}
		} 
	return 16;
 }
