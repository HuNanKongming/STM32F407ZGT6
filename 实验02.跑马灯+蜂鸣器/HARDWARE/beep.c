#include "beep.h"
#include "stm32f4xx.h"

void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);  //??GPIOG??
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;       //你真的帅哥嘛
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;		//??????
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	//????
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;		//???????
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;	//100hz
	GPIO_Init(GPIOG,&GPIO_InitStructure);	
	GPIO_ResetBits(GPIOG,GPIO_Pin_7);
}

