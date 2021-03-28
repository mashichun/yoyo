#include <IRremote.h>

//初次打开请参考【加载库文件】教程加载IRremote库

volatile int System_Time; //系统时间
volatile int Speed;//速度
volatile int Interval_Time;//间隔时间
IRrecv irrecv_A3(A3);//定义红外接收引脚  A3
decode_results results_A3;


long ir_item;
//停止
void Stop() {
  digitalWrite(4,LOW);
  analogWrite(5,0);
  digitalWrite(7,LOW);
  analogWrite(6,0);
}

//前进
void Forward() {
  digitalWrite(4,LOW);
  analogWrite(5,Speed);
  digitalWrite(7,LOW);
  analogWrite(6,Speed);
}
//右转
void Right_Turn() {
  digitalWrite(4,LOW);
  analogWrite(5,Speed);
  digitalWrite(7,HIGH);
  analogWrite(6,Speed);
}
//后退
void Back_Off() {
  digitalWrite(4,HIGH);
  analogWrite(5,Speed);
  digitalWrite(7,HIGH);
  analogWrite(6,Speed);
}

//左转
void Left_Turn() {
  digitalWrite(4,HIGH);
  analogWrite(5,Speed);
  digitalWrite(7,LOW);
  analogWrite(6,Speed);
}

void setup(){
  Serial.begin(9600);
  Speed = 160;
  System_Time = 0;
  Interval_Time = 0;
  irrecv_A3.enableIRIn();
  irrecv_A3.enableIRIn();
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

#define COMMAND_UP    0x18
#define COMMAND_DOWN  0x52
#define COMMAND_LEFT  0x8
#define COMMAND_RIGHT 0x5a
#define COMMAND_FLY   0x1c
#define COMMAND_NUM1  0x11

void loop(){
  char buf[64];
  uint16_t command;
  
  // Serial.print("hello yoyo\n");
  if (irrecv_A3.decode()) {

   // irrecv_A3.printIRResultShort(&Serial);
   // Serial.print(irrecv_A3.decodedIRData.address, HEX);
   // Serial.print(irrecv_A3.decodedIRData.command, HEX);

    sprintf(buf, "\naddr(%x:%x:%lx)\n", 
            irrecv_A3.decodedIRData.address, 
            irrecv_A3.decodedIRData.command,
            irrecv_A3.decodedIRData.decodedRawData);
  Serial.print(buf);
    ir_item=results_A3.value;
    String type="UNKNOWN";
    String typelist[14]={"UNKNOWN", "NEC", "SONY", "RC5", "RC6", "DISH", "SHARP", "PANASONIC", "JVC", "SANYO", "MITSUBISHI", "SAMSUNG", "LG", "WHYNTER"};
    if(results_A3.decode_type>=1&&results_A3.decode_type<=13){
      type=typelist[results_A3.decode_type];
    }
    //Serial.print("IR TYPE:"+type+" ");
    
    command = irrecv_A3.decodedIRData.command;
 
    if (command == COMMAND_UP) {
       Forward();
       Serial.print("Yoyo move forward\n");
    } else if (command == COMMAND_DOWN) {
      Back_Off();
      Serial.print("Yoyo move back\n");
    } else if (command == COMMAND_LEFT) {
      Left_Turn();
      Serial.print("Yoyo turn left\n");
    } else if (command == COMMAND_RIGHT) {
      Right_Turn();
      Serial.print("Yoyo turn right\n");
    } else if (command == COMMAND_FLY) {
      Serial.print("Yoyo fly fly high\n");
    } else if (ir_item == 0xFFB04F) {
      Speed = constrain(Speed + 20, 50, 255);
    } else if (ir_item == 0xFF6897) {
      Speed = constrain(Speed - 20, 50, 255);
    }
    Interval_Time = System_Time + 130;
    irrecv_A3.resume();
  } else {
  }
  if (System_Time > Interval_Time) {
    Stop();

  }
  System_Time= millis();

}
