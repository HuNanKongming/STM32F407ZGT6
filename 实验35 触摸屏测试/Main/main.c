 #include "usart1.h"   
#include "led.h"
#include "lcd.h"
#include "key.h" 
#include "touch.h"
#include "xpt2046.h" 
#include "ft5426.h"


/*********************************************************************************
************************启明欣欣 STM32F407核心应用板******************************
**********************************************************************************
* 文件名称: 例程19 触摸屏main.c                                                  *
* 文件简述：触摸屏实现一个画图板                                                 *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：触摸屏实现一个画图板                                                 *    
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/	





int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();  //初始化延时函数
	uart1_init(9600);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化 
	KEY_Init(); 				//按键初始化  
	Touch_Init();				//触摸屏初始化
 	BRUSH_COLOR=RED;    //设置字体为红色 
//	LCD_DisplayString(10,10,16,"Illuminati STM32");	
//  LCD_DisplayString(20,40,24,"Author:Clever");
//	LCD_DisplayString(30,80,24,"19.TOUCH TEST");

//	delay_ms(1000);
	
 	Clear_Screen();	       //先清屏 	
  if(lcd_id==0x9341)
	   R_Touch_test();     //电阻屏画板测试测试
	else if(lcd_id==0x1963)
	   C_Touch_test(); 		 //电容屏画板测试测试			  
}
