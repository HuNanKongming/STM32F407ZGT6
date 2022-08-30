#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"

extern uint8_t counter;
uint8_t val = 180;
uint8_t dir = 0;
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    LED_Init();
    TIM3_Int_Init(1, 167);

    while(1)
    {
        if(dir)val++;//dir==1 led0pwmval递增
        else val--;	//dir==0 led0pwmval递减
        if(val > 200)dir = 0; //led0pwmval到达300后，方向为递减
        if(val == 0)dir = 1;	//led0pwmval递减到0后，方向改为
        delay_ms(10);

    }

}


/**
*******************下面注释掉的代码是通过 位带 操作实现IO口控制**************************************

int main(void)
{

	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
  while(1)
	{
     LED0=0;			  //LED0亮
	   LED1=1;				//LED1灭
		 delay_ms(500);
		 LED0=1;				//LED0灭
		 LED1=0;				//LED1亮
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/

/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{

	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0亮
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1灭
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0灭
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1亮
		 delay_ms(500);

	 }
 }
**************************************************************************************************
**/




