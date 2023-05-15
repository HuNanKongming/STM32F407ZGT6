#include "tim.h"
#include "led.h"
#include "gpiod.h"
#include "stm32f4xx_tim.h"                 

void TIM1_PWM_Init(uint32_t arr,uint32_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//使能定时器E时钟
    //使能GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽复用输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;     //GPIOE9
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //速度100MHz
    GPIO_Init(GPIOE,&GPIO_InitStructure);  //初始化PE9

    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);  //GPIOE复用为定时器1
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); //TIM1时钟使能
    
    //时基初始化
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr - 1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  //初始化定时器1

	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
    TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 

    TIM_Cmd(TIM1,ENABLE);  //使能定时器1
    TIM_CtrlPWMOutputs(TIM1,ENABLE); //
}

