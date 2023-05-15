#include "led.h"
#include "lcd.h"
#include "tim.h"
#include "key.h"
#include "stdio.h"
#include "gpiod.h"
#include "usart1.h"
#include "string.h"

extern u8 TEMP_data[20],Receive_ok;
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
void send_com(u8 data)
{
	u8 bytes[3]={0};
	bytes[0]=0xa5;
	bytes[1]=data;//功能字节
	//bytes[2]=0xEA;
	USART_Send(bytes,3);//发送帧头、功能字节、校验和
}

void send_out(int16_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[30],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=2*length;//数据个数
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k]>>8;
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,2*length+5);	
}

int main(void)
{
	float TO=0,TA=0;
  u8 sum=0,i=0;
	int16_t data[2]={0};
	
	
  u32 pwmzkb;
//  u16 i=1;
//  u16 num;
	uart1_init(115200);	    //串口初始化波特率为9600
  delay_init();         //初始化延时函数
  LED_Init();					  //初始化LED
  LCD_Init();           //初始化LCD FSMC接口和显示驱动
  KEY_Init();
  BRUSH_COLOR=RED;      //设置画笔颜色为红色
  TIM1_PWM_Init(499,84-1);
  GPIOd_I12_13nit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_ms(1000);
	
	
for(int k = 0;k<100;k++)
send_com(0x45);//发送读方位角指令


  while(1)
   {
				
		 LED0 = !LED0;
		 delay_ms(500);
		 
			if(Receive_ok)//串口接收完毕
		{
			for(sum=0,i=0;i<(TEMP_data[3]+4);i++)//TEMP_data[3]=4
			sum+=TEMP_data[i];
			if(sum==TEMP_data[i])//校验和判断
			{
				TO=(float)((TEMP_data[4]<<8)|TEMP_data[5])/100;//得到真实温度
				printf("%f\r\n",TO);
				data[0]=(TEMP_data[4]<<8)|TEMP_data[5];//放大100倍的物体温度
				TA=(float)((TEMP_data[6]<<8)|TEMP_data[7])/100;//得到真实温度
				data[1]=(TEMP_data[6]<<8)|TEMP_data[7];//放大100倍的环境温度
				//send_out(data,2,0x45);//上传给上位机
				/*				for(int j=0;j < 9;j++)
		{
			printf("%c",TEMP_data[j]);
		}*/
						
			}
			Receive_ok=0;//处理数据完毕标志
		}		

	 }

}

