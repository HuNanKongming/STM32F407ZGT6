#include "tim.h"
#include "led.h"
#include "gpiod.h"
#include "stm32f4xx_tim.h"                 

void TIM1_PWM_Init(uint32_t arr,uint32_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//ʹ�ܶ�ʱ��Eʱ��
    //ʹ��GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//���츴�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;     //GPIOE9
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //�ٶ�100MHz
    GPIO_Init(GPIOE,&GPIO_InitStructure);  //��ʼ��PE9

    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);  //GPIOE����Ϊ��ʱ��1
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); //TIM1ʱ��ʹ��
    
    //ʱ����ʼ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr - 1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  //��ʼ����ʱ��1

	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
    TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 

    TIM_Cmd(TIM1,ENABLE);  //ʹ�ܶ�ʱ��1
    TIM_CtrlPWMOutputs(TIM1,ENABLE); //
}

