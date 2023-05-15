#include "led.h"
#include "lcd.h"
#include "key.h"
#include "stdio.h"
#include "dht11.h"
#include "usart.h" 
/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程4 LCD显示主函数main.c                                            *
* 文件简述：LCD显示实验                                                          *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：LCD显示字符、数字和图形                                              *
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{
  u16 i=1;
  u16 num;
	u8 t = 0;
	u8 temperatre;
	u8 humidity;
  u8 buf[100] = "0123456789";//字符数组，可以保存字符串
  
	delay_init();         //初始化延时函数
  LED_Init();					  //初始化LED
  LCD_Init();           //初始化LCD FSMC接口和显示驱动
  KEY_Init();
	uart_init(115200);
  BRUSH_COLOR=RED;      //设置画笔颜色为红色



  LCD_DisplayString(10,10,24,"Illuminati STM32F4");
  LCD_DisplayString(20,40,16,"Author:Clever");
  LCD_DisplayString(30,80,24,"DHT11 TEST");
  LCD_DisplayString(50,110,16,"ATOM@ALIENTEK");

	while(1)
	{	    	    
 		if(t%10==0)//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperatre,&humidity);		//读取温湿度值				
			printf("%d\r\n",temperatre);
			printf("%d\r\n",humidity);
		}				   
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
	}

}

