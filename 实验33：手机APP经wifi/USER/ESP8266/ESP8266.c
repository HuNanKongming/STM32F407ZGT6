#include "ESP8266.h"
#include "usart1.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "usart1.h"
char ssid[] = "JINGBO";  //���ԣ�JINGBO ���룺11112222
char pwd[] = "11112222";	//�ֻ� HUAWEI ���룺11112222
int port = 8080;
char host[] = "192.168.137.1"; //10.50.23.212 	127.0.0.1


short wifi_count = 0;
short esp8266_step = 0; //���ò���
char wifi_rxbuf[WIFI_BUF_SIZE];

u8 receive_str[UART4_REC_NUM];     //���ջ�������,���USART_REC_LEN���ֽ� 
u8 uart_byte_count=0;
u8 APP_mode=0;          //APP����ģʽ  0�����������  1�����շ�����


void Uart4_Init(u32 bound);
uint8_t Wifi_Reset(uint16_t timeout);
//����4�жϷ������
void UART4_IRQHandler(void)
{
	u8 rec_data;
	    uint8_t rx_temp;
    
    if (USART_GetITStatus(WifiUsart, USART_IT_RXNE) != RESET)
    {
        rx_temp = (uint8_t)USART_ReceiveData(WifiUsart);

        if (esp8266_step <= LINK_STEP)
        {
            if (rx_temp != '\0')
            {
                wifi_rxbuf[wifi_count++] = rx_temp;
            }
            else
            {
//                printf("%s", wifi_rxbuf);
                wifi_count = 0;
                memset(wifi_rxbuf, 0, sizeof(wifi_rxbuf));
            }
        }
        else
        {
            if (rx_temp == '{')
            {
                wifi_count = 1;
                memset(wifi_rxbuf, 0, sizeof(wifi_rxbuf));
            }
            else if (rx_temp == '}')
            {
                wifi_count = -1;

//                if(esp8266_step >= LINK_STEP && xQueueSendFromISR(wifiRec_Queue, &wifi_rxbuf, NULL) == errQUEUE_FULL)
//                    printf("wifiRec_Queue is full!");
            }
            else if (wifi_count > 0 && wifi_count < WIFI_BUF_SIZE)
            {
                wifi_rxbuf[wifi_count - 1] = rx_temp;
                wifi_count++;
            }
        }
    }


	//Wifi_SendData("�жϲ�����");
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж� 
		{
				rec_data =(u8)USART_ReceiveData(UART4);         //(USART1->DR) ��ȡ���յ�������

        if(rec_data=='S')		  	                         //�����S����ʾ��������Ϣ����ʼλ
				{
					uart_byte_count=0x01; 
				}	
				
			else if(rec_data=='E')		                         //���E����ʾ��������Ϣ���͵Ľ���λ
				{
					if(strcmp("open_led1",(char *)receive_str)==0)         {LED0=0;printf("helloworld");}	   //����LED0
					else if(strcmp("close_led1",(char *)receive_str)==0)   LED0=1;	   //����LED0
					
					else if(strcmp("open_led2",(char *)receive_str)==0)    LED1=0;	   //����LED1
					else if(strcmp("close_led2",(char *)receive_str)==0)   LED1=1;	   //����LED1
					
					else if(strcmp("open_led3",(char *)receive_str)==0)    LED2=0;	   //����LED2
					else if(strcmp("close_led3",(char *)receive_str)==0)   LED2=1;	   //����LED2
									
						//���������
					 else if(strcmp("kneadON",(char *)receive_str)==0)
					{
							//����������п���
						LED1 = 0;
						rou_mianji_open();
					}
					 else if(strcmp("kneadOFF",(char *)receive_str)==0)
					{
							//����������п���
						LED1 = 1;
						rou_mianji_close();
					}
					
					//��ˮ�ÿ���
					 else if(strcmp("pumpON",(char *)receive_str)==0)
					{
								water_suction_pump_open();   //�򿪳�ˮ��
								LED2 = 0;
					}
					 else if(strcmp("pumpOFF",(char *)receive_str)==0)
					{
							water_suction_pump_close();  //�رճ�ˮ��
							LED2 = 1;
					}
						
					for(uart_byte_count=0;uart_byte_count<32;uart_byte_count++)receive_str[uart_byte_count]=0x00;
					uart_byte_count=0;    
				}				  
			else if((uart_byte_count>0)&&(uart_byte_count<=UART4_REC_NUM))
				{
				   receive_str[uart_byte_count-1]=rec_data;
				   uart_byte_count++;
				}                		 
   } 
}

