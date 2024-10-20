#ifdef ESP8266 //Pilih Board Lolin (Wemos) D1 R1
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>
#ifndef STASSID
#define STASSID "Experia"
#define STAPSK "12345678"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

//Modbus Registers Offsets
const int LED_COIL1= 100;
const int LED_COIL2= 101;
const int Input_COIL1= 100;
const int Input_COIL2= 101;

//Used Pins
const int ledPin1 = 2; //GPIO0
const int ledPin2 = 3;
const int inputPin1 = 5;
const int inputPin2 = 6;

//Status Pin
int LedCoil1_status;
int LedCoil2_status;
int InputCoil1_status;
int InputCoil2_status;

//Kunci-Kunci
int Kunci1;

//ModbusIP object
ModbusIP mb;

long ts;

void setup() {
  Serial.begin(115200);

  //Mode Wifi Ke Station
  WiFi.mode(WIFI_STA);

  //Konek Wifi
  WiFi.begin(ssid, password);
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  delay(1000);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Tampilkan pada Serial Monitor jika dapat terhubung
  Serial.println("");
  Serial.println("WiFi terhubung");
  Serial.println("Alamat IP: ");
  Serial.println(WiFi.localIP());
  delay(2000);

  mb.server();

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  mb.addCoil(LED_COIL1);
  mb.addCoil(LED_COIL2);
  mb.addIsts(Input_COIL1);
  mb.addIsts(Input_COIL2);

  ts = millis();
  Kunci1 = 0;
}

void loop() {
  //Call once inside loop() - all magic here
  mb.task();

  //Read each one seconds
  if (Kunci1 == 0){
    if (millis() > ts + 1000) {
      ts = millis();
      //digitalWrite(ledPin1, HIGH);
      LedCoil1_status = 1;
      InputCoil1_status = 1;
      Kunci1 = 1;
    }
  }
  if (Kunci1 == 1){
    if (millis() > ts + 1000) {
      ts = millis();
      //digitalWrite(ledPin1, LOW);
      LedCoil1_status = 0;
      InputCoil1_status = 0;
      Kunci1 = 0;
    }
  }

  //Attach ledPin to LED_COIL register
  digitalWrite(ledPin1, mb.Coil(LED_COIL1)); // from other Device or Platform
  digitalWrite(ledPin2, mb.Coil(LED_COIL2));

  //Attach LedPin status to Input_Coil register
  mb.Ists(Input_COIL1, InputCoil1_status ); // digitalRead(switchPin)
  mb.Ists(Input_COIL2, InputCoil1_status );

  Serial.print("CoilReg 100:");Serial.print(mb.Coil(LED_COIL1));
  Serial.print("||");
  Serial.print("CoilReg 101:");Serial.print(mb.Coil(LED_COIL2));
  Serial.print("||");
  Serial.print("InputReg 100:");Serial.print(mb.Ists(Input_COIL1));
  Serial.print("||");
  Serial.print("InputReg 101:");Serial.print(mb.Ists(Input_COIL2));
  Serial.print("||");
  Serial.print(InputCoil1_status);
  Serial.println();

  delay(10);
}