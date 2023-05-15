#include "led.h"
#include "lcd.h"
#include "tim.h"
#include "key.h"
#include "stdio.h"
#include "gpiod.h"
#include "usart1.h"
#include "usart3.h"
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
extern uint8_t rebuf3[20];
extern u8 TEMP_data3[20],Receive_ok3;

void send_com(u8 data)
{
	u8 bytes[3]={0};
	bytes[0]=0xa5;
	bytes[1]=data;//�����ֽ�
	//bytes[2]=0xEA;
	USART_Send(bytes,3);//����֡ͷ�������ֽڡ�У���
}



int main(void)
{
  u32 pwmzkb;
//  u16 i=1;
//  u16 num;
	uart1_init(9600);	    //���ڳ�ʼ��������Ϊ9600
	uart3_init(9600);
  delay_init();         //��ʼ����ʱ����
  LED_Init();					  //��ʼ��LED
  LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����
  KEY_Init();
  BRUSH_COLOR=RED;      //���û�����ɫΪ��ɫ
  TIM1_PWM_Init(499,84-1);
  GPIOd_I12_13nit();
	GPIOE_I8_10nit();
	GPIOF_1_0nit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	
		float TO=0,TA=0;
  u8 sum=0,i=0;
	int16_t data[2]={0};
for(int k = 0;k<100;k++)
send_com(0x45);//���Ͷ���λ��ָ��
	water_suction_pump_close();
	rou_mianji_close();

  while(1)
   {
					 
		// LED0 = !LED0;
		// delay_ms(500);
		 
			if(Receive_ok3)//���ڽ������
		{
			for(sum=0,i=0;i<(TEMP_data3[3]+4);i++)//TEMP_data3[3]=4
			sum+=TEMP_data3[i];
			if(sum==TEMP_data3[i])//У����ж�
			{
				TO=(float)((TEMP_data3[4]<<8)|TEMP_data3[5])/100;//�õ���ʵ�¶�
				printf("temperature = %f\r\n",TO);
				data[0]=(TEMP_data3[4]<<8)|TEMP_data3[5];//�Ŵ�100���������¶�
				TA=(float)((TEMP_data3[6]<<8)|TEMP_data3[7])/100;//�õ���ʵ�¶�
				printf("temperature = %f\r\n",TA);
				data[1]=(TEMP_data3[6]<<8)|TEMP_data3[7];//�Ŵ�100���Ļ����¶�
				if(TO  > 20)
				{
					LED0 = 0;
					LED1 = 1;
					LED2 = 1;
				}
				if(TO  > 25)
				{
					LED0 = 0;
					LED1 = 0;
					LED2 = 1;
				}
				if(TO  > 30)
				{
					LED0 = 0;
					LED1 = 0;
					LED2 = 0;
				}
				//send_out(data,2,0x45);//�ϴ�����λ��
				/*				for(int j=0;j < 9;j++)
		{
			printf("%c",TEMP_data3[j]);
		}*/
						
			}
			Receive_ok3=0;//����������ϱ�־
		}	
		
		
		 
		 
		 

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

