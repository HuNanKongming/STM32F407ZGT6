#include "led.h"
#include "stm32f4xx.h"
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  // use GPIO.clork
	//PF3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed;
	 GPIO_Init(GPIOE,&GPIO_InitStructure);	
GPIO_SetBits(GPIOE,GPIO_Pin_3);
	//PF4
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed;
		 GPIO_Init(GPIOE,&GPIO_InitStructure);	
GPIO_SetBits(GPIOE,GPIO_Pin_4);
	//PG9
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed;
		 GPIO_Init(GPIOG,&GPIO_InitStructure);	
GPIO_SetBits(GPIOG,GPIO_Pin_9);
	
	
}

