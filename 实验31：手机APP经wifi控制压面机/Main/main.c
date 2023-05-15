#include "usart4_wifi.h"
#include "led.h"
#include "beep.h"
#include "lcd.h"
#include "key.h"

#include "rtc.h"

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
	u8 t=0;	
	short temperature;
  u8  tempbuf[9]={0}; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();		      //延时初始化 
	
	ESP8266_init();       //先使能ESP8266 就是置CH_PD为高
	GPIOE_I8_10nit();
	
	KEY_Init();
	LED_Init();		  		  //初始化与LED 
	BEEP_Init();          //蜂鸣器初始化
  LCD_Init();           //初始化LCD FSMC接口和显示驱动

	RTC_InitConfig();
	
	BRUSH_COLOR=RED;      //设置画笔颜色为红色
	
	LCD_DisplayString(20,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(20,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"WIFI TEST");
	
	BRUSH_COLOR=BLUE;  
	LCD_DisplayString(30,110,16,"WIFI Server Init...");	
	WIFI_Server_Init();  //配置wifi模块工作在多连接服务器模式  该例程是配置wifi模块为服务器 需要用到客户端或者透传的话请参考配套资料设置
	LCD_DisplayString(30,110,16,"WIFI Server OK!!!  ");
	
	LCD_DisplayString(30,140,16,"APP_mode: 0  ");
	LCD_DisplayString(30,160,16,"Wait APP Control");
	RTC_GetTimes(RTC_Format_BIN);
	Time_Display();
	
                    
	while(1)
	{		
		if(APP_mode==0)
		 {
				if(t%100==0)   //500ms采集一次温度值
				{ 
					RTC_GetTimes(RTC_Format_BIN);
          Time_Display();
					
					LCD_DisplayString(30,190,16,"T:   . C");

 					temperature=434;   //这边为了测试没接DS18B20时的温度值传送，故写了个固体值
					
					if(temperature<0)
					{
						LCD_DisplayChar(30+16,190,'-',16);			//显示负号
						temperature=-temperature;					      //转为正数
						tempbuf[0]='-';
					}
					else 
					{
						LCD_DisplayChar(30+16,190,' ',16);			//去掉负号
						tempbuf[0]=' ';
					}
					LCD_DisplayNum(30+24,190,temperature/10,2,16,0);	//显示正数部分	    
					LCD_DisplayNum(30+48,190,temperature%10,1,16,0);	//显示小数部分
					
					
					tempbuf[1]=temperature/100%10+'0';
					tempbuf[2]=temperature/10%10+'0';
					tempbuf[3]='.';
					tempbuf[4]=temperature%10+'0';
					
					uart4SendChars("AT+CIPSEND=0,5\r\n",16);  //发送数据前需要 先发送 “发送数据的命令”  “5”代表发送的字符串长度
					delay_ms(500);
					uart4SendChars(tempbuf,5);
				}					
		 }
		
		else if(APP_mode==1)
		 {
				key_scan(0);
				
				if(keyup_data==KEY0_DATA)
					{
					 uart4SendChars("AT+CIPSEND=0,14\r\n",17); //发送数据前需要 先发送 “发送数据的命令”
					 delay_ms(500);
					 uart4SendChars("WIFI SEND TEST",14);
					}
				if(keyup_data==KEY1_DATA)
					{
					  uart4SendChars("AT+CIPSEND=0,4\r\n",16);
					  delay_ms(500);
						uart4SendChars("TEST",4);
					}
		 }
		 delay_ms(5);
		 t++;
	}
}

