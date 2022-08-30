#include "led.h"
#include "stm32f4xx.h"

void LED_Init(void)
{
	
 GPIO_InitTypeDef GPIO_InitStructure;
	
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	//F3
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
 GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	
	//F4
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
 GPIO_Init(GPIOE,&GPIO_InitStructure);	
GPIO_SetBits(GPIOE,GPIO_Pin_4);

	//FG 9
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
 GPIO_Init(GPIOG,&GPIO_InitStructure);	
GPIO_SetBits(GPIOG,GPIO_Pin_9);

}


