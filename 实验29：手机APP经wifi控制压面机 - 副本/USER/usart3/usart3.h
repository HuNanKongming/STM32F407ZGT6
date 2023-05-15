#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

#define USART3_REC_NUM  			100  	//�����������ֽ��� 200
extern u8 uart3_byte_count;          //uart_byte_countҪС��USART_REC_LEN
extern u8 receive_str3[USART3_REC_NUM];  

void uart3_init(u32 bound);
void uart3SendChars(u8 *str, u16 strlen);
void USART_Send(uint8_t *Buffer, uint8_t Length);
int fputc3(int ch, FILE *f); 
extern u8 TEMP_data[20],Receive_ok3;
#endif


