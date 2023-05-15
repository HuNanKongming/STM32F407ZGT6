#include "ESP8266.h"
#include "led.h"
#include "beep.h"
#include "lcd.h"
#include "key.h"
#include "gpiod.h"
#include "rtc.h"
#include "tim.h"
#include "usart1.h"
/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: WIFI模块使用                                                         *
* 文件简述：                                                                     *
* 创建日期：2015.03.06                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：手机首先wifi搜索连接该WiFi模块配置成的路由器，然后打开手机APP输入服  *
            务器(wifi模块)IP跟端口号就可以控制开发板                             * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

// 由于wifi与DP83848网口公用，所以在使用wifi时把DP83848旁的一排短路帽拆掉

/* 手机首先wifi搜索连接该WiFi模块配置成的路由器，然后打开手机APP输入服  *
   务器(wifi模块)IP跟端口号就可以控制开发板  wifi模块的IP一般固定为192.168.4.1   */ 

u8 const *weekdate[7]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

void Time_Display()
{
 LCD_DisplayString(30,220,16,"Date:20  -  -  ");
 LCD_DisplayNum(86,220,RTC_DateStruct.RTC_Year,2,16,1);
 LCD_DisplayNum(110,220,RTC_DateStruct.RTC_Month,2,16,1);
 LCD_DisplayNum(134,220,RTC_DateStruct.RTC_Date,2,16,1);
 LCD_DisplayString(30,240,16,"Time:  :  :  ");
 LCD_DisplayNum(70,240,RTC_TimeStruct.RTC_Hours,2,16,1);	
 LCD_DisplayNum(94,240,RTC_TimeStruct.RTC_Minutes,2,16,1);
 LCD_DisplayNum(118,240,RTC_TimeStruct.RTC_Seconds,2,16,1);
 LCD_DisplayString(30,260,16,"Week:        ");
 LCD_DisplayString(70,260,16,(u8 *)weekdate[RTC_DateStruct.RTC_WeekDay-1]);
}

int main(void)
{   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();		      //延时初始化 
	uart1_init(115200);
	//WIFI_LinkServer();       //先使能ESP8266 就是置CH_PD为高
	GPIOE_I8_10nit(); //揉面机GPIO初始化
	GPIOE_I14_11Init();		//抽水泵GPIO初始化
	GPIOd_I12_13nit();
	TIM1_PWM_Init(499,84-1);
	KEY_Init();
	LED_Init();		  		  //初始化与LED 
	BEEP_Init();          //蜂鸣器初始化
	printf("WIFI初始化开始标志\r\n");
	ESP8266_Init();
	printf("WIFI初始化结束标志\r\n");
	while(1)
	{		
						LED0=0;
	}
}

