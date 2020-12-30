/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

RF24 radio(8,9); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  Serial.println("Setup ***********");
  pinMode(10, OUTPUT);// use pin 10 for uno and nano use 53 for mega
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  if (radio.isChipConnected()) {
    Serial.println("is Connected ****");
  }
  else {
    Serial.println("not connected ...");
  }
  printf_begin();
  Serial.println(F("details::"));
  radio.printDetails();
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  delay(2500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2500);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  delay(2500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2500);                       // wait for a second
}
void loop() {
  Serial.print(".");
  if (radio.isChipConnected()) {
    Serial.println("is Connected ****");
  }
  else {
    Serial.println("not connected ...");
  }
  const char on_text[32] = "On";
  const char off_text[32] = "Off";

  for(byte i=0;i<5;i++) {
    if (radio.available()) {
      int text[3];
      radio.read(&text, sizeof(text));
      Serial.print(sizeof(text));
      Serial.print(":");
      Serial.print(text[0]);
      Serial.print(":");
      Serial.print(text[1]);
      Serial.print(":");
      Serial.print(text[2]);
    }
    delay(200);
    Serial.print(", ");
  }
  Serial.println();
}
