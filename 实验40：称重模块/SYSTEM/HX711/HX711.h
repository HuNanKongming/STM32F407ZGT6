#ifndef _HX711_H_
#define _HX711_H_
#include "sys.h"
void Init_Hx711(void);
unsigned long HX711_Read(void);
long Get_Weight(void);
void Get_Maopi(void);

#endif
