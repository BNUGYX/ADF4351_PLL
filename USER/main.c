#include "Freq_control.h"

int main(void)
{		  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Freq_control_init();
	while(1)
	{
		mode_select();
	}
}

