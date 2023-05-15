#include "ESP8266.h"
#include "led.h"
char name[] = "HUAWEI";//ElectronicLab
char password[] = "11112222";//ElectronicLab666
int port = 5000;

char host[] = "10.50.23.67";//192.168.137.1  10.100.128.8

uint8_t wifi_count = 0;
uint8_t esp8266_step = 0;//���ò�����
char wifi_rxbuf[Wifi_BufSize];

//extern QueueHandle_t WifiRecieveData_Queue;

void Usart2_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��GPIOA.2
  
	//USART2_RX	  GPIOA.3��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			  //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //��ʼ��GPIOA.3
	
	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
  
	USART_Init(USART2, &USART_InitStructure);	   //��ʼ������1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);					   //ʹ�ܴ���1
}

//����1�жϷ������
void USART2_IRQHandler(void) 
{
	uint8_t rx_temp;
  
	if (USART_GetITStatus(WifiUsart, USART_IT_RXNE) != RESET)
	{
		LED1 = 0;
	}
}

//wifi����
void ESP8266_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
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
 * @description: ����wifi����
 * @param: command�������ַ��� timeout����ʱʱ��
 * @return: 0���������ʧ��  1��������ճɹ�
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
 * @description: wifi��λ��ʹĳЩ������Ч������Ҫ��������
 * @param: timeout �ȴ���ʱʱ��
 * @return: 0-��λʧ�� 1-��λ�ɹ�
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

//����1����һ���ַ�
void Wifi_SendChar(u8 ch)
{
	while ((WifiUsart->SR & 0x40) == 0)
		;
	WifiUsart->DR = (u8)ch;
}

/****************************************************************************
* ��    ��: void uart1SendChars(u8 *str)
* ��    �ܣ�����2����һ�ַ���
* ��ڲ�����*str�����͵��ַ���
* ���ز�������
* ˵    ���� 
****************************************************************************/
void Wifi_SendData(char *str)
{
	uint16_t k = 0;
	uint16_t len = strlen(str);
	do
	{
		Wifi_SendChar(*(str + k));
		k++;
	} //ѭ������,ֱ���������
	while (k < len);
	Wifi_SendChar('\r');
	Wifi_SendChar('\n');
}

