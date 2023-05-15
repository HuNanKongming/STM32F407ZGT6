#ifndef _WIFI_H_
#define _WIFI_H_

#include "common.h"
#include "string.h"
#include "stdio.h"

#define Wifi_BufSize 200

#define WIFI_RESET PAout(4)	// PA4

#define WifiUsart USART2

void WIFI_Server_Init(void);
void Wifi_Interrupt(void);
void Wifi_SendData(char *str);
uint8_t Wifi_SendCmd(char *command, uint16_t timeout);
uint8_t Wifi_Reset(uint16_t timeout);

#endif
