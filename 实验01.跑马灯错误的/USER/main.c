#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"

int main(void)
{
 delay_init(168);
	
 LED_Init();
	
	while(1){
			GPIO_SetBits(GPIOE,GPIO_Pin_3);
			GPIO_SetBits(GPIOE,GPIO_Pin_4);
			GPIO_ResetBits(GPIOG,GPIO_Pin_9);
		delay_ms(500);
			GPIO_SetBits(GPIOE,GPIO_Pin_3);
			GPIO_ResetBits(GPIOE,GPIO_Pin_4);
			GPIO_SetBits(GPIOG,GPIO_Pin_9);
	   delay_ms(500);			
			GPIO_ResetBits(GPIOE,GPIO_Pin_3);
			GPIO_SetBits(GPIOE,GPIO_Pin_4);
			GPIO_SetBits(GPIOG,GPIO_Pin_9);
	delay_ms(500);	
			GPIO_SetBits(GPIOE,GPIO_Pin_3);
			GPIO_SetBits(GPIOE,GPIO_Pin_4);
			GPIO_SetBits(GPIOG,GPIO_Pin_9);
	delay_ms(500);			
			GPIO_ResetBits(GPIOE,GPIO_Pin_3);
			GPIO_ResetBits(GPIOE,GPIO_Pin_4);
			GPIO_ResetBits(GPIOG,GPIO_Pin_9);
	delay_ms(500);
	 
	}

}

