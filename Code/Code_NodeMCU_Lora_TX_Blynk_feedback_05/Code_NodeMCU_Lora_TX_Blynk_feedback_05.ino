/**********************************************************************************
 *  TITLE: Transceiver LoRa sketch to send signal using Button & WiFi (Blynk) and receive feedbacks
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/OgED6Mr6lsM
 *  Related Blog : https://iotcircuithub.com/arduino-projects/
 *  by Tech StudyCell
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP8266 NodeMCU : https://github.com/esp8266/Arduino
 *  Download the libraries
 *  Blynk 1.0.1 Library:  https://github.com/blynkkk/blynk-library
 **********************************************************************************/

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

String lora_band = "865000000"; //enter band as per your country
String lora_networkid = "5";    //enter Lora Network ID
String lora_address = "1";      //enter Lora address
String lora_RX_address = "2";   //enter Lora RX address


char auth[] = BLYNK_AUTH_TOKEN;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> 
#include <SoftwareSerial.h>

BlynkTimer timer;

//define ESP8266 NodeMCU GPIO pins
SoftwareSerial mySerial(13, 15); // RX, TX  D7, D8

#define pSwitch_1 10  //SD3
#define pSwitch_2 0   //D3 
#define pSwitch_3 14  //D5
#define pSwitch_4 3   //RX

#define wifiLed   16   //D0
#define sLED       2   //D4

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5

int wifiFlag = 0;
int i;
String incomingString;
boolean state;

void getFeedback(String excpt_str){
  i = 0;
  incomingString = "";
  state = true;

  digitalWrite(sLED, LOW);
  Blynk.virtualWrite(VPIN_BUTTON_5, "Waiting for Feedback...");

  //Serial.print("Waiting for feedback");
  while(1){
    if(mySerial.available()>0){
      incomingString = mySerial.readString();
      Serial.println(incomingString);
      if(incomingString.indexOf(excpt_str) > 0) {
        state = true; break;
      }
    }      
    if (i > 70) {
      state = false; break;
    }
    delay(50);
    digitalWrite(sLED, !digitalRead(sLED));
    i++;
    }

    if(state){
      digitalWrite(sLED, HIGH);
      Blynk.virtualWrite(VPIN_BUTTON_5, "Feedback Received.");
      //Serial.println("Received");
    }
    else{
      digitalWrite(sLED, LOW);
      Blynk.virtualWrite(VPIN_BUTTON_5, "Feedback Not Received.");
      //Serial.println("Not Received");
    }    
}

void manual_control()
{
  if (digitalRead(pSwitch_1) == LOW){
      mySerial.println("AT+SEND="+ lora_RX_address +",2,R1");
      getFeedback("FR1");
    }
  else if (digitalRead(pSwitch_2) == LOW){
      mySerial.println("AT+SEND="+ lora_RX_address +",2,R2");
      getFeedback("FR2");
    }
  else if (digitalRead(pSwitch_3) == LOW){
      mySerial.println("AT+SEND="+ lora_RX_address +",2,R3");
      getFeedback("FR3");
    }
  else if (digitalRead(pSwitch_4) == LOW){
      mySerial.println("AT+SEND="+ lora_RX_address +",2,R4");
      getFeedback("FR4");
    }
}

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1) {
  if(param.asInt() == 1){
    mySerial.println("AT+SEND="+ lora_RX_address +",2,R1");
    getFeedback("FR1");
  }
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  if(param.asInt() == 1){
    mySerial.println("AT+SEND="+ lora_RX_address +",2,R2");
    getFeedback("FR2");
  }
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  if(param.asInt() == 1){
    mySerial.println("AT+SEND="+ lora_RX_address +",2,R3");
    getFeedback("FR3");
  }
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  if(param.asInt() == 1){
    mySerial.println("AT+SEND="+ lora_RX_address +",2,R4");
    getFeedback("FR4");
  }
}

void checkBlynkStatus() 
{ // called every 3 seconds by SimpleTimer
  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(wifiLed, LOW); //Turn on WiFi LED
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(115200);
  pinMode(wifiLed, OUTPUT);
  pinMode(sLED, OUTPUT);
  
  pinMode(pSwitch_1, INPUT_PULLUP);
  pinMode(pSwitch_2, INPUT_PULLUP);
  pinMode(pSwitch_3, INPUT_PULLUP);
  pinMode(pSwitch_4, INPUT_PULLUP);

  digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
  digitalWrite(sLED, HIGH);   //Turn on Status LED
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  
  delay(1500);
  mySerial.println("AT+BAND=" + lora_band);
  delay(500);
  mySerial.println("AT+ADDRESS=" + lora_address);
  delay(500);
  mySerial.println("AT+NETWORKID=" + lora_networkid);
  
  digitalWrite(sLED, LOW);
  Blynk.virtualWrite(VPIN_BUTTON_5, "Ready...");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (mySerial.available()){
    Serial.write(mySerial.read());
  }
  
  Blynk.run();
  manual_control();
  
  timer.run(); // Initiates SimpleTimer
}
