#include "gpiod.h"

void GPIOd_I12_13nit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //ʹ��GPIOGʱ��
  
  //��ʼ����������Ӧ����GPIOG7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);               //��ʼ��GPIO
	
  GPIO_ResetBits(GPIOD,GPIO_Pin_12);                    //��������Ӧ����GPIOG7����,�رշ����
}

