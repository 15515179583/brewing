#include "water.h"
#include "hx711.h"
#include "stepper.h"
#include "ds18b20.h"
#include "buzzer.h"

/*
 * WiFi配置信息
*/
const char *ssid = "smile";
const char *password = "smile2230550672";
/*
 * 服务器信息配置
*/
String userName = "kangkang";
String toUser = "smile";
const char *serverIP = "192.168.137.1";
int serverPort = 18001;
WiFiClient client;
bool bConnected = false;
char buff[512]="";
int nm = 0;
bool start = false;
String data;
//---------------------------------------------------------------
float Weight = 0,WuliaoWeight,CupWeight=6,WeightMax=150;
int flag=0,WeightFlag=0,WaterFlag=0,StirFlag=0;
int i=0;
typedef struct State {
  double temp;//温度
  float weight;//重量
  bool isWater;//是否抽水
  bool isStir;//是否搅拌
};
State state;
void setup() {
// put your setup code here, to run once:
  Init_Hx711();       //初始化HX711模块连接的IO设置
  DS18B20_Init();     //初始化DS18B20模块连接的IO设置
  Init_Buzzer();      //初始化BUZZER模块连接的IO设置 
  Init_Stepper();     // 设置电机的转速
  Init_Water();       //初始化继电器模块连接的IO设置
  Water_Off();
  Get_Maopi();        //获取毛皮
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  data = "";
  //连接服务器
  if(bConnected == false) {
    if(! client.connect(serverIP,serverPort)) {
      Serial.println("connection failed");
      delay(5000);
      return;
    }
    bConnected = true;
    Serial.println("connection ok");
    client.print(userName);
    client.print(toUser);
  } else if(client.available()) {
      while (client.available()) {
      buff[nm++] = client.read();
    }
    buff[nm] = 0x00;
    nm = 0;
    Serial.print(buff);
    if(buff[0]=='s'&&buff[1]=='t'&&buff[2]=='a'&&buff[3]=='r'&&buff[4]=='t') {
      start = true;
      Get_Maopi(); //初始化
      state.temp =0;
      state.weight = 0;
      state.isWater = false;
      state.isStir = false;
      Serial.println("Welcome to use!\r\n");
    } else if (buff[0]=='e'&&buff[1]=='n'&&buff[2]=='d') {
      start = false;
      client.print("{\"weight\":0,\"temp\":0,\"isWater\":0,\"isStir\":0}");
      Serial.println("beybey!\r\n");
    }
    //client.print(buff);
    client.flush();
  } else {
    if(start) {
      //client.print("data");//将要发送的数据
      if(state.weight>=0) {
        data += "{";
        data += "\"weight\":";
        data += String(state.weight);
        data +=",\"temp\":";
        data += String(state.temp);
        data += ",\"isWater\":";
        data += String(state.isWater);
        data += ",\"isStir\":";
        data += String(state.isStir);
        data += "}";
        Serial.print("you will send : ");
        Serial.print(data);
        Serial.print("\r\n");
        client.print(data);
      }
    }
  }
  //-------------------------------------------------
  //设备运行判断
  if(start == false) {
    return;
  }
  //-------------------------------------------------
  Weight = Get_Weight();  //计算放在传感器上的重物重量
  if(Weight<0) {
    Weight = 0;
  }
  Serial.print(float(Weight/1000),3); //串口显示重量
  state.weight = Weight;
  Serial.print(" kg\n");  //显示单位
  double temp = TempRead();
  temp *= 0.0625;//转换精度为0.0625/LSB
  state.temp = temp;
  Serial.print(temp);
  Serial.print("C\n");   //显示单位
  
  //放杯子和物料
  if(WeightFlag==0){
    WuliaoWeight=Weight-CupWeight;    //测量物料的的重量
    if(WuliaoWeight<=0){
      delay(2000);
      return ;
    }
    else if(WuliaoWeight>1000){       //超重提示
      Buzzer_On();
      delay(2000);
      return ;
    }
    WeightFlag=1;     //物料标志
  }
  delay(1000);
  if(Get_Weight()<= WeightMax &&WaterFlag==0&&WeightFlag==1){
    Water_On();       //抽水
    state.isWater = true;
    return ;
  }
  Water_Off();        //关闭抽水
  state.isWater = false;
  WaterFlag=1;        //抽水标志
  if(StirFlag==0){
    if(i<10){    //搅拌
      state.isStir = true;
      Stepper_Clockwise();
      i++;
      return;
    }
    StirFlag=1;       //搅拌标志
    i=0;
    state.isStir = false;
  }
  if(temp<30&&StirFlag == 1){      //适合饮用
    Buzzer_On();
    if(Weight<CupWeight){     //是否取走,取走休眠
      delay(5000);
      WeightFlag=0;
      WaterFlag=0;
      StirFlag=0;
      state.temp = 0;
      state.weight = 0;
      state.isWater = false;
      state.isStir = false;
      Get_Maopi(); //初始化
    }
  }
}