//wifi��λ���ų�ʼ��
void ESP8266_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//��ʼ��GPIO

    GPIO_SetBits(GPIOF, GPIO_Pin_14);
}

void ESP8266_Init(void)
{
    ESP8266_GPIO_Init();
    Uart4_Init(115200);
	
    Wifi_Reset(5000);

    if (WaitConnection(8000)) //��ȴ�8S����û�л�ȡ��IP��ַ�ͽ���һ������
    {
        esp8266_step = 3;
    }
			
#if WIFI_DEBUG
    else
        printf("���ӳ�ʱ......\r\n");
#endif
	
    CommonConnection();
}

/**
 * @author: TanXY
 * @description: ������������������״�����
 */
void CommonConnection(void)
{
    static char try_count = 0;
    char router[64];
    char server[64];

    sprintf(router, "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
    sprintf(server, "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);
    
    for (;;)
    {
        if (esp8266_step == 0)
        {
#if WIFI_DEBUG
            printf("step0-> ѡ��ģʽ��ʼ\r\n");
#endif
            if (Wifi_SendCmd("AT+CWMODE=1", 5000))
            {
#if WIFI_DEBUG
                printf("step0-> ѡ��ģʽ���\r\n");
#endif
                esp8266_step++;
            }
        }
        else if (esp8266_step == 1)
        {
#if WIFI_DEBUG
            printf("step1-> ��ʼ�رն�����\r\n");
#endif
            if (Wifi_SendCmd("AT+CIPMUX=0", 3000))
            {
#if WIFI_DEBUG
                printf("step1-> �رն��������\r\n");
#endif
                esp8266_step++;
            }
        }
        else if (esp8266_step == 2)
        {
            try_count++;
#if WIFI_DEBUG
            printf("step3-> ����WIFI�˺�=>\r\nSSID: %s WIFI����:%s\r\n", ssid, pwd);
#endif
            if (Wifi_SendCmd(router, 8000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("WIFI���ӳɹ�!\r\n");
#endif
            }
            if (try_count >= 4)
            {
#if WIFI_DEBUG
                printf("WIFI����ʧ��\r\n");
#endif
                try_count = 0;
                __set_FAULTMASK(1);
                NVIC_SystemReset();
            }
        }
        else if (esp8266_step == 3)
        {
            try_count++;
#if WIFI_DEBUG
            printf("step6-> ���ӷ�����=>\r\nIP:%s:%d", host, port);
#endif
            if (Wifi_SendCmd(server, 6000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("���������ӳɹ�!\r\n");
#endif
                try_count = 0;
            }
            if (try_count > 3)
            {
#if WIFI_DEBUG
                printf("����������ʧ��\r\n");
#endif
                try_count = 0;
//                Wifi_SendCmd("AT+RESTORE", 5000);  //��λ;
//                __set_FAULTMASK(1);
//	                NVIC_SystemReset();
            }
        }
        else if (esp8266_step == 4)
        {
#if WIFI_DEBUG
            printf("step7-> ����͸��ģʽ!\r\n");
#endif
            if (Wifi_SendCmd("AT+CIPMODE=1", 4000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("����͸�ӳɹ�\r\n");
#endif
            }
        }
        else if (esp8266_step == 5)
        {
#if WIFI_DEBUG
            printf("step8-> ��ʼ͸�ӳ�ʼ��\r\n");
#endif
            if (Wifi_SendCmd("AT+CIPSEND", 4000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("͸�ӳ�ʼ���ɹ�\r\n");
#endif
                break;
            }
        }
    }
}

/**
 * @author: TanXY
 * @description: �ȴ�ģ��������wifi
 * @param: command�������ַ��� timeout����ʱʱ��
 * @return: 0���������ʧ��  1��������ճɹ�
 */
uint8_t WaitConnection(uint16_t timeout)
{
    while (timeout)
    {
        delay_ms(50);

        if (strstr((char *)wifi_rxbuf, "WIFI GOT IP"))
        {
#if WIFI_DEBUG
            printf("Connect OK\r\n");
#endif
            wifi_count = 0;
            memset((char *)wifi_rxbuf, 00, WIFI_BUF_SIZE);
            return 1;
        }
#if WIFI_DEBUG
        if (timeout % 1000 == 0)
            printf("timeout->%d\r\n", timeout / 1000);
#endif
        timeout -= 50;
    }

    wifi_count = 0;
    memset((char *)wifi_rxbuf, 0, WIFI_BUF_SIZE);
    return 0;
}

/**
 * @author: TanXY
 * @description: ����wifi����
 * @param: command�������ַ��� timeout����ʱʱ�� ��λ��
 * @return: 0���������ʧ��  1��������ճɹ�
 */
uint8_t Wifi_SendCmd(char *command, uint16_t timeout)
{
    wifi_count = 0;
    memset(wifi_rxbuf, 0, WIFI_BUF_SIZE);
    Wifi_SendData(command);

    while (timeout)
    {
        delay_ms(100);

        if (strstr((char *)wifi_rxbuf, "OK"))
        {
            wifi_count = 0;
            memset((char *)wifi_rxbuf, 00, WIFI_BUF_SIZE);
            return 1;
        }
#if WIFI_DEBUG
        if (timeout % 1000 == 0)
            printf("timeout->%d\r\n", timeout / 1000);
#endif
        timeout -= 100;
    }
#if WIFI_DEBUG
    printf("timeout, try again!\r\n");
#endif
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
    delay_ms(500);
    WIFI_RESET = 1;

    while (timeout)
    {
        delay_ms(100);

        if (strstr((char *)wifi_rxbuf, "ready"))
        {
            delay_ms(100);
#if WIFI_DEBUG
            printf("Reset OK\r\n");
#endif
            wifi_count = 0;
            memset((char *)wifi_rxbuf, 00, WIFI_BUF_SIZE);
            return 1;
        }

        timeout -= 100;
#if WIFI_DEBUG
        if (timeout % 1000 == 0)
            printf("timeout->%d\r\n", timeout / 1000);
#endif
    }

    wifi_count = 0;
    memset((char *)wifi_rxbuf, 00, WIFI_BUF_SIZE);
#if WIFI_DEBUG
    printf("reset timeout, try again!\r\n");
#endif
    return 0;
}

//����1����һ���ַ�
void Wifi_SendChar(u8 ch)
{
    while ((WifiUsart->SR & 0x40) == 0)
        ;

    WifiUsart->DR = (u8)ch;
}

/**
 * @author: TanXY
 * @description: ͨ��wifi��������
 * @param: str �����͵�����
 */
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

void Uart4_Init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); //ʹ��USART4��GPIOAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);						//GPIOA0����ΪUSART4
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);						//GPIOA1����ΪUSART4
    
    //UART4_TX   GPIOA.1  UART4_RX   GPIOA.0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
    
    //USART4 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //��ռ���ȼ�4
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                           //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
    
    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;                                     //���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure);      //��ʼ������4
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
    USART_Cmd(UART4, ENABLE);                     //ʹ�ܴ���4
}



