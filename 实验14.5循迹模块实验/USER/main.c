#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"


//ALIENTEK ̽����STM32F407������ ʵ��3
//��������ʵ��-�⺯���汾 
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 


int main(void)
{ 
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  u8 key;           //�����ֵ
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿� 
	uart_init(115200); 	//���ڳ�ʼ�� 
	KEY_Init();       //��ʼ���밴�����ӵ�Ӳ���ӿ�
	LED0=0;				  	//�ȵ������
	EXTIX_Init();
	Tracking_Init();
	while(1)
	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3) == 1)
//		{
			printf("%d\r\n",GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3));
//		}
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3) == 0)
//		{
//			printf("0\r\n");
//		}
//		printf("OK!\r\n");
//		delay_ms(1000);
	}

}
