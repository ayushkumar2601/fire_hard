#include <WiFi.h>
#include <esp_now.h>

#define NODE_ID 1

#define MQ2_PIN 34
#define FLAME_PIN 35
#define BUZZER 32

#define RED1 25
#define RED2 33
#define GREEN1 26
#define GREEN2 27

int smokeThreshold = 3000;
unsigned long startTime;

uint8_t node2[] = {0x1C,0xC3,0xAB,0xB3,0xFD,0x78};
uint8_t node3[] = {0x14,0x33,0x5C,0x03,0x92,0x94};
uint8_t node4[] = {0x70,0x4B,0xCA,0x27,0x78,0x84};

typedef struct struct_message{
  int node;
  bool fire;
  bool sensorFail;
} struct_message;

struct_message outgoingData;

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

  digitalWrite(GREEN1,HIGH);
  digitalWrite(GREEN2,HIGH);

  WiFi.mode(WIFI_STA);
  esp_now_init();

  addPeer(node2);
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

  if(smoke == 0 || smoke >= 4090){

    outgoingData = {NODE_ID,false,true};
    digitalWrite(BUZZER,LOW);
  }

  else if(smoke > smokeThreshold && flame == LOW){

    digitalWrite(RED1,HIGH);
    digitalWrite(RED2,HIGH);
    digitalWrite(GREEN1,LOW);
    digitalWrite(GREEN2,LOW);
    digitalWrite(BUZZER,HIGH);

    outgoingData = {NODE_ID,true,false};
  }

  else{

    digitalWrite(RED1,LOW);
    digitalWrite(RED2,LOW);
    digitalWrite(GREEN1,HIGH);
    digitalWrite(GREEN2,HIGH);
    digitalWrite(BUZZER,LOW);

    outgoingData = {NODE_ID,false,false};
  }

  esp_now_send(NULL,(uint8_t*)&outgoingData,sizeof(outgoingData));
  delay(1000);
}