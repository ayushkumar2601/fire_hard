#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define NODE_ID 2

#define MQ2_PIN 34
#define FLAME_PIN 35
#define BUZZER 32

#define RED1 25
#define RED2 33
#define GREEN1 26
#define GREEN2 27

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int smokeThreshold = 3000;
unsigned long startTime;

uint8_t node1[] = {0x28,0x05,0xA5,0x33,0x68,0xC4};
uint8_t node3[] = {0x14,0x33,0x5C,0x03,0x92,0x94};
uint8_t node4[] = {0x70,0x4B,0xCA,0x27,0x78,0x84};

typedef struct struct_message{
  int node;
  bool fire;
  bool sensorFail;
} struct_message;

struct_message incomingData;
struct_message outgoingData;

bool nodeFire[5];
bool nodeFail[5];

void refreshDisplay(){

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  for(int i=1;i<=4;i++){
    if(nodeFire[i]){
      display.setTextSize(2);
      display.setCursor(20,5);
      display.println("FIRE!");
      display.setTextSize(1);
      display.setCursor(0,40);
      display.print("Node ");
      display.print(i);
      display.println(" detected");
      display.display();
      return;
    }
  }

  display.setTextSize(1);
  display.setCursor(20,25);
  display.println("System Normal");
  display.display();
}

void OnDataRecv(const esp_now_recv_info *info,const uint8_t *data,int len){

  memcpy(&incomingData,data,sizeof(incomingData));

  int n = incomingData.node;
  nodeFire[n] = incomingData.fire;
  nodeFail[n] = incomingData.sensorFail;

  refreshDisplay();
}

void addPeer(uint8_t *mac){
  esp_now_peer_info_t peerInfo={};
  memcpy(peerInfo.peer_addr,mac,6);
  peerInfo.channel=0;
  peerInfo.encrypt=false;
  esp_now_add_peer(&peerInfo);
}

void setup(){

  Serial.begin(115200);

  pinMode(MQ2_PIN,INPUT);
  pinMode(FLAME_PIN,INPUT_PULLUP);

  pinMode(RED1,OUTPUT);
  pinMode(RED2,OUTPUT);
  pinMode(GREEN1,OUTPUT);
  pinMode(GREEN2,OUTPUT);
  pinMode(BUZZER,OUTPUT);

  Wire.begin(21,22);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);

  WiFi.mode(WIFI_STA);
  esp_now_init();

  esp_now_register_recv_cb(OnDataRecv);

  addPeer(node1);
  addPeer(node3);
  addPeer(node4);

  startTime = millis();
}

void loop(){

  int smoke = analogRead(MQ2_PIN);
  int flame = digitalRead(FLAME_PIN);

  if(millis()-startTime < 10000){
    delay(1000);
    return;
  }

  if(smoke > smokeThreshold && flame == LOW){

    digitalWrite(RED1,HIGH);
    digitalWrite(RED2,HIGH);
    digitalWrite(GREEN1,LOW);
    digitalWrite(GREEN2,LOW);
    digitalWrite(BUZZER,HIGH);

    nodeFire[NODE_ID]=true;
    outgoingData = {NODE_ID,true,false};
  }

  else{

    digitalWrite(RED1,LOW);
    digitalWrite(RED2,LOW);
    digitalWrite(GREEN1,HIGH);
    digitalWrite(GREEN2,HIGH);
    digitalWrite(BUZZER,LOW);

    nodeFire[NODE_ID]=false;
    outgoingData = {NODE_ID,false,false};
  }

  esp_now_send(NULL,(uint8_t*)&outgoingData,sizeof(outgoingData));
  refreshDisplay();

  delay(1000);
}