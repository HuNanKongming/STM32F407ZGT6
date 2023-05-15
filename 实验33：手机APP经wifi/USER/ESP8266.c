#include "ESP8266.h"

char ssid[] = "TanXY";
char pwd[] = "12345678";

int port = 5000;

char host[] = "192.168.137.1";

short wifi_count = 0;
short esp8266_step = 0; //配置步骤
char wifi_rxbuf[WIFI_BUF_SIZE];

void Uart4_Init(u32 bound);

//串口3中断服务程序
void UART4_IRQHandler(void)
{
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

                if(esp8266_step >= LINK_STEP && xQueueSendFromISR(wifiRec_Queue, &wifi_rxbuf, NULL) == errQUEUE_FULL)
                    printf("wifiRec_Queue is full!");
            }
            else if (wifi_count > 0 && wifi_count < WIFI_BUF_SIZE)
            {
                wifi_rxbuf[wifi_count - 1] = rx_temp;
                wifi_count++;
            }
        }
    }
}

//wifi复位引脚初始化
void ESP8266_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//初始化GPIO

    GPIO_SetBits(GPIOF, GPIO_Pin_14);
}

void ESP8266_Init(void)
{
    ESP8266_GPIO_Init();
    Uart4_Init(115200);

    Wifi_Reset(5000);

    if (WaitConnection(8000)) //最长等待8S，还没有获取到IP地址就进入一般配置
    {
        esp8266_step = 3;
    }
#if WIFI_DEBUG
    else
        printf("connect timeout......\r\n");
#endif

    CommonConnection();
}

/**
 * @author: TanXY
 * @description: 完整的连接命令，用于首次连接
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
            printf("step0-> config as station mode\r\n");
#endif
            if (Wifi_SendCmd("AT+CWMODE=1", 5000))
            {
#if WIFI_DEBUG
                printf("step0-> config OK\r\n");
#endif
                esp8266_step++;
            }
        }
        else if (esp8266_step == 1)
        {
#if WIFI_DEBUG
            printf("step1-> config as single access mode\r\n");
#endif
            if (Wifi_SendCmd("AT+CIPMUX=0", 3000))
            {
#if WIFI_DEBUG
                printf("step1-> config OK\r\n");
#endif
                esp8266_step++;
            }
        }
        else if (esp8266_step == 2)
        {
            try_count++;
#if WIFI_DEBUG
            printf("step3-> connect To=>\r\nSSID: %s password:%s\r\n", ssid, pwd);
#endif
            if (Wifi_SendCmd(router, 8000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("connection successfully!\r\n");
#endif
            }
            if (try_count >= 4)
            {
#if WIFI_DEBUG
                printf("try failed\r\n");
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
            printf("step6-> connect server=>\r\nIP:%s:%d", host, port);
#endif
            if (Wifi_SendCmd(server, 6000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("connect successfully!\r\n");
#endif
                try_count = 0;
            }
            if (try_count > 3)
            {
#if WIFI_DEBUG
                printf("try failed\r\n");
#endif
                try_count = 0;
                Wifi_SendCmd("AT+RESTORE", 5000);
                __set_FAULTMASK(1);
                NVIC_SystemReset();
            }
        }
        else if (esp8266_step == 4)
        {
#if WIFI_DEBUG
            printf("step7-> config as transparent transmission!\r\n");
#endif
            if (Wifi_SendCmd("AT+CIPMODE=1", 4000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("config successfully!\r\n");
#endif
            }
        }
        else if (esp8266_step == 5)
        {
#if WIFI_DEBUG
            printf("step8-> start transparent transmission...\r\n");
#endif
            if (Wifi_SendCmd("AT+CIPSEND", 4000))
            {
                esp8266_step++;
#if WIFI_DEBUG
                printf("start transmission!\r\n");
#endif
                break;
            }
        }
    }
}

/**
 * @author: TanXY
 * @description: 等待模块连接上wifi
 * @param: command：命令字符串 timeout：超时时间
 * @return: 0：命令接收失败  1：命令接收成功
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
 * @description: 发送wifi命令
 * @param: command：命令字符串 timeout：超时时间 单位秒
 * @return: 0：命令接收失败  1：命令接收成功
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
 * @description: wifi复位，使某些配置生效，不需要掉电重启
 * @param: timeout 等待超时时间
 * @return: 0-复位失败 1-复位成功
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

//串口1发送一个字符
void Wifi_SendChar(u8 ch)
{
    while ((WifiUsart->SR & 0x40) == 0)
        ;

    WifiUsart->DR = (u8)ch;
}

/**
 * @author: TanXY
 * @description: 通过wifi发送数据
 * @param: str 待发送的数据
 */
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

void Uart4_Init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); //使能USART4，GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);						//GPIOA0复用为USART4
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);						//GPIOA1复用为USART4
    
    //UART4_TX   GPIOA.1  UART4_RX   GPIOA.0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
    
    //USART4 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; //抢占优先级4
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化NVIC寄存器
    
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                                     //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式

    USART_Init(UART4, &USART_InitStructure);      //初始化串口4
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_Cmd(UART4, ENABLE);                     //使能串口4
}
