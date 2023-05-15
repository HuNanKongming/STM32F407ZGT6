#include "usart3.h"

void TIM3_PWM_Init()
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	 //TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��PORTAʱ��	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOA0����Ϊ��ʱ��4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;           //����Ϊ12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PD12          
	  
	TIM_TimeBaseStructure.TIM_Prescaler=839;  //��ʱ����Ƶ   //100kHZ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=1999;      //�Զ���װ��ֵ   // 1/100MS *2000=20MS
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;       //��ʼ������
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);  //��ʼ����ʱ��4
	
	//��ʼ��TIM5 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1   CCR<period Ϊ��Ч��ƽ
		TIM_OCInitStructure.TIM_Pulse=50;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //��Ч��ƽΪ��
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);   //����Tָ���Ĳ�����ʼ������TIM4 OC1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);   //����Tָ���Ĳ�����ʼ������TIM4 OC2
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);  
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM4		
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
}  


