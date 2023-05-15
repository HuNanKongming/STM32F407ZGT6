#include "led.h"
#include "lcd.h"
#include "tim.h"
#include "key.h"
#include "stdio.h"
#include "gpiod.h"
#include "usart1.h"
#include "ESP8266.h"

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
  u32 pwmzkb;
//  u16 i=1;
//  u16 num;
	uart1_init(9600);	    //���ڳ�ʼ��������Ϊ9600
  delay_init();         //��ʼ����ʱ����
  LED_Init();					  //��ʼ��LED
  LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����
  KEY_Init();
  BRUSH_COLOR=RED;      //���û�����ɫΪ��ɫ
  TIM1_PWM_Init(499,84-1);
  GPIOd_I12_13nit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	
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
          TIM_SetCompare1(TIM1,pwmzkb);	 // �޸�ռ�ձ�

        }

      if(keydown_data==KEY2_DATA)
        {
          LED2=!LED2;   //��ָʾ�ƣ�֤��K3�Ѿ�����
					GPIO_SetBits(GPIOD,GPIO_Pin_13); 
					GPIO_ResetBits(GPIOD,GPIO_Pin_12); 
          pwmzkb=500;
          TIM_SetCompare1(TIM1,pwmzkb);	 // �޸�ռ�ձ�

        }
      if(keydown_data==KEY3_DATA)
        {

          LED0=!LED0;   //��ָʾ�ƣ�֤��K3�Ѿ�����
					GPIO_SetBits(GPIOD,GPIO_Pin_13); 
					GPIO_ResetBits(GPIOD,GPIO_Pin_12);
          pwmzkb=250;
					TIM_SetCompare1(TIM1,pwmzkb); 

        }
    } 

}

