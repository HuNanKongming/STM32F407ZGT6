#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "common.h"

#define WIFI_RESET PFout(14) // PB1

#define WIFI_BUF_SIZE    128
#define LINK_STEP          5
#define WIFI_DEBUG      1
#define WifiUsart       UART4
#define UART4_REC_NUM  200  	//定义最大接收字节数 200
extern u8 uart_byte_count;          //uart_byte_count要小于USART_REC_LEN
extern u8 receive_str[UART4_REC_NUM]; 

void ESP8266_Init(void);
void Wifi_Interrupt(void);
void Wifi_SendData(char *str);
uint8_t WaitConnection(uint16_t timeout);
void CommonConnection(void);
uint8_t Wifi_SendCmd(char *command, uint16_t timeout);
uint8_t Wifi_Reset(uint16_t timeout);

#endif
