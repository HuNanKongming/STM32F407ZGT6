#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

#define USART1_REC_NUM  			100  	//定义最大接收字节数 200
extern u8 uart_byte_count;          //uart_byte_count要小于USART_REC_LEN
extern u8 receive_str[USART1_REC_NUM];  

void uart1_init(u32 bound);
void uart1SendChars(u8 *str, u16 strlen);
void USART_Send(uint8_t *Buffer, uint8_t Length);
void USART1_send_byte(u8 ch);
extern u8 TEMP_data[20],Receive_ok;

#endif


