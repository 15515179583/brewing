#ifndef __DS18B20__H__
#define __DS18B20__H__

#include <ESP8266WiFi.h>

#define  DSPIN 3

extern boolean DS18B20_Init();
extern void DS18B20_Write(byte data);
extern byte DS18B20_Read();
extern int TempRead();

#endif
