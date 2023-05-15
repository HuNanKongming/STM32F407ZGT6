#include "led.h"
#include "lcd.h"
#include "tim.h"
#include "key.h"
#include "stdio.h"
#include "gpiod.h"
#include "usart1.h"
#include "stdio.h"
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
extern uint8_t rebuf[20];


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
	float TO=0,TA=0;
  u8 sum=0,i=0;
	int16_t data[2]={0};
	
	
	
//  u32 pwmzkb;
//  u16 i=1;
//  u16 num;
	uart1_init(9600);	    //���ڳ�ʼ��������Ϊ9600
  delay_init();         //��ʼ����ʱ����
  LED_Init();					  //��ʼ��LED
  KEY_Init();
  BRUSH_COLOR=RED;      //���û�����ɫΪ��ɫ
  TIM1_PWM_Init(499,84-1);
  GPIOd_I12_13nit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	
	//send_com();//���Ͷ���λ��ָ��
	//send_com();
	
	
	
for(int k = 0;k<100;k++)
send_com(0x45);//���Ͷ���λ��ָ��


  while(1)
   {
				
		 LED0 = !LED0;
		 delay_ms(500);
		 
			if(Receive_ok)//���ڽ������
		{
			for(sum=0,i=0;i<(TEMP_data[3]+4);i++)//TEMP_data[3]=4
			sum+=TEMP_data[i];
			if(sum==TEMP_data[i])//У����ж�
			{
				TO=(float)((TEMP_data[4]<<8)|TEMP_data[5])/100;//�õ���ʵ�¶�
				printf("%f\r\n",TO);
				data[0]=(TEMP_data[4]<<8)|TEMP_data[5];//�Ŵ�100���������¶�
				TA=(float)((TEMP_data[6]<<8)|TEMP_data[7])/100;//�õ���ʵ�¶�
				data[1]=(TEMP_data[6]<<8)|TEMP_data[7];//�Ŵ�100���Ļ����¶�
				if(TO > 25)
				{
					LED1 = 0;
					LED2 = 1;
				}
				if(TO > 30)
				{
					LED1 = 0;
					LED2 = 0;
				}
				//send_out(data,2,0x45);//�ϴ�����λ��
				/*				for(int j=0;j < 9;j++)
		{
			printf("%c",TEMP_data[j]);
		}*/
						
			}
			Receive_ok=0;//����������ϱ�־
		}		

	 }


	} 


