/**********************************************************************************
 *  TITLE: Arduino LoRa control 4 Relays with real-time feedback (Receiving end)
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/OgED6Mr6lsM
 *  Related Blog : https://iotcircuithub.com/arduino-projects/
 *  by Tech StudyCell
 *  Board used: Arduino UNO
 *  
 **********************************************************************************/
 
String lora_band = "865000000"; //enter band as per your country
String lora_networkid = "5";    //enter Lora Network ID
String lora_address = "2";      //enter Lora address
String lora_RX_address = "1";   //enter Lora RX address (for sending)

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX D2, D3

// define the GPIO connected with Relays
#define RelayPin1 4 //D4
#define RelayPin2 5 //D5
#define RelayPin3 6 //D6
#define RelayPin4 7 //D7

#define sLed   13   //D13

String incomingString;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(115200);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(sLed, OUTPUT);


  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);

  digitalWrite(sLed, HIGH);
  
  delay(1500);
  mySerial.println("AT+BAND=" + lora_band);
  delay(500);
  mySerial.println("AT+NETWORKID=" + lora_networkid);
  delay(500);
  mySerial.println("AT+ADDRESS=" + lora_address);
  digitalWrite(sLed, LOW);
}

void loop()
{ 
  lora_control();
}

void lora_control(){
  
  if(mySerial.available()) {
    incomingString = mySerial.readString();
    Serial.println(incomingString);
    digitalWrite(sLed, HIGH);
    if(incomingString.indexOf("R1") >0) {
      digitalWrite(RelayPin1, !digitalRead(RelayPin1));
      mySerial.println("AT+SEND="+ lora_RX_address +",3,FR1");
    }
    else if(incomingString.indexOf("R2") >0) {
      digitalWrite(RelayPin2, !digitalRead(RelayPin2));
      mySerial.println("AT+SEND="+ lora_RX_address +",3,FR2");
    }
    else if(incomingString.indexOf("R3") >0) {
      digitalWrite(RelayPin3, !digitalRead(RelayPin3));
      mySerial.println("AT+SEND="+ lora_RX_address +",3,FR3");
    }
    else if(incomingString.indexOf("R4") >0) {
      digitalWrite(RelayPin4, !digitalRead(RelayPin4));
      mySerial.println("AT+SEND="+ lora_RX_address +",3,FR4");
    }
   delay(100);
   digitalWrite(sLed, LOW);
 }
}
