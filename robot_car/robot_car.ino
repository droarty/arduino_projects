/*
* Firmware for the ”2WD Ultrasonic Motor Robot Car Kit”
*
* Stephen A. Edwards
* Adapted by droarty
* 
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";

/*
* Hardware configuration :
* A pair of DC motors driven by an L298N H bridge motor driver
* An HC-SR04 ultrasonic range sensor mounted atop a small hobby servo
*/
#include <Servo.h>
Servo servo;
// Ultrasonic Module pins
const int trigPin = A2; // 10 microsecond high pulse causes chirp , wait 50 us
const int echoPin = A1; // Width of high pulse indicates distance
// Servo motor that aims ultrasonic sensor .
const int servoPin = A0; // PWM output for hobby servo
// Motor control pins : L298N H bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control

int buttonState = 0;
enum Motor { LEFT, RIGHT };

// Set motor speed: 255 full ahead, -255 full reverse , 0 stop
void go( enum Motor m, int speed)
{
digitalWrite (m == LEFT ? in1Pin : in3Pin , speed > 0 ? HIGH : LOW );
digitalWrite (m == LEFT ? in2Pin : in4Pin , speed <= 0 ? HIGH : LOW );
analogWrite(m == LEFT ? enAPin : enBPin, speed < 0 ? -speed : speed );
}



// Initial motor test :
// left motor forward then back
// right motor forward then back
void testMotors ()
{
static int speed[8] = { 128, 255, 128, 0 ,
-128, -255, -128, 0};
go(RIGHT, 0);
for (unsigned char i = 0 ; i < 8 ; i++)
go(LEFT, speed[i ]), delay (200);
for (unsigned char i = 0 ; i < 8 ; i++)
go(RIGHT, speed[i ]), delay (200);
}



// Read distance from the ultrasonic sensor , return distance in mm
//
// Speed of sound in dry air , 20C is 343 m/s
// pulseIn returns time in microseconds (10ˆ-6)
// 2d = p * 10ˆ-6 s * 343 m/s = p * 0.00343 m = p * 0.343 mm/us
unsigned int readDistance ()
{
digitalWrite ( trigPin , HIGH );
delayMicroseconds (10);
digitalWrite ( trigPin , LOW );
unsigned long period = pulseIn ( echoPin, HIGH );
return period * 343 / 2000;
}



#define NUM_ANGLES 7
unsigned char sensorAngle[NUM_ANGLES] = { 60, 70, 80, 90, 100, 110, 120 };
unsigned int distance [NUM_ANGLES];
// Scan the area ahead by sweeping the ultrasonic sensor left and right
// and recording the distance observed. This takes a reading , then
// sends the servo to the next angle. Call repeatedly once every 50 ms or so.
void readNextDistance ()
{
  static unsigned char angleIndex = 0;
  static signed char step = 1;
  distance [angleIndex ] = readDistance ();
  angleIndex += step ;
  if (angleIndex == NUM_ANGLES - 1) step = -1;
  else if (angleIndex == 0) step = 1;
  servo . write ( sensorAngle[angleIndex ] );
}


void get_distance() {
  Serial.println( readDistance() );
}

// Get the next sensor reading
// If anything appears to be too close , back up
// Otherwise, go forward
void run_autonomous() {
  readNextDistance ();
  // See if something is too close at any angle
  unsigned char tooClose = 0;
  for (unsigned char i = 0 ; i < NUM_ANGLES ; i++) {
    Serial.print(distance[i]);
    if ( distance [ i ] < 300)
      tooClose = 1;
  }
  if (tooClose) {
    // Something's nearby: back up left
    go(LEFT, -180);
    go(RIGHT, -80);
    Serial.println("... backing up");
  } else {
    // Nothing in our way: go forward
    go(LEFT, 255);
    go(RIGHT, 255);
    Serial.println("... going forward");
  }
  // Check the next direction in 25 ms
  delay (25);
}

// Initial configuration
//
// Configure the input and output pins
// Center the servo
// Turn off the motors
// Test the motors
// Scan the surroundings once
//
void setup () {
  pinMode(trigPin , OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite ( trigPin , LOW);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  servo . attach ( servoPin );
  servo . write (90);
  go(LEFT, 0);
  go(RIGHT, 0);
  testMotors ();
  // Scan the surroundings before starting
  servo . write ( sensorAngle[0] );
  delay (200);
  for (unsigned char i = 0 ; i < NUM_ANGLES ; i ++){
    readNextDistance ();
    delay (200);
  }
  
  // set up wireless
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
  Serial.println("..................");
}



// Main loop:
//
void loop () {
//  if (radio.isChipConnected()) {
//    Serial.print("is Connected ++++");
//  }
//  else {
//    Serial.print("not connected ----");
//  }

  if (radio.available()) {
    Serial.print(".");
    int inputArray[3];
    radio.read(&inputArray, sizeof(inputArray));
    int motorSpeed = (inputArray[0] - 512)/4;
    int spinSpeed = (inputArray[1] - 512)/8;
    int leftMotorSpeed = motorSpeed + buttonState * spinSpeed;
    int rightMotorSpeed = motorSpeed - buttonState * spinSpeed;
    buttonState = inputArray[2];
    if(buttonState == 0) {
      Serial.print(leftMotorSpeed);
      Serial.print(":");
      Serial.print(rightMotorSpeed);
      Serial.print(":");
      Serial.print(buttonState);
      Serial.println();
      go(RIGHT, rightMotorSpeed);
      go(LEFT , leftMotorSpeed);
    }
    else {
      run_autonomous();
    }
  }
  else {
//    go(RIGHT, 0);
//    go(LEFT, 0);
  }
  delay (200);
  Serial.println(">");
}


//
void loop_to_center_servo () {
  servo . write ( 0 );
  delay(2000);
  servo . write ( 90 );
  delay(2000);
  servo . write ( 180 );
  delay(2000);
  servo . write ( 90 );
  delay(10000);
}
