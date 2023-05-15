#include "usart4_wifi.h"
#include "led.h"
#include "beep.h"
#include "lcd.h"
#include "key.h"

#include "rtc.h"

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
	u8 t=0;	
	short temperature;
  u8  tempbuf[9]={0}; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();		      //��ʱ��ʼ�� 
	
	ESP8266_init();       //��ʹ��ESP8266 ������CH_PDΪ��
	GPIOE_I8_10nit();
	
	KEY_Init();
	LED_Init();		  		  //��ʼ����LED 
	BEEP_Init();          //��������ʼ��
  LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����

	RTC_InitConfig();
	
	BRUSH_COLOR=RED;      //���û�����ɫΪ��ɫ
	
	LCD_DisplayString(20,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(20,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"WIFI TEST");
	
	BRUSH_COLOR=BLUE;  
	LCD_DisplayString(30,110,16,"WIFI Server Init...");	
	WIFI_Server_Init();  //����wifiģ�鹤���ڶ����ӷ�����ģʽ  ������������wifiģ��Ϊ������ ��Ҫ�õ��ͻ��˻���͸���Ļ���ο�������������
	LCD_DisplayString(30,110,16,"WIFI Server OK!!!  ");
	
	LCD_DisplayString(30,140,16,"APP_mode: 0  ");
	LCD_DisplayString(30,160,16,"Wait APP Control");
	RTC_GetTimes(RTC_Format_BIN);
	Time_Display();
	
                    
	while(1)
	{		
		if(APP_mode==0)
		 {
				if(t%100==0)   //500ms�ɼ�һ���¶�ֵ
				{ 
					RTC_GetTimes(RTC_Format_BIN);
          Time_Display();
					
					LCD_DisplayString(30,190,16,"T:   . C");

 					temperature=434;   //���Ϊ�˲���û��DS18B20ʱ���¶�ֵ���ͣ���д�˸�����ֵ
					
					if(temperature<0)
					{
						LCD_DisplayChar(30+16,190,'-',16);			//��ʾ����
						temperature=-temperature;					      //תΪ����
						tempbuf[0]='-';
					}
					else 
					{
						LCD_DisplayChar(30+16,190,' ',16);			//ȥ������
						tempbuf[0]=' ';
					}
					LCD_DisplayNum(30+24,190,temperature/10,2,16,0);	//��ʾ��������	    
					LCD_DisplayNum(30+48,190,temperature%10,1,16,0);	//��ʾС������
					
					
					tempbuf[1]=temperature/100%10+'0';
					tempbuf[2]=temperature/10%10+'0';
					tempbuf[3]='.';
					tempbuf[4]=temperature%10+'0';
					
					uart4SendChars("AT+CIPSEND=0,5\r\n",16);  //��������ǰ��Ҫ �ȷ��� ���������ݵ����  ��5�������͵��ַ�������
					delay_ms(500);
					uart4SendChars(tempbuf,5);
				}					
		 }
		
		else if(APP_mode==1)
		 {
				key_scan(0);
				
				if(keyup_data==KEY0_DATA)
					{
					 uart4SendChars("AT+CIPSEND=0,14\r\n",17); //��������ǰ��Ҫ �ȷ��� ���������ݵ����
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

