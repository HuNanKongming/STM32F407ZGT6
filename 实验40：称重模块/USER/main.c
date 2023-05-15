#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "HX711.h"
#include "led.h"
//ALIENTEK ̽����STM32F407������ ʵ��0
//STM32F4����ģ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
//extern s32 Weight_Shiwu;
float Weight = 0;
int main(void)
{
	u32 t=0;
	uart_init(9600);
	delay_init(168);		//��ʱ��ʼ�� 
	Init_Hx711();
	
	delay_ms(2000);
	Get_Maopi();
	delay_ms(100);
	LED_Init();
  while(1){	
	Weight = Get_Weight();
	Weight = (float)Weight/1000.0f;
	printf("%0.3f kg\n",Weight);	//������ʾ����
	delay_ms(1000);
	}
}




