#include "usart4_wifi.h"
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "lcd.h"
#include "beep.h" 
#include "rtc.h"
#include "stdio.h"
/*********************************************************************************
*********************MCU启明 STM32F407应用开发板(高配版)**************************
**********************************************************************************
* 文件名称: lte.c                                                                *
* 文件简述：WIFI使用                            ·                                *
* 创建日期：2015.10.06                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用手机APP控制开发板                                                * 
**********************************************************************************
*********************************************************************************/	

u8 receive_str[UART4_REC_NUM];     //接收缓存数组,最大USART_REC_LEN个字节 
u8 uart_byte_count=0;
u8 APP_mode=0;          //APP控制模式  0：命令控制区  1：接收发送区

unsigned char MODE[]="AT+CWMODE=3\r\n";
unsigned char Router[]="AT+CWSAP=\"qiming_wifi\",\"0123456789\",11,4\r\n";  //配置成路由器 名字为qiming_wifi 密码0123456789
unsigned char RST[]="AT+RST\r\n";
unsigned char M_Connection[]="AT+CIPMUX=1\r\n";
unsigned char SERVER[]="AT+CIPSERVER=1,5000\r\n";  //端口号5000
unsigned char SEND[]="AT+CIPSEND=\r\n";  //AT+CIPSEND= 发送数据

/****************************************************************************
* 名    称: void uart4_init(u32 bound)
* 功    能：LTE_uart4初始化
* 入口参数：bound：波特率   
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart4_init(u32 bound)
{   
/****************************** 串口4初始化*********************************/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能USART4时钟 
	 	USART_DeInit(UART4);  //复位串口4
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);  //GPIOA0复用为USART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); //GPIOA1复用为USART4
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA0与GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);          //初始化PA9，PA10
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART4, &USART_InitStructure); //初始化串口1	
  USART_Cmd(UART4, ENABLE);  //使能串口4 
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);         //开启相关中断
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;      //串口4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器、
/****************************** 串口4初始化**********************************/  
}

//使能ESP8266 就是置CH_PD为高
void ESP8266_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);          // 
  GPIO_SetBits(GPIOC, GPIO_Pin_1); 
}

void GPIOE_I8_10nit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructureE;	

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //使能GPIOG时钟
  
  //初始化蜂鸣器对应引脚GPIOG7
  GPIO_InitStructureE.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_OUT;        //普通输出模式
  GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;       //推挽输出
  GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_DOWN;       //下拉
  GPIO_Init(GPIOE, &GPIO_InitStructureE);               //初始化GPIO
	
  GPIO_ResetBits(GPIOE,GPIO_Pin_8);                    //蜂鸣器对应引脚GPIOG7拉低,关闭蜂鸣?
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
}

