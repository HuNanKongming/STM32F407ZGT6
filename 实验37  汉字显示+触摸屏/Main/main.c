#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "malloc.h" 
#include "tfcard_sdio.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"     
#include "updatefont.h"  //����HanZiUse.lib�и����ֿ⺯�� ����Ҽ�ѡ���ͷ�ļ��ɲ鿴��ͷ�ļ�
#include "showhz.h"	     //����HanZiUse.lib����ʾ���ֺ���

#include "usart1.h"
#include "touch.h"
#include "xpt2046.h" 
#include "ft5426.h"
/*********************************************************************************
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: ����15 ������ʾ                                                      *
* �ļ�������������LCD����ʾ                                                      *
* �������ڣ�2015.03.14                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����������ʹ����������װ�⺯��������������ҵ������⺯����Ӧ��.h�ļ����� *
            ��ϸ���˽����ʹ�øÿ⺯����ʹ�÷�װ�⺯�������⣺������ĵײ㣬ֻ�� *
						�˽�ӿں�����Ӧ�þ�����˼򵥡���������ֲ��ʱ��Ҫע���װ����õ��� *
						�ⲿ����������˵����ֲʱҲҪ�ѷ�װ�������õ�����Ҳ�ӵ�������������ϸ *
						�鿴.h˵��                                                           * 
* �Ա����̣�https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

 
int main(void)
{        
	u8 i;
	FATFS *fsTF;  //TF���ļ�ϵͳ		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();         //��ʼ����ʱ����
	LED_Init();					  //��ʼ��LED  
 	LCD_Init();					  //LCD��ʼ��  
 	KEY_Init();					  //������ʼ��  
	W25QXX_Init();			  //��ʼ��W25Q128

//===============================================	
uart1_init(9600);		//��ʼ�����ڲ�����Ϊ115200
Touch_Init();				//��������ʼ��
/******************HanZiUse.lib���� �����ֿ�Ҫ�õ���**********************/
	Memory_Init(INSRAM);	//��ʼ���ڲ��ڴ��  

 	fsTF=(FATFS*)Mem_malloc(INSRAM,sizeof(FATFS));	//Ϊ�ļ�ϵͳ�����ڴ� 
  f_mount(fsTF,"0:",1); 		//����TF�� 
/***********************************************************************/	
	
	while(font_init()) 	 //��������ֿ⣬�ֿ�ȱʧ�����ֿ�
	{ 
		update_hzfont();   //�����ֿ�	      
	}  

	BRUSH_COLOR=RED;       
//  LCD_DisplayString(10,10,24,"Illuminati STM32");	
//  LCD_DisplayString(10,40,16,"Author:Clever");
//	LCD_DisplayHZstr(30,80,24,"15.������ʾ");		
//  LCD_DisplayHZstr(30,110,16,"KEY0:�����ֿ�");	
//  LCD_DisplayHZstr(30,130,16,"KEY1:���Ժ���");	
	 
	while(1)
	{
     key_scan(0); 	
		 LCD_DisplayHZstr(100,250,16,"����LED3");
		 LCD_DisplayHZstr(100,290,16,"Ϩ��LED3");
		 if(lcd_id==0x9341)
					R_Touch_test();     //������������Բ���

//		 if(keydown_data==KEY0_DATA)  //��KEY0��TF���е��ֿ���µ�W25Q128
//		 {
//		   update_hzfont(); 
//			 LCD_DisplayString(10,10,24,"Illuminati STM32");	
//       LCD_DisplayString(10,40,16,"Author:Clever");
//	     LCD_DisplayHZstr(30,80,24,"15.������ʾ");
//			 LCD_DisplayHZstr(30,110,16,"KEY0:�����ֿ�");	
//       LCD_DisplayHZstr(30,130,16,"KEY1:���Ժ���");	    			 
//		 }
//		 if(keydown_data==KEY1_DATA)
//		 {		 
//		  switch(i)
//			   {
//				  case 0: 
//						{
//							LCD_DisplayHZstr(30,160,16,"���Ժ��ֵ�һ�仰");
//					    LCD_DisplayHZstr(30,180,16,"MCU ����STM32F4������");
//							break;
//					  }
//						case 1: 
//						{
//							LCD_DisplayHZstr(30,160,16,"���Ժ��ֵڶ��仰");
//					    LCD_DisplayHZstr(30,180,16,"MCU���������������ʦ");
//							break;
//					  }
//						case 2: 
//						{
//							LCD_DisplayHZstr(30,160,16,"���Ժ��ֵ����仰");
//					    LCD_DisplayHZstr(30,180,16,"���㿪��ѧϰF4�Ĵ��� ");
//							break;
//					  }
//			   }
//				i++;
//			  if(i>2) i=0;
//		 }
//		delay_ms(5);
	} 
}







