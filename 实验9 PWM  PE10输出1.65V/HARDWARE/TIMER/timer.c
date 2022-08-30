#include "timer.h"
#include "led.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStrue.TIM_Period=arr;
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//Ê¹ÄÜGPIO
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        
	GPIO_Init(GPIOE,&GPIO_InitStructure);              
	
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(void)
{
//		int counter=0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		if(~(GPIOE->IDR)& GPIO_IDR_IDR_10)
		{
		GPIO_SetBits(GPIOE,GPIO_Pin_10);
		}
		else
		GPIO_ResetBits(GPIOE,GPIO_Pin_10);
	}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
	
	
	//		delay_ms(1);
//		counter++;
//		if(counter > 300)
//		{
//			counter=0;
//		}
//		if(counter>200)
//		{
//			GPIO_SetBits(GPIOE,GPIO_Pin_10);
//		}
//		else
//		{
//			GPIO_ResetBits(GPIOE,GPIO_Pin_10);
//		}
//		
//	}		
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

//		if(~(GPIOE->IDR)& GPIO_IDR_IDR_10)
//		{
//		GPIO_SetBits(GPIOE,GPIO_Pin_10);
//			delay_ms(1);
//		}
//		if((GPIOE->IDR)& GPIO_IDR_IDR_10)
//		else
//		GPIO_ResetBits(GPIOE,GPIO_Pin_10);
//	}
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//		delay_ms(1);

