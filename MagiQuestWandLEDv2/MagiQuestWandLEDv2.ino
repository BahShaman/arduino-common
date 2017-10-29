/*
 * IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * RCMM protocol added by Matthias Neeracher
 */

#include <IRremote.h>

  int RECV_PIN = 11;
  #define ENABLE_MagiQuest
  IRrecv irrecv(RECV_PIN);
  decode_results results;

typedef struct {
   String wand_name;
   String wand_color;
   long unsigned int wand_id;
   int led_pin;
   long unsigned int last_intensity;
} Wand;



const int NUM_WANDS = 4;
// Name and color are just for printing back to the serial 
// Name, Color, ID, LedPin, ??  //
Wand wands[4] = {
  { "Simon"    , "red"    , 0x0001 , 13 , 0 },
  { "Haylie"   , "green"  , 0x7281 , 9 , 0 },
  { "Josiah"   , "blue"   , 0xFFFFB681 , 10  , 0 },
  { "Nehemiah" , "yellow" , 0xFFFFC001 , 7  , 0 },
};

void setupWands(){
  for (int i = 0; i < NUM_WANDS; i++) {
    pinMode(wands[i].led_pin, OUTPUT);
  }
}

void flashWands(){
  for (int i = 0; i < NUM_WANDS; i++) {
    flashWand(i);
  }
}

void flashWand(int idx){
  Serial.print("Flashing ");
  Serial.print(wands[idx].wand_name);
  Serial.print("'s wand (");
  Serial.print(wands[idx].wand_color);
  Serial.println(")");
  for (int i = 0; i < 2; i++) {
    digitalWrite(wands[idx].led_pin, HIGH);
    delay(125);               // wait for a second
    digitalWrite(wands[idx].led_pin, LOW); 
    delay(125);               // wait for a second
  }
}

void setup()
{
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  Serial.println("starting...");

  setupWands();
  flashWands();
  
}

int findWand(int wandid){
  Serial.print("looking for wand ");
  Serial.println(wandid, HEX);
  
  for (int i = 0; i < NUM_WANDS; i++) {
    //Serial.print("looping for wand ");
    //Serial.println(wands[i].wand_id, HEX);
    if (wands[i].wand_id == wandid){
      return i;
    }
  }
  Serial.println("Wand Not Found");
  return -1;
}

void receiveWand(decode_results *results){
  int count = results->rawlen;

  if (results->decode_type == UNKNOWN) {
    Serial.println("Unknown encoding: ");
  } 
  #ifdef ENABLE_MagiQuest
    else if (results->decode_type == MAGIQUEST) {
    #ifndef IS_AVTINY
      Serial.print("Decoded MAGIQUEST - Magnitude=");
      Serial.print(results->magiquestMagnitude, HEX);
      Serial.print(", wand_id=");
    #endif //IS_AVTINY
    int wandid = results->value;  
      Serial.println(wandid, HEX);
    int wandidx = findWand(wandid);
    if(wandidx > -1){
      flashWand(wandidx);
    }
  #endif
  }
}

// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
//  decode_results *results = (decode_results *)v
void dump(decode_results *results) {

  int count = results->rawlen;

  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  } 

#ifdef ENABLE_MagiQuest
  else if (results->decode_type == MAGIQUEST) {
  #ifndef IS_AVTINY
    Serial.print("Decoded MAGIQUEST - Magnitude=");
    Serial.print(results->magiquestMagnitude, HEX);
    Serial.print(", wand_id=");
  #endif //IS_AVTINY
  int wandid = results->value;  
  Serial.println(wandid, HEX);
    if (wandid == 0x7281) {
      Serial.println("Haylie");
      for (int i = 0; i < 2; i++) {
        digitalWrite(9, HIGH);
        delay(250);               // wait for a second
        digitalWrite(9, LOW); 
        delay(125);               // wait for a second
      }
    }
    if (wandid == 0x0001) {
      Serial.println("Simon");
      for (int i = 0; i < 2; i++) {
        digitalWrite(13, HIGH);
        delay(250);               // wait for a second
        digitalWrite(13, LOW); 
        delay(125);               // wait for a second
      }
    }
  //FFFFB681
    if (wandid == 0xFFFFB681) {
      Serial.println("Josiah");
      for (int i = 0; i < 2; i++) {
        digitalWrite(10, HIGH);
        delay(250);               // wait for a second
        digitalWrite(10, LOW); 
        delay(125);               // wait for a second
      }
    }
    if (wandid == 0xFFFFC001) {
      Serial.println("Nehemiah");
      for (int i = 0; i < 2; i++) {
        digitalWrite(7, HIGH);
        delay(250);               // wait for a second
        digitalWrite(7, LOW); 
        delay(125);               // wait for a second
      }
    }
  }
}
#endif //ENABLE_MagiQuest

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    receiveWand(&results);
    irrecv.resume(); // Receive the next value
  }
}