//串口1发送一个字符
void uart4SendChar(u8 ch)
{      
	while((UART4->SR&0x40)==0);  
    UART4->DR = (u8) ch;      
}
/****************************************************************************
* 名    称: void uart1SendChars(u8 *str, u16 strlen)
* 功    能：串口1发送一字符串
* 入口参数：*str：发送的字符串
            strlen：字符串长度
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart4SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart4SendChar(*(str + k)); k++; }   //循环发送,直到发送完毕   
    while (k < strlen); 
} 

void rou_mianji_open()
{
				GPIO_ResetBits(GPIOE,GPIO_Pin_8);
				GPIO_SetBits(GPIOE,GPIO_Pin_10); //PE10为高时候肉面积转，PE10为揉面机的信号线
}

void rou_mianji_close()
{
		GPIO_ResetBits(GPIOE,GPIO_Pin_8);
		GPIO_ResetBits(GPIOE,GPIO_Pin_10); //PE10为高时候肉面积转，PE10为揉面机的信号线
}

void WIFI_Server_Init(void)
{
	uart4_init(115200);	    //串口初始化波特率为115200  wifi模块出厂配置的波特率为115200
	
	uart4SendChars(MODE,sizeof(MODE));   
	delay_ms(1000);
	
	uart4SendChars(Router,sizeof(Router));   //配置wifi模块成路由器 相应的路由名字跟密码
	delay_ms(1000);                          //到这步手机或者电脑就可以搜到名字为qiming_wifi的wifi

	uart4SendChars(RST,sizeof(RST));     //重启模块
	delay_ms(1000);

	uart4SendChars(M_Connection,sizeof(M_Connection));  //开启多连接
	delay_ms(1000);
 
	uart4SendChars(SERVER,sizeof(SERVER));  //配置成服务器 与 设置端口号5000  
	delay_ms(1000);                         //到这步打开手机APP输入设置的IP跟端口号就可以连接了  wifi模块的IP一般固定为192.168.4.1
}
//串口1中断服务程序
void UART4_IRQHandler(void)  
{
	u8 rec_data;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断 
		{
				rec_data =(u8)USART_ReceiveData(UART4);         //(USART1->DR) 读取接收到的数据
				
        if(rec_data=='S')		  	                         //如果是S，表示是命令信息的起始位
				{
					uart_byte_count=0x01; 
				}

			else if(rec_data=='E')		                         //如果E，表示是命令信息传送的结束位
				{
					if(strcmp("open_led1",(char *)receive_str)==0)         LED0=0;	   //点亮LED0
					else if(strcmp("close_led1",(char *)receive_str)==0)   LED0=1;	   //关灭LED0
					
					else if(strcmp("open_led2",(char *)receive_str)==0)    LED1=0;	   //点亮LED1
					else if(strcmp("close_led2",(char *)receive_str)==0)   LED1=1;	   //关灭LED1
					
					else if(strcmp("open_led3",(char *)receive_str)==0)    LED2=0;	   //点亮LED2
					else if(strcmp("close_led3",(char *)receive_str)==0)   LED2=1;	   //关灭LED2
					
					else if(strcmp("open_beep",(char *)receive_str)==0)    BEEP=1; 	   //蜂鸣器响
					else if(strcmp("close_beep",(char *)receive_str)==0)   BEEP=0; 	   //蜂鸣器不响
					else if(strcmp("L0",(char *)receive_str)==0)
            {
              LED0=!LED0;

            }
					
					
					else if(strcmp("app_mode1",(char *)receive_str)==0)    
					    {
								APP_mode=0; 
					      LCD_DisplayString(30,140,16,"APP_mode: 0  ");
			          LCD_DisplayString(30,160,16,"Wait APP Control");
								LCD_Fill_onecolor(0,180,239,319,WHITE);
					    }//APP为状态控制区
					else if(strcmp("app_mode2",(char *)receive_str)==0)    
					    {
								APP_mode=1;
								LCD_DisplayString(30,140,16,"APP_mode: 1  ");
			          LCD_DisplayString(30,160,16,"Receive and send");
								LCD_Fill_onecolor(0,180,239,319,WHITE);
					    }//APP为接收发送区
				//揉面机控制
							 else if(strcmp("打开揉面机",(char *)receive_str)==0)
							{
									//对揉面机进行控制
								rou_mianji_open();
							}
							 else if(strcmp("关闭揉面机",(char *)receive_str)==0)
							{
									//对揉面机进行控制
								rou_mianji_close();
							}
					
					
					
					else if(receive_str[0]=='T')   //时间校准
					    {						
								RTC_DateStruct.RTC_Year= (receive_str[3]-'0')*10 + (receive_str[4]-'0');						
								RTC_DateStruct.RTC_Month= (receive_str[5]-'0')*10 + (receive_str[6]-'0');							
								RTC_DateStruct.RTC_Date= (receive_str[7]-'0')*10 + (receive_str[8]-'0');							
								RTC_TimeStruct.RTC_Hours= (receive_str[9]-'0')*10 + (receive_str[10]-'0');						
								RTC_TimeStruct.RTC_Minutes= (receive_str[11]-'0')*10 + (receive_str[12]-'0');					
								RTC_TimeStruct.RTC_Seconds= (receive_str[13]-'0')*10 + (receive_str[14]-'0');		
		
								RTC_SetTimes(RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
								delay_ms(100);
								uart4SendChars("AT+CIPSEND=0,10\r\n",16);
					      delay_ms(500);
								uart4SendChars("correct_OK",10);
							}
					
				  if(APP_mode==1)   //APP为接收发送区时，显示APP发来的数据  数据一定要以S开头 E结尾
					  {
					    LCD_DisplayString(30,190,16,receive_str);	 //可以根据自己发送的数据 写相应的动作 按前面的编写方法
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


