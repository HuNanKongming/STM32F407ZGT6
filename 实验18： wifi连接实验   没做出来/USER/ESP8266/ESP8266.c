#include "ESP8266.h"
#include "led.h"
char name[] = "HUAWEI";//ElectronicLab
char password[] = "11112222";//ElectronicLab666
int port = 5000;

char host[] = "10.50.23.67";//192.168.137.1  10.100.128.8

uint8_t wifi_count = 0;
uint8_t esp8266_step = 0;//配置步骤编号
char wifi_rxbuf[Wifi_BufSize];

//extern QueueHandle_t WifiRecieveData_Queue;

void Usart2_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化GPIOA.2
  
	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			  //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //初始化GPIOA.3
	
	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化NVIC寄存器
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
  
	USART_Init(USART2, &USART_InitStructure);	   //初始化串口1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启串口接受中断
	USART_Cmd(USART2, ENABLE);					   //使能串口1
}

//串口1中断服务程序
void USART2_IRQHandler(void) 
{
	uint8_t rx_temp;
  
	if (USART_GetITStatus(WifiUsart, USART_IT_RXNE) != RESET)
	{
		LED1 = 0;
	}
}

//wifi连接
void ESP8266_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void WIFI_Server_Init(void)
{
	char client[100];
	char server[100];
	
	sprintf(client, "AT+CWJAP=\"%s\",\"%s\"", name, password);
	sprintf(server, "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);
	
	ESP8266_Init();
	Usart2_Init(115200);
	for(;;)
	{
		if(esp8266_step == 0)
		{
			printf("wifi reset...\r\n");
			if(Wifi_Reset(5000))
			{
				printf("wifi reset successful!\r\n");
				esp8266_step++;
			}
		}
		else if(esp8266_step == 1)
		{
			printf("step1-> config as station mode!\r\n");
			if(Wifi_SendCmd("AT+CWMODE=1", 4000))
			{
				esp8266_step++;
				printf("configuration successful!\r\n");
			}
		}
		else if(esp8266_step == 2)
		{
			printf("step2-> restart!\r\n");
			if(Wifi_SendCmd("AT+RST", 5000))
			{
				esp8266_step++;
				printf("configuration successful!\r\n");
				delay_ms(1000);
			}
		}
		else if(esp8266_step == 3)
		{
			printf("step3-> connect To=>\r\nrouter: %s:%s\r\npassword:%s\r\n", name, host, password);
			if(Wifi_SendCmd(client, 8000))
			{
				esp8266_step++;
				printf("connection successful!\r\n");
			}
		}
		else if(esp8266_step == 4)
		{
			printf("step4-> start single route mode!\r\n");
			if(Wifi_SendCmd("AT+CIPMUX=0", 2000))
			{
				esp8266_step++;
				printf("start successful!\r\n");
			}
		}
		else if(esp8266_step == 5)
		{
			printf("step5-> connect server!\r\n");
			if(Wifi_SendCmd(server, 6000))
			{
				esp8266_step++;
				printf("connection successful!\r\n");
			}
		}
		else if(esp8266_step == 6)
		{
			printf("step6-> config as transparent transmission!\r\n");
			if(Wifi_SendCmd("AT+CIPMODE=1", 4000))
			{
				esp8266_step++;
				printf("configuration successful!\r\n");
			}
		}
		else if(esp8266_step == 7)
		{
			printf("step7-> start transparent transmission...\r\n");
			if(Wifi_SendCmd("AT+CIPSEND", 4000))
			{
				esp8266_step++;
				printf("start transmission!\r\n");
				break;
			}
		}
	}
}

/**
 * @author: TanXY
 * @description: 发送wifi命令
 * @param: command：命令字符串 timeout：超时时间
 * @return: 0：命令接收失败  1：命令接收成功
 */
uint8_t Wifi_SendCmd(char *command, uint16_t timeout)
{
	wifi_count = 0;
	memset(wifi_rxbuf, 0, Wifi_BufSize);
	Wifi_SendData(command);
	
	while(timeout)
	{
		delay_ms(50);
		if(strstr((char *)wifi_rxbuf, "OK"))
			return 1;
		if(timeout % 1000 == 0)
			printf("timeout->%d\r\n", timeout/1000);
		timeout -= 50;
	}
	printf("timeout, try again!\r\n");
	
	return 0;
}

/**
 * @author: TanXY
 * @description: wifi复位，使某些配置生效，不需要掉电重启
 * @param: timeout 等待超时时间
 * @return: 0-复位失败 1-复位成功
 */
uint8_t Wifi_Reset(uint16_t timeout)
{
	WIFI_RESET = 0;
	delay_xms(500);
	WIFI_RESET = 1;
	while(timeout)
	{
		delay_xms(50);
		if(strstr((char *)wifi_rxbuf,"ready"))
			return 1;
		timeout -= 50;
		if(timeout % 1000 == 0)
			printf("timeout->%d\r\n",timeout / 1000);
	}
	
	printf("wait timeout, try again!\r\n");
	return 0;
}

//串口1发送一个字符
void Wifi_SendChar(u8 ch)
{
	while ((WifiUsart->SR & 0x40) == 0)
		;
	WifiUsart->DR = (u8)ch;
}

/****************************************************************************
* 名    称: void uart1SendChars(u8 *str)
* 功    能：串口2发送一字符串
* 入口参数：*str：发送的字符串
* 返回参数：无
* 说    明： 
****************************************************************************/
void Wifi_SendData(char *str)
{
	uint16_t k = 0;
	uint16_t len = strlen(str);
	do
	{
		Wifi_SendChar(*(str + k));
		k++;
	} //循环发送,直到发送完毕
	while (k < len);
	Wifi_SendChar('\r');
	Wifi_SendChar('\n');
}

