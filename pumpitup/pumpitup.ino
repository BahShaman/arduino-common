
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
 unsigned long waitmillis = 1000;
 unsigned long lastmillis  = 0;
 int degrees = 45;

 void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(ledPin, OUTPUT);              // initializes digital pins 0 to 7 as outputs
  waitmillis = 60000; //ten minutes
  //Serial.begin(38400);
  pressbutton();
  
 }

 void pressbutton(){
    for(int x=0;x<degrees;x++){
        delay(5);             
        myservo.write(x);              // tell servo to go to position in variable 'pos'
    }
    delay(10);
    for(int y=degrees;y>0;y--){
        delay(5);             
        myservo.write(y);              // tell servo to go to position in variable 'pos'
    }
 }

 void loop() {
  // reads the value of the variable resistor 
  unsigned long nextmillis = lastmillis + waitmillis;

  if(millis() > nextmillis){
  //Serial.print('N');
  //Serial.println(nextmillis);
  //Serial.print('L');
  //Serial.println(lastmillis);
  //Serial.print('W');
  //Serial.println(waitmillis);
    lastmillis = millis();
    pressbutton();
  }
  
 }

