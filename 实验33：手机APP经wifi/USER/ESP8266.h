#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "hardware.h"

#define WIFI_RESET PFout(14) // PB1

#define WIFI_BUF_SIZE    128
#define LINK_STEP          5
#define WIFI_DEBUG      1
#define WifiUsart       UART4

void ESP8266_Init(void);
void Wifi_Interrupt(void);
void Wifi_SendData(char *str);
uint8_t WaitConnection(uint16_t timeout);
void CommonConnection(void);
uint8_t Wifi_SendCmd(char *command, uint16_t timeout);
uint8_t Wifi_Reset(uint16_t timeout);

#endif
