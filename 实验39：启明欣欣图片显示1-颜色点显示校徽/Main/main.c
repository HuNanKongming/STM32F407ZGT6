#include "led.h"
#include "lcd.h"
#include "picture.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程20 图片显示实验                                                  *
* 文件简述：图片显示                                                             *
* 创建日期：2015.04.15                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：该例程为图片显示方法之一，利用位图(直接颜色点)显示                   *
            还有种方法是从SD卡读出图片数据进行解码显示                           *
* 淘宝店铺：https://shop125046348.taobao.com                                     *					
**********************************************************************************
*********************************************************************************/

extern const unsigned char logo[153608];
 
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();       //初始化延时函数
	LED_Init();					//初始化LED
 	LCD_Init();
  BRUSH_COLOR=RED;    //设置字体为红色 
  LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"20.PICTURE TEST");
	 
  picture_display(0,0,(u8 *)logo);
	while(1)
	{	    	    
    delay_ms(500);
			LED0=!LED0;
  }
}

