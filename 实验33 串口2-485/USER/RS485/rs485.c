#include "rs485.h"	
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "beep.h"
	 
/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: rs485.c                                                              *
* 文件简述：rs485初始化                                                          *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/


  	  
//接收缓存区 	
u8 RS485_receive_str[512];   //接收缓冲,最大128个字节.
u8 uart_byte_count=0;        //接收到的数据长度

										 
//初始化IO 串口2   bound:波特率	
void RS485_Init(u32 bound)
{  	 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
  //串口2引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
	
	//USART2    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	
	//PG8推挽输出，485模式控制  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOG6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化PG8
	
	RS485_TX_EN=0;				//初始化默认为接收模式	
	
   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
  USART_Cmd(USART2, ENABLE);  //使能串口 2	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

//串口2接收中断服务函数
void USART2_IRQHandler(void)
{
	u8 rec_data;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
				rec_data =(u8)USART_ReceiveData(USART2);         //(USART2->DR) 读取接收到的数据
        if(rec_data=='S')		  	                         //如果是S，表示是命令信息的起始位
				{
					uart_byte_count=0x01; 
				}

			else if(rec_data=='E')		                         //如果E，表示是命令信息传送的结束位
				{
					if(strcmp("Light_led1",(char *)RS485_receive_str)==0)        LED1=0;	//点亮LED1
					else if(strcmp("Close_led1",(char *)RS485_receive_str)==0)   LED1=1;	//关灭LED1
					else if(strcmp("Open_beep",(char *)RS485_receive_str)==0)    BEEP=1; 	//蜂鸣器响
					else if(strcmp("Close_beep",(char *)RS485_receive_str)==0)   BEEP=0; 	//蜂鸣器不响
					
					for(uart_byte_count=0;uart_byte_count<32;uart_byte_count++) RS485_receive_str[uart_byte_count]=0x00;
					uart_byte_count=0;    
				}				  
			else if((uart_byte_count>0)&&(uart_byte_count<=128))
				{
				   RS485_receive_str[uart_byte_count-1]=rec_data;
				   uart_byte_count++;
				}
	}  											 
} 


/****************************************************************************
* 名    称: void RS485_Send_Data(u8 *buf,u8 len)
* 功    能：RS485发送len个字节
* 入口参数：buf:发送区首地址
            len:发送的字节数 
* 返回参数：无
* 说    明：(为了和本代码的接收匹配,这里建议数据长度不要超过512个字节)       
****************************************************************************/	
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			    //设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
    USART_SendData(USART2,buf[t]); //发送数据
	}	 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);   //等待发送结束		
	uart_byte_count=0;	  
	RS485_TX_EN=0;				//发送完设置为接收模式	
}

//void RS485_Receive_Data(u8 *buf,u8 *len)
//{
//	u8 rxlen=uart_byte_count;
//	u8 i=0;
//	*len=0; //默认为 0
//	delay_ms(10); //等待 10ms,连续超过 10ms 没有接收到一个数据,则认为接收结束
//	if(rxlen==uart_byte_count&&rxlen) //接收到了数据,且接收完成了
//	{
//		GPIO_ResetBits(GPIOE, GPIO_Pin_3 | GPIO_Pin_4);  
//		for(i=0;i<rxlen;i++)
//		{
//		buf[i]=RS485_receive_str[i];
//		}
//		*len=uart_byte_count; //记录本次数据长度
//		uart_byte_count=0; //清零
//	}
//}





