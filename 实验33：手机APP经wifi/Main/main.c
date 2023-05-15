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
**********************MCU���� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: WIFIģ��ʹ��                                                         *
* �ļ�������                                                                     *
* �������ڣ�2015.03.06                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    �����ֻ�����wifi�������Ӹ�WiFiģ�����óɵ�·������Ȼ����ֻ�APP�����  *
            ����(wifiģ��)IP���˿ںžͿ��Կ��ƿ�����                             * 
* �Ա����̣�https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

// ����wifi��DP83848���ڹ��ã�������ʹ��wifiʱ��DP83848�Ե�һ�Ŷ�·ñ���

/* �ֻ�����wifi�������Ӹ�WiFiģ�����óɵ�·������Ȼ����ֻ�APP�����  *
   ����(wifiģ��)IP���˿ںžͿ��Կ��ƿ�����  wifiģ���IPһ��̶�Ϊ192.168.4.1   */ 

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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();		      //��ʱ��ʼ�� 
	uart1_init(115200);
	//WIFI_LinkServer();       //��ʹ��ESP8266 ������CH_PDΪ��
	GPIOE_I8_10nit(); //�����GPIO��ʼ��
	GPIOE_I14_11Init();		//��ˮ��GPIO��ʼ��
	GPIOd_I12_13nit();
	TIM1_PWM_Init(499,84-1);
	KEY_Init();
	LED_Init();		  		  //��ʼ����LED 
	BEEP_Init();          //��������ʼ��
	printf("WIFI��ʼ����ʼ��־\r\n");
	ESP8266_Init();
	printf("WIFI��ʼ��������־\r\n");
	while(1)
	{		
						LED0=0;
	}
}

