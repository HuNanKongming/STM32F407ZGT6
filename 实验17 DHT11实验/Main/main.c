#include "led.h"
#include "lcd.h"
#include "key.h"
#include "stdio.h"
#include "dht11.h"
#include "usart.h" 
/*********************************************************************************
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: ����4 LCD��ʾ������main.c                                            *
* �ļ�������LCD��ʾʵ��                                                          *
* �������ڣ�2017.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����LCD��ʾ�ַ������ֺ�ͼ��                                              *
* �Ա����̣�https://shop125046348.taobao.com                                     *
* ��    ���������̴��������ѧϰ�ο�                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{
  u16 i=1;
  u16 num;
	u8 t = 0;
	u8 temperatre;
	u8 humidity;
  u8 buf[100] = "0123456789";//�ַ����飬���Ա����ַ���
  
	delay_init();         //��ʼ����ʱ����
  LED_Init();					  //��ʼ��LED
  LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����
  KEY_Init();
	uart_init(115200);
  BRUSH_COLOR=RED;      //���û�����ɫΪ��ɫ



  LCD_DisplayString(10,10,24,"Illuminati STM32F4");
  LCD_DisplayString(20,40,16,"Author:Clever");
  LCD_DisplayString(30,80,24,"DHT11 TEST");
  LCD_DisplayString(50,110,16,"ATOM@ALIENTEK");

	while(1)
	{	    	    
 		if(t%10==0)//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperatre,&humidity);		//��ȡ��ʪ��ֵ				
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

