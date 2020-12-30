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
void setup() {
  pinMode(10, OUTPUT);// use pin 10 for uno and nano use 53 for mega
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
  const int on_text[] = {500, 600, -500};
  Serial.print(radio.write(&on_text, sizeof(on_text)));
  Serial.println("on");
  delay(2500);                       // wait for a second
  const int off_text[] ={400, 700, -400};
  Serial.print(radio.write(&off_text, sizeof(off_text)));
  Serial.println("off");
  delay(2500);                       // wait for a second
}


/*
 * Setup *Setu⸮Setup ***********
20:40:31.855 -> test
20:40:31.855 -> test2
20:40:31.889 -> SPI Speedz  = 10 Mhz
20:40:31.889 -> STATUS     = 0xff RX_DR=1 TX_DS=1 MAX_RT=1 RX_P_NO=7 TX_FULL=1
20:40:31.958 -> RX_ADDR_P0-1   = 0xffffffff7f 0xffffffffff
20:40:31.992 -> RX_ADDR_P2-5   = 0x7f 0xff 0x7f 0xff
20:40:32.059 -> TX_ADDR    = 0xffffffff7f
20:40:32.059 -> RX_PW_P0-6   = 0xff 0x7f 0xff 0x7f 0xff 0x7f
20:40:32.126 -> EN_AA    = 0xff
20:40:32.126 -> EN_RXADDR  = 0x00
20:40:32.160 -> RF_CH    = 0x00
20:40:32.160 -> RF_SETUP   = 0x00
20:40:32.193 -> CONFIG     = 0x00
20:40:32.228 -> DYNPD/FEATURE  = 0x7f 0x00
20:40:32.228 -> Data Rate  = 1MBPS
20:40:32.261 -> Model    = nRF24L01
20:40:32.295 -> CRC Length   = Disabled
20:40:32.295 -> PA Power   = PA_MIN
20:40:32.329 -> test3
*/
