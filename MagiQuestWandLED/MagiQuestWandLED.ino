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

void setup()
{
  Serial.begin(9600);
  Serial.println("starting...");
  irrecv.enableIRIn(); // Start the receiver
  pinMode(13, OUTPUT); //LED on Model A   
  pinMode(9, OUTPUT); //LED on Model A   
  pinMode(10, OUTPUT); //LED on Model A   
  pinMode(7, OUTPUT); //LED on Model A   

  for (int i = 0; i < 5; i++) {
    digitalWrite(1, HIGH);
    delay(125);               // wait for a second
    digitalWrite(1, LOW); 
    delay(125);               // wait for a second
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(13, HIGH);
    delay(125);               // wait for a second
    digitalWrite(13, LOW); 
    delay(125);               // wait for a second
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(9, HIGH);
    delay(125);               // wait for a second
    digitalWrite(9, LOW); 
    delay(125);               // wait for a second
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(10, HIGH);
    delay(125);               // wait for a second
    digitalWrite(10, LOW); 
    delay(125);               // wait for a second
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(7, HIGH);
    delay(125);               // wait for a second
    digitalWrite(7, LOW); 
    delay(125);               // wait for a second
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
  int wand = results->value;  
  Serial.println(wand, HEX);
    if (wand == 0x7281) {
      Serial.println("Haylie");
      for (int i = 0; i < 2; i++) {
        digitalWrite(9, HIGH);
        delay(250);               // wait for a second
        digitalWrite(9, LOW); 
        delay(125);               // wait for a second
      }
    }
    if (wand == 0x0001) {
      Serial.println("Simon");
      for (int i = 0; i < 2; i++) {
        digitalWrite(13, HIGH);
        delay(250);               // wait for a second
        digitalWrite(13, LOW); 
        delay(125);               // wait for a second
      }
    }
  //FFFFB681
    if (wand == 0xFFFFB681) {
      Serial.println("Josiah");
      for (int i = 0; i < 2; i++) {
        digitalWrite(10, HIGH);
        delay(250);               // wait for a second
        digitalWrite(10, LOW); 
        delay(125);               // wait for a second
      }
    }
    if (wand == 0xFFFFC001) {
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
    Serial.println(results.value, HEX);
    dump(&results);
    irrecv.resume(); // Receive the next value
  }
}
