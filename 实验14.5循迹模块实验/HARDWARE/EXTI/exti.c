#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "exti.h"

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);  //使能SYSCFG时钟
	KEY_Init();       //初始化与按键连接的硬件接口
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource9); //设置IO口与中断线的映射关系
	
	//对外部中断进行设置
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);
	if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9) == 0)
	{
		LED1 = !LED1;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line9);
}


