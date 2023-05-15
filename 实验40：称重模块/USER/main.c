#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "HX711.h"
#include "led.h"
//ALIENTEK 探索者STM32F407开发板 实验0
//STM32F4工程模板-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
//extern s32 Weight_Shiwu;
float Weight = 0;
int main(void)
{
	u32 t=0;
	uart_init(9600);
	delay_init(168);		//延时初始化 
	Init_Hx711();
	
	delay_ms(2000);
	Get_Maopi();
	delay_ms(100);
	LED_Init();
  while(1){	
	Weight = Get_Weight();
	Weight = (float)Weight/1000.0f;
	printf("%0.3f kg\n",Weight);	//串口显示重量
	delay_ms(1000);
	}
}




