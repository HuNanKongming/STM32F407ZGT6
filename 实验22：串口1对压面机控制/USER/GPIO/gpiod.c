#include "gpiod.h"

void GPIOd_I12_13nit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOG时钟
  
  //初始化蜂鸣器对应引脚GPIOG7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //下拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);               //初始化GPIO
	
  GPIO_ResetBits(GPIOD,GPIO_Pin_12);                    //蜂鸣器对应引脚GPIOG7拉低,关闭蜂鸣�
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

void GPIOF_1_0nit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructureF;	

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //使能GPIOG时钟
  
  //初始化蜂鸣器对应引脚GPIOG7
  GPIO_InitStructureF.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructureF.GPIO_Mode = GPIO_Mode_OUT;        //普通输出模式
  GPIO_InitStructureF.GPIO_OType = GPIO_OType_PP;       //推挽输出
  GPIO_InitStructureF.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructureF.GPIO_PuPd = GPIO_PuPd_DOWN;       //下拉
  GPIO_Init(GPIOF, &GPIO_InitStructureF);               //初始化GPIO
	
  GPIO_ResetBits(GPIOF,GPIO_Pin_0);                    //蜂鸣器对应引脚GPIOG7拉低,关闭蜂鸣?
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}


