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

void GPIOE_I8_10nit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructureE;	

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��GPIOGʱ��
  
  //��ʼ����������Ӧ����GPIOG7
  GPIO_InitStructureE.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_OUT;        //��ͨ���ģʽ
  GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;       //�������
  GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_DOWN;       //����
  GPIO_Init(GPIOE, &GPIO_InitStructureE);               //��ʼ��GPIO
	
  GPIO_ResetBits(GPIOE,GPIO_Pin_8);                    //��������Ӧ����GPIOG7����,�رշ���?
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
}

void GPIOF_1_0nit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructureF;	

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��GPIOGʱ��
  
  //��ʼ����������Ӧ����GPIOG7
  GPIO_InitStructureF.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructureF.GPIO_Mode = GPIO_Mode_OUT;        //��ͨ���ģʽ
  GPIO_InitStructureF.GPIO_OType = GPIO_OType_PP;       //�������
  GPIO_InitStructureF.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructureF.GPIO_PuPd = GPIO_PuPd_DOWN;       //����
  GPIO_Init(GPIOF, &GPIO_InitStructureF);               //��ʼ��GPIO
	
  GPIO_ResetBits(GPIOF,GPIO_Pin_0);                    //��������Ӧ����GPIOG7����,�رշ���?
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}


