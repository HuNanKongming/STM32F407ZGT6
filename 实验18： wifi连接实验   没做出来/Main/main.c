#include "led.h"
#include "lcd.h"
#include "tim.h"
#include "key.h"
#include "stdio.h"
#include "gpiod.h"
#include "usart1.h"
#include "ESP8266.h"

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
  u32 pwmzkb;
//  u16 i=1;
//  u16 num;
	uart1_init(9600);	    //串口初始化波特率为9600
  delay_init();         //初始化延时函数
  LED_Init();					  //初始化LED
  LCD_Init();           //初始化LCD FSMC接口和显示驱动
  KEY_Init();
  BRUSH_COLOR=RED;      //设置画笔颜色为红色
  TIM1_PWM_Init(499,84-1);
  GPIOd_I12_13nit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

	
			  LCD_DisplayString(10,10,24,"Illuminati STM32F4");
				LCD_DisplayString(20,40,16,"Author:Shen jingbo");
				LCD_DisplayString(30,80,24,"4.TFTLCD TEST");
				LCD_DisplayString(50,110,16,"KEY0:Screen Test");
				LCD_DisplayString(50,130,16,"KEY1:Number Test");
				LCD_DisplayString(50,150,16,"KEY2:Time Test");

  while(1)
   {

					

      key_scan(0);
			delay_ms(10);	
      if(keydown_data==KEY0_DATA)
        {					
					LED0=!LED0;
					GPIO_SetBits(GPIOD,GPIO_Pin_12); 
					GPIO_ResetBits(GPIOD,GPIO_Pin_13); 
          pwmzkb=500;
					TIM_SetCompare1(TIM1,pwmzkb); 
        }

      if(keydown_data==KEY1_DATA)
        {
          LED1=!LED1;
					GPIO_SetBits(GPIOD,GPIO_Pin_12); 
					GPIO_ResetBits(GPIOD,GPIO_Pin_13); 
          pwmzkb=250;
          TIM_SetCompare1(TIM1,pwmzkb);	 // 修改占空比

        }

      if(keydown_data==KEY2_DATA)
        {
          LED2=!LED2;   //作指示灯，证明K3已经按下
					GPIO_SetBits(GPIOD,GPIO_Pin_13); 
					GPIO_ResetBits(GPIOD,GPIO_Pin_12); 
          pwmzkb=500;
          TIM_SetCompare1(TIM1,pwmzkb);	 // 修改占空比

        }
      if(keydown_data==KEY3_DATA)
        {

          LED0=!LED0;   //作指示灯，证明K3已经按下
					GPIO_SetBits(GPIOD,GPIO_Pin_13); 
					GPIO_ResetBits(GPIOD,GPIO_Pin_12);
          pwmzkb=250;
					TIM_SetCompare1(TIM1,pwmzkb); 

        }
    } 

}

