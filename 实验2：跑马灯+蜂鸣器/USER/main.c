#include "led.h"
#include "sys.h"
#include "beep.h"
#include "delay.h"
#include "usart.h"
int main()
{
  delay_init(168);
	LED_Init();
	BEEP_Init();
	while(1)
	{  
		GPIO_ResetBits(GPIOG,GPIO_Pin_7);		
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
    GPIO_ResetBits(GPIOG, GPIO_Pin_9);
    delay_ms(500);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    GPIO_SetBits(GPIOG, GPIO_Pin_9);
    delay_ms(500);
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
    GPIO_SetBits(GPIOG, GPIO_Pin_9);
    delay_ms(500);
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    GPIO_ResetBits(GPIOG, GPIO_Pin_9);
    delay_ms(500);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
    GPIO_SetBits(GPIOG, GPIO_Pin_9);
    delay_ms(500);
		GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    GPIO_ResetBits(GPIOG, GPIO_Pin_9);
		GPIO_SetBits(GPIOG,GPIO_Pin_7);
		    delay_ms(500);

	}

}

