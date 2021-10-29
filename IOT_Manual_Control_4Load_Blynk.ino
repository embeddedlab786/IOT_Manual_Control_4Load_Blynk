/* 
 https://arduino.esp8266.com/stable/package_esp8266com_index.json
 */
 
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>

char AUTH[] = "-----------------"; //Auth code sent via Email
char WIFI_SSID[] = "M.Ansar"; //Wifi name
char WIFI_PASS[] = "78612345";  //Wifi Password

// define the GPIO connected with Relays and switches
#define Relay1  D0 //GPIO 16
#define Relay2  D1 //GPIO 5
#define Relay3  D2 //GPIO 4
#define Relay4  D3 //GPIO 0

#define Switch1 10 //SD3
#define Switch2 9  //SD2
#define Switch3 12 //D5
#define Switch4 14 //D6

#define wifiLed D4 //GPIO 2

int load1, load2, load3, load4;
int wifiFlag = 1;

BlynkTimer timer;

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer
  bool isconnected = Blynk.connected();
  if (isconnected == false) {wifiFlag = 1;
    digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  }
  if (isconnected == true) {
  if(wifiFlag==1){wifiFlag = 0;
  update_blynk();
  }  
    digitalWrite(wifiLed, LOW); //Turn on WiFi LED
  }
}


void setup() {  
Serial.begin(9600);

pinMode(Switch1, INPUT_PULLUP);
pinMode(Switch2, INPUT_PULLUP);
pinMode(Switch3, INPUT_PULLUP);
pinMode(Switch4, INPUT_PULLUP);

EEPROM.begin(512);
load1 = EEPROM.read(1);
load2 = EEPROM.read(2);
load3 = EEPROM.read(3);
load4 = EEPROM.read(4);

pinMode(Relay1, OUTPUT); 
pinMode(Relay2, OUTPUT); 
pinMode(Relay3, OUTPUT); 
pinMode(Relay4, OUTPUT);

Relays();

pinMode(wifiLed, OUTPUT);

WiFi.begin(WIFI_SSID, WIFI_PASS);
timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
Blynk.config(AUTH);
delay(100);
}

void loop() {

if (WiFi.status() != WL_CONNECTED){
    //Serial.println("WiFi Not Connected");
  }
  else{
    //Serial.println("WiFi Connected");
    Blynk.run();
  }
 
if(wifiFlag==0){with_internet();}
           else{without_internet();}

 timer.run(); // Initiates SimpleTimer
 
}

BLYNK_WRITE(V0){
load1 = param.asInt();
Relays();
}

BLYNK_WRITE(V1){
load2 = param.asInt(); 
Relays();
}

BLYNK_WRITE(V2){
load3 = param.asInt(); 
Relays();
}

BLYNK_WRITE(V3){
load4 = param.asInt();
Relays(); 
}

void with_internet(){
     if(digitalRead(Switch1) == LOW){
      load1 = !load1;
      Relays(); 
      update_blynk();
      delay(300); 
    }
else if(digitalRead(Switch2) == LOW){
      load2 = !load2;
      Relays(); 
      update_blynk();
      delay(300);
    }
else if(digitalRead(Switch3) == LOW){
      load3 = !load3;
      Relays(); 
      update_blynk();
      delay(300);
    }
else if(digitalRead(Switch4) == LOW){
      load4 = !load4;
      Relays();
      update_blynk(); 
      delay(300);
   }
}

void without_internet(){
     if(digitalRead(Switch1) == LOW){
      load1 = !load1;
      Relays(); 
      delay(300); 
    }
else if(digitalRead(Switch2) == LOW){
      load2 = !load2;
      Relays(); 
      delay(300);
    }
else if(digitalRead(Switch3) == LOW){
      load3 = !load3;
      Relays(); 
      delay(300);
    }
else if(digitalRead(Switch4) == LOW){
      load4 = !load4;
      Relays(); 
      delay(300);
    }
}

void update_blynk(){
Blynk.virtualWrite(V0, load1);  
Blynk.virtualWrite(V1, load2);
Blynk.virtualWrite(V2, load3);
Blynk.virtualWrite(V3, load4);
}

void Relays(){
digitalWrite(Relay1, load1);  
digitalWrite(Relay2, load2);
digitalWrite(Relay3, load3);
digitalWrite(Relay4, load4);
write_eeprom();
}

void write_eeprom(){
EEPROM.write(1, load1);
EEPROM.write(2, load2);
EEPROM.write(3, load3);
EEPROM.write(4, load4);  
EEPROM.commit(); 
}
