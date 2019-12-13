#ifndef __STEPPER__H__
#define __STEPPER__H__

#include <ESP8266WiFi.h>

//使用arduino IDE自带的Stepper.h库文件
#include <Stepper.h>


extern void Init_Stepper();
extern void Stepper_Clockwise();
extern void Stepper_Anticlockwise();

#endif
