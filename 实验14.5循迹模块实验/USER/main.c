#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"


//ALIENTEK 探索者STM32F407开发板 实验3
//按键输入实验-库函数版本 
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 


int main(void)
{ 
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  u8 key;           //保存键值
	delay_init(168);  //初始化延时函数
	LED_Init();				//初始化LED端口 
	uart_init(115200); 	//串口初始化 
	KEY_Init();       //初始化与按键连接的硬件接口
	LED0=0;				  	//先点亮红灯
	EXTIX_Init();
	Tracking_Init();
	while(1)
	{
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3) == 1)
//		{
			printf("%d\r\n",GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3));
//		}
//		if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3) == 0)
//		{
//			printf("0\r\n");
//		}
//		printf("OK!\r\n");
//		delay_ms(1000);
	}

}
