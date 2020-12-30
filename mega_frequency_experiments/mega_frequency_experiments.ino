void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("setup");
  pinMode(6, OUTPUT);
  tone(6,40000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(7, HIGH); // sets the digital pin 13 on
  delay(30);            // waits for a second
  digitalWrite(7, LOW);  // sets the digital pin 13 off
  delay(30);            // waits for a second

}
