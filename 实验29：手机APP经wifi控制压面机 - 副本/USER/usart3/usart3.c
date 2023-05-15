#include "usart3.h"
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "tim.h"

u8 TEMP_data3[20]={0},Receive_ok3=0;
uint8_t rebuf3[20]={0};
/*********************************************************************************
************************�������� STM32F407���Ŀ�����******************************
**********************************************************************************
* �ļ�����: usart1.c                                                             *
* �ļ�������USART1ʹ��                                                           *
* �������ڣ�2015.03.06                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    �������ô��ڵ������־���USART1����LED��������������                    * 
**********************************************************************************
*********************************************************************************/	

u8 receive_str3[USART3_REC_NUM];     //���ջ�������,���USART_REC_LEN���ֽ� 
u8 uart3_byte_count=0;
//u32 n;

/****************************************************************************
* ��    ��: void uart1_init(u32 bound)
* ��    �ܣ�USART1��ʼ��
* ��ڲ�����bound��������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart3_init(u32 bound)
{   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART1ʱ�� 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10����ΪUSART1
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //��ʼ��PA9��PA10
   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //��������ж�
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

//����1����һ���ַ�
void uart3SendChar(u8 ch)
{      
	while((USART3->SR&0x40)==0);
    USART3->DR = (u8) ch;      
}

/****************************************************************************
* ��    ��: int fputc(int ch, FILE *f)
* ��    �ܣ��ض�����printf���������  
* ��ڲ�����
* ���ز�����
* ˵    ������printf()֮��ĺ�����ʹ���˰�����ģʽ��ʹ�ñ�׼��ᵼ�³����޷�
            ����,�����ǽ������:ʹ��΢��,��Ϊʹ��΢��Ļ�,����ʹ�ð�����ģʽ. 
            ���ڹ������Եġ�Target��-����Code Generation���й�ѡ��Use MicroLIB����
            ���Ժ�Ϳ���ʹ��printf��sprintf������  
****************************************************************************/
int fputc3(int ch, FILE *f)   //�ض�����printf���������  
{
    uart3SendChar(ch);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	
    return ch;
}

/****************************************************************************
* ��    ��: void uart1SendChars(u8 *str, u16 strlen)
* ��    �ܣ�����1����һ�ַ���
* ��ڲ�����*str�����͵��ַ���
            strlen���ַ�������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart3SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart3SendChar(*(str + k)); k++; }   //ѭ������,ֱ���������   
    while (k < strlen); 
} 

//����һ���ֽ�����
//input:byte,�����͵�����
void USART3_send_byte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//�ȴ��������
	USART3->DR=byte;	
}

//���Ͷ��ֽ�����+У���

void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];//�ۼ�Length-1ǰ������
		USART3_send_byte(Buffer[i++]);
	}
}

//����1�жϷ������
void USART3_IRQHandler(void)  
{
		static uint8_t i=0,rebuf3[20]={0};
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//�жϽ��ձ�־
	{
		//LED1 = 0;
		rebuf3[i++]=USART_ReceiveData(USART3);//��ȡ�������ݣ�ͬʱ����ձ�־

		if (rebuf3[0]!=0x5a)//֡ͷ����
			i=0;	
		if ((i==2)&&(rebuf3[1]!=0x5a))//֡ͷ����
			i=0;
	
		if(i>3)//i����4ʱ���Ѿ����յ��������ֽ�rebuf3[3]
		{
			if(i!=(rebuf3[3]+5))//�ж��Ƿ����һ֡�������
				return;	
			switch(rebuf3[2])//������Ϻ���
			{
				case 0x45:
					if(!Receive_ok3)//�����ݴ�����ɺ�Ž����µ�����
					{
						memcpy(TEMP_data3,rebuf3,9);//�������յ�������
				/*for(int j=0;j < 9;j++)
		{
			printf("%c",rebuf3[j]);
		}
						*/
						Receive_ok3=1;//������ɱ�־
					}
					break;
				case 0x15:break;
				case 0x35:break;
			}
			i=0;//������0
		}
	}

} 
 
