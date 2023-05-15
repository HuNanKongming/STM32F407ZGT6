#include "lcd.h"
#include "picture.h"
#include "color.h"

/*********************************************************************************
***********************MCU启明 STM32F407开发板(高配版)****************************
**********************************************************************************
* 文件名称: picture.c                                                            *
* 文件简述：位图显示驱动程序                                                     *
* 创建日期：2015.04.05                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：该代码编写思路参考网上代码                                           * 
**********************************************************************************
*********************************************************************************/	 	
 
/****************************************************************************
* 名    称: u16 picture_getcolor(u8 mode,u8 *str)
* 功    能：从8位数据获得16位颜色 
* 入口参数：mode:1,低位在前,高位在后.
                 0,高位在前,低位在后.
						str:数据
* 返回参数：无
* 说    明：     
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
* 名    称: void picture_show(u16 xsta,u16 ysta,u16 width,u16 height,u8 scan,u8 *p)
* 功    能：在液晶上画图（仅支持：从左到右，从上到下 or 从上到下，从左到右 的扫描方式！
* 入口参数：xsta,ysta,width,height:画图区域
            *p:图像数据
* 返回参数：无
* 说    明：     
****************************************************************************/ 
void picture_show(u16 xsta,u16 ysta,u16 width,u16 height,u8 scan,u8 *p)
{  
	u32 i;
	u32 len=0;
	if((scan&0x03)==0)//水平扫描
	{
		LCD_AUTOScan_Dir(L2R_U2D);//从左到右,从上到下
		LCD_Set_Window(xsta,ysta,width,height);
		LCD_SetCursor(xsta,ysta);//设置光标位置 
				
	}else  //垂直扫描
	{  
		LCD_AUTOScan_Dir(U2D_L2R);//从上到下,从左到右
		LCD_Set_Window(xsta,ysta,width,height);
		LCD_SetCursor(xsta,ysta);//设置光标位置  
	}
	LCD_WriteRAM_Prepare();   //开始写入GRAM
	len=width*height;			   //写入的数据长度
	for(i=0;i<len;i++)
	{
		LCD_DATA=picture_getcolor(scan&(1<<4),p); 
		p+=2;
	} 	  
	LCD_Set_Window(0,0,lcd_width,lcd_height);  	    
}  

/****************************************************************************
* 名    称: void picture_display(u16 x,u16 y,u8 * imgx)
* 功    能：在指定的位置显示一个图片
* 入口参数：x,y:指定位置
            imgx:图片数据(必须包含图片信息头,"4096色/16位真彩色/18位真彩色/24位真彩色/32位真彩色”的图像数据头)
						str:数据
* 返回参数：无
* 说    明：注意:针对STM32,不能选择image2lcd的"高位在前(MSB First)"选项,否则imginfo的数据将不正确!!     
****************************************************************************/ 
void picture_display(u16 x,u16 y,u8 * imgx)
{
	COLORHEAD *imginfo;
 	u8 ifosize=sizeof(COLORHEAD); //得到COLORHEAD结构体的大小
	imginfo=(COLORHEAD*)imgx;
 	picture_show(x,y,imginfo->w,imginfo->h,imginfo->scan,imgx+ifosize);		
}


