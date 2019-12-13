#ifndef __HX711__H__
#define __HX711__H__

#include <ESP8266WiFi.h>

#define HX711_SCK 2
#define HX711_DT 4 

extern void Init_Hx711();
extern unsigned long HX711_Read(void);
extern long Get_Weight();
extern void Get_Maopi();

#endif
