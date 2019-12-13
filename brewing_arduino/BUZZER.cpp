#include "buzzer.h"

//****************************************************
//初始化蜂鸣器模块连接的IO设置
//****************************************************
void Init_Buzzer()
{
  pinMode(tonepin,OUTPUT);//设置数字IO脚模式，OUTPUT为输出
}

//****************************************************
//蜂鸣器开
//****************************************************
void Buzzer_On()
{
  unsigned char i,j;
        for(i=0;i<80;i++)//输出一个频率的声音
        {
          digitalWrite(tonepin,HIGH);//发声音
          delay(1);//延时1ms
          digitalWrite(tonepin,LOW);//不发声音
          delay(1);//延时ms
        }
        /*for(i=0;i<100;i++)//输出另一个频率的声音,这里的100与前面的80一样，用来控制频率，可以自己调节
        {
          digitalWrite(tonepin,HIGH);
          delay(2);
          digitalWrite(tonepin,LOW);
          delay(2);
        }*/
}
