/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

const int analogPin5 = A5;
const int analogPin6 = A6; 
const int analogPin7 = A7;
int button5 = 0;  // variable to store the value read
int lastButton5 = 0;  // variable to store the value read
int yValue = 0;  // variable to store the value read
int xValue = 0;  // variable to store the value read
const int digitalPin2 = 2; // Left motor Direction 2
bool buttonState = false;
int sampleSize = 10;
int sampleCnt = 0;
int xTot = 0;
int yTot = 0;

int outputArray[3];

void setup() {
  Serial.begin(9600);
  Serial.println("Setup ***********");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  if (radio.isChipConnected()) {
    Serial.println("is Connected ****");
  }
  else {
    Serial.println("not connected ...");
  }
  printf_begin();
  Serial.println(F("details::"));
  radio.printDetails();
}
void loop() {
  if (radio.isChipConnected()) {
    Serial.println("is Connected ****");
  }
  else {
    Serial.println("not connected ...");
  }
  xTot = 0;
  yTot = 0;
  sampleCnt = 0;
  for (sampleCnt=0; sampleCnt<sampleSize; sampleCnt++){
    yValue = analogRead(analogPin6);  // read the input pin
//    Serial.print(yValue);          // debug value
//    Serial.print(", ");
    xValue = analogRead(analogPin7);  // read the input pin
//    Serial.print(xValue);          // debug value
//    Serial.print(", ");
    button5 = analogRead(analogPin5);  // read the input pin
//    Serial.print(button5);          // debug value
//    Serial.println();
    if(button5 == 0 && lastButton5 != 0 ) {
      buttonState = !buttonState;
    }
    lastButton5 = button5;

    xTot += xValue;
    yTot += yValue;
    delay(20);
  }

  outputArray[0] = xTot/sampleSize;
  outputArray[1] = yTot/sampleSize;
  outputArray[2] = buttonState;

  String outputStr;
  for (int i: outputArray) {
      outputStr += ":" + String(i);
  }

  Serial.print(sizeof(outputArray));
  Serial.print(":");
  Serial.println(outputStr);
  
  if (!radio.write(&outputArray, sizeof(outputArray))){  
    Serial.println("packet delivery failed");
  }
}
