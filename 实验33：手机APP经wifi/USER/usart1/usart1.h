#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

#define USART1_REC_NUM  			100  	//定义最大接收字节数 200
extern u8 uart_byte_count1;          //uart_byte_count1要小于USART_REC_LEN
extern u8 receive_str1[USART1_REC_NUM];  

void uart1_init(u32 bound);
void uart1SendChars(u8 *str, u16 strlen);
void water_suction_pump_close();
void water_suction_pump_open();
void rou_mianji_close();
void rou_mianji_open();	


#endif


