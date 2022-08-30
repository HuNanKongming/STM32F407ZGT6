#include "led.h"
#include "lcd.h"
#include "key.h"
#include "stdio.h"
/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程4 LCD显示主函数main.c                                            *
* 文件简述：LCD显示实验                                                          *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：LCD显示字符、数字和图形                                              *
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{
  u16 i=1;
  u16 num;
  u8 buf[100] = "0123456789";//字符数组，可以保存字符串
  delay_init();         //初始化延时函数
  LED_Init();					  //初始化LED
  LCD_Init();           //初始化LCD FSMC接口和显示驱动
  KEY_Init();
  BRUSH_COLOR=RED;      //设置画笔颜色为红色



  LCD_DisplayString(10,10,24,"Illuminati STM32F4");
  LCD_DisplayString(20,40,16,"Author:Clever");
  LCD_DisplayString(30,80,24,"4.TFTLCD TEST");
  LCD_DisplayString(50,110,16,"KEY0:Screen Test");
  LCD_DisplayString(50,130,16,"KEY1:Number Test");
  while(1)
    {
      key_scan(0);

      if(keydown_data==KEY0_DATA)
        {
          while(i)
            {
              switch(i)
                {
                case 1:
                  LCD_Clear(BLACK);
                  break;
                case 2:
                  LCD_Clear(BLUE);
                  break;
                case 3:
                  LCD_Clear(RED);
                  break;
                case 4:
                  LCD_Clear(MAGENTA);
                  break;
                case 5:
                  LCD_Clear(GREEN);
                  break;
                case 6:
                  LCD_Clear(CYAN);
                  break;
                case 7:
                  LCD_Clear(YELLOW);
                  break;
                case 8:
                  LCD_Clear(BRRED);
                  break;
                case 9:
                  LCD_Clear(GRAY);
                  break;
                case 10:
                  LCD_Clear(BROWN);
                  break;
                case 11:
                  LCD_Clear(WHITE);
                  break;
                }
              i++;
              LED0=!LED0;
              LED1=!LED1;
              delay_ms(1000);
              if(i==12)
                {
                  i=0;
                  LCD_DisplayString(10,10,24,"Illuminati STM32F4");
                  LCD_DisplayString(20,40,16,"Author:Clever");
                  LCD_DisplayString(30,80,24,"4.TFTLCD TEST");
                  LCD_DisplayString(50,110,16,"KEY0:Screen Test");
                  LCD_DisplayString(50,130,16,"KEY1:Number Test");
                }
            }
        }

      if(keydown_data==KEY1_DATA)
        {
          LCD_Clear(WHITE);
          LCD_DisplayString(20,20,24,"Number Test");
          num=406;

          LCD_DisplayNum(40,60,num,6,24,0);

          LCD_DisplayString(80,90,24,"Shen jingbo");
          LCD_DisplayString(80,120,24,"yyds");
//				 LCD_DisplayNum(40,90,num,6,24,1);
//				LCD_DisplayNum(40,120,num,6,24,1);
          delay_ms(10);
        }

      if(keydown_data==KEY2_DATA)
        {
          LED2=!LED2;   //作指示灯，证明K3已经按下
          LCD_Clear(WHITE);

//					for(i = 10; i >= 0; i--)
//			{
          //将格式化出一个字符串，将字符串赋值给buf数组
          sprintf(buf,"高手%d",i-'0');//将字符串打印输出到buf数组中，相当于给buf数组赋值
          //将buf中的字符串显示到屏幕中

          LCD_DisplayString(120,120,24,"9");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"8");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"7");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"6");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"5");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"4");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"3");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);

          LCD_DisplayString(120,120,24,"2");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"1");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(120,120,24,"0");
          delay_ms(1000);//显示一个会儿
          LCD_Clear(WHITE);
          LCD_DisplayString(50,120,24,"lets go");
          delay_ms(3000);//显示一个会儿
          LCD_Clear(WHITE);


//			}





        }
      if(keydown_data==KEY3_DATA)
        {

          LED0=!LED0;   //作指示灯，证明K3已经按下
          LED1=!LED1;
          LCD_Clear(WHITE);

          for(i = 0; i <100; i++)
            {
              //将格式化出一个字符串，将字符串赋值给buf数组
              sprintf(buf,"高手%d",i-'0');//将字符串打印输出到buf数组中，相当于给buf数组赋值
              //将buf中的字符串显示到屏幕中

              LCD_DisplayString(120,120,24,"9");
              delay_ms(1000);//显示一个会儿



//			}




            }
        }

    }

}

