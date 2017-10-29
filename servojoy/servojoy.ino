
 /* Read Jostick
  * ------------
  *
  * Reads two analog pins that are supposed to be
  * connected to a jostick made of two potentiometers
  *
  * We send three bytes back to the comp: one header and two
  * with data as signed bytes, this will take the form:
  *     Jxy\r\n
  *
  * x and y are integers and sent in ASCII 
  * 
  * http://www.0j0.org | http://arduino.berlios.de
  * copyleft 2005 DojoDave for DojoCorp
  */
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

 int ledPin = 13;
 int joyPin1 = 0;                 // slider variable connecetd to analog pin 0
 int value1 = 0;                  // variable to read the value from the analog pin 0

 void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(ledPin, OUTPUT);              // initializes digital pins 0 to 7 as outputs
  Serial.begin(38400);
 }

 int treatValue(int data) {
  return data;  
  //return (data * 9 / 1024) + 48;
  //return (data / 1024);  
 }

 int servoValue(int data) {
  //return (data / 180);
  int v = map(data, 0, 1023, 180, 0);
  return v;  
  //return (data * 9 / 1024) + 48;
  //return (data / 1024);  
 }
 

 void loop() {
  // reads the value of the variable resistor 
  value1 = analogRead(joyPin1);   
  delay(50);             

  //digitalWrite(ledPin, HIGH);           
  //delay(treatValue(value1));
  //digitalWrite(ledPin, LOW);
  //delay(treatValue(value1));
  Serial.print(treatValue(value1));
  Serial.print('-');
  Serial.print(servoValue(value1));
  myservo.write(servoValue(value1));              // tell servo to go to position in variable 'pos'
  Serial.println('-');
 }

