#include "gpiod.h"

void GPIOd_I12_13nit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Ê¹ÄÜGPIOGÊ±ÖÓ
  
  //³õÊ¼»¯·äÃùÆ÷¶ÔÓ¦Òı½ÅGPIOG7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //ÏÂÀ­
  GPIO_Init(GPIOD, &GPIO_InitStructure);               //³õÊ¼»¯GPIO
	
  GPIO_ResetBits(GPIOD,GPIO_Pin_12);                    //·äÃùÆ÷¶ÔÓ¦Òı½ÅGPIOG7À­µÍ,¹Ø±Õ·äÃùÆ
}

