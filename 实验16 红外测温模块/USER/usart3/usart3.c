#include "usart3.h"

void TIM3_PWM_Init()
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	 //TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOA0复用为定时器4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;           //引脚为12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PD12          
	  
	TIM_TimeBaseStructure.TIM_Prescaler=839;  //定时器分频   //100kHZ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1999;      //自动重装载值   // 1/100MS *2000=20MS
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;       //初始化计数
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);  //初始化定时器4
	
	//初始化TIM5 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //选择定时器模式:TIM脉冲宽度调制模式1   CCR<period 为有效电平
		TIM_OCInitStructure.TIM_Pulse=50;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平为高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);   //根据T指定的参数初始化外设TIM4 OC1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);   //根据T指定的参数初始化外设TIM4 OC2
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIM4在CCR2上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);  
	TIM_Cmd(TIM3, ENABLE);  //使能TIM4		
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
}  


