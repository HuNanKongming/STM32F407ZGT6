#include "lcd.h"
#include "picture.h"
#include "color.h"

/*********************************************************************************
***********************MCU���� STM32F407������(�����)****************************
**********************************************************************************
* �ļ�����: picture.c                                                            *
* �ļ�������λͼ��ʾ��������                                                     *
* �������ڣ�2015.04.05                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    �����ô����д˼·�ο����ϴ���                                           * 
**********************************************************************************
*********************************************************************************/	 	
 
/****************************************************************************
* ��    ��: u16 picture_getcolor(u8 mode,u8 *str)
* ��    �ܣ���8λ���ݻ��16λ��ɫ 
* ��ڲ�����mode:1,��λ��ǰ,��λ�ں�.
                 0,��λ��ǰ,��λ�ں�.
						str:����
* ���ز�������
* ˵    ����     
****************************************************************************/ 
u16 picture_getcolor(u8 mode,u8 *str)
{
	u16 color;
	if(mode)
	{
		color=((u16)*str++)<<8;
		color|=*str;
	}else
	{
		color=*str++;
		color|=((u16)*str)<<8;
	}
	return color;	
}

/****************************************************************************
* ��    ��: void picture_show(u16 xsta,u16 ysta,u16 width,u16 height,u8 scan,u8 *p)
* ��    �ܣ���Һ���ϻ�ͼ����֧�֣������ң����ϵ��� or ���ϵ��£������� ��ɨ�跽ʽ��
* ��ڲ�����xsta,ysta,width,height:��ͼ����
            *p:ͼ������
* ���ز�������
* ˵    ����     
****************************************************************************/ 
void picture_show(u16 xsta,u16 ysta,u16 width,u16 height,u8 scan,u8 *p)
{  
	u32 i;
	u32 len=0;
	if((scan&0x03)==0)//ˮƽɨ��
	{
		LCD_AUTOScan_Dir(L2R_U2D);//������,���ϵ���
		LCD_Set_Window(xsta,ysta,width,height);
		LCD_SetCursor(xsta,ysta);//���ù��λ�� 
				
	}else  //��ֱɨ��
	{  
		LCD_AUTOScan_Dir(U2D_L2R);//���ϵ���,������
		LCD_Set_Window(xsta,ysta,width,height);
		LCD_SetCursor(xsta,ysta);//���ù��λ��  
	}
	LCD_WriteRAM_Prepare();   //��ʼд��GRAM
	len=width*height;			   //д������ݳ���
	for(i=0;i<len;i++)
	{
		LCD_DATA=picture_getcolor(scan&(1<<4),p); 
		p+=2;
	} 	  
	LCD_Set_Window(0,0,lcd_width,lcd_height);  	    
}  

/****************************************************************************
* ��    ��: void picture_display(u16 x,u16 y,u8 * imgx)
* ��    �ܣ���ָ����λ����ʾһ��ͼƬ
* ��ڲ�����x,y:ָ��λ��
            imgx:ͼƬ����(�������ͼƬ��Ϣͷ,"4096ɫ/16λ���ɫ/18λ���ɫ/24λ���ɫ/32λ���ɫ����ͼ������ͷ)
						str:����
* ���ز�������
* ˵    ����ע��:���STM32,����ѡ��image2lcd��"��λ��ǰ(MSB First)"ѡ��,����imginfo�����ݽ�����ȷ!!     
****************************************************************************/ 
void picture_display(u16 x,u16 y,u8 * imgx)
{
	COLORHEAD *imginfo;
 	u8 ifosize=sizeof(COLORHEAD); //�õ�COLORHEAD�ṹ��Ĵ�С
	imginfo=(COLORHEAD*)imgx;
 	picture_show(x,y,imginfo->w,imginfo->h,imginfo->scan,imgx+ifosize);		
}


