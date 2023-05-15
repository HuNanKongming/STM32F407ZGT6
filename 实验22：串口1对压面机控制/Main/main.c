#include "led.h"
#include "lcd.h"
#include "tim.h"
#include "key.h"
#include "stdio.h"
#include "gpiod.h"
#include "usart1.h"
#include "usart3.h"
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
extern uint8_t rebuf3[20];
extern u8 TEMP_data3[20],Receive_ok3;

void send_com(u8 data)
{
	u8 bytes[3]={0};
	bytes[0]=0xa5;
	bytes[1]=data;//功能字节
	//bytes[2]=0xEA;
	USART_Send(bytes,3);//发送帧头、功能字节、校验和
}



int main(void)
{
  u32 pwmzkb;
//  u16 i=1;
//  u16 num;
	uart1_init(9600);	    //串口初始化波特率为9600
	uart3_init(9600);
  delay_init();         //初始化延时函数
  LED_Init();					  //初始化LED
  LCD_Init();           //初始化LCD FSMC接口和显示驱动
  KEY_Init();
  BRUSH_COLOR=RED;      //设置画笔颜色为红色
  TIM1_PWM_Init(499,84-1);
  GPIOd_I12_13nit();
	GPIOE_I8_10nit();
	GPIOF_1_0nit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

	
		float TO=0,TA=0;
  u8 sum=0,i=0;
	int16_t data[2]={0};
for(int k = 0;k<100;k++)
send_com(0x45);//发送读方位角指令
	water_suction_pump_close();
	rou_mianji_close();

  while(1)
   {
					 
		// LED0 = !LED0;
		// delay_ms(500);
		 
			if(Receive_ok3)//串口接收完毕
		{
			for(sum=0,i=0;i<(TEMP_data3[3]+4);i++)//TEMP_data3[3]=4
			sum+=TEMP_data3[i];
			if(sum==TEMP_data3[i])//校验和判断
			{
				TO=(float)((TEMP_data3[4]<<8)|TEMP_data3[5])/100;//得到真实温度
				printf("temperature = %f\r\n",TO);
				data[0]=(TEMP_data3[4]<<8)|TEMP_data3[5];//放大100倍的物体温度
				TA=(float)((TEMP_data3[6]<<8)|TEMP_data3[7])/100;//得到真实温度
				printf("temperature = %f\r\n",TA);
				data[1]=(TEMP_data3[6]<<8)|TEMP_data3[7];//放大100倍的环境温度
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
				//send_out(data,2,0x45);//上传给上位机
				/*				for(int j=0;j < 9;j++)
		{
			printf("%c",TEMP_data3[j]);
		}*/
						
			}
			Receive_ok3=0;//处理数据完毕标志
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

