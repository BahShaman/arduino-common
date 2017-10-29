#include <IRremote.h>

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

//adding a second receiver
  int WINNER_LEFT = 0;
  int WINNER_RIGHT = 1;
  int WINNER_NONE = 99;
  int RECV_PIN_0 = 11;
  int RECV_PIN_1 = 6;
  #define IS_AVTINY
  #define ENABLE_MagiQuest
  IRrecv irrecvLeft(RECV_PIN_0);
  decode_results resultsLeft;
  IRrecv irrecvRight(RECV_PIN_1);
  decode_results resultsRight;
  int winner;
  bool GAME_ON;

typedef struct {
   String wand_name;
   String wand_color;
   long unsigned int wand_id;
   int led_pin;
   long unsigned int last_intensity;
} Wand;



const int NUM_WANDS = 4;
// Name and color are just for printing back to the serial 
// Name, Color, ID, LedPin, last intensity (see struct)  //
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
  irrecvRight.enableIRIn(); // Start the receiver
  irrecvLeft.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  Serial.println("starting...");

  winner = WINNER_NONE;

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
  #endif
  }
}


bool isValidWand(decode_results *results){
    int wandid = results->value;  
      Serial.println(wandid, HEX);
    int wandidx = findWand(wandid);
    if(wandidx > -1){
      flashWand(wandidx);
    }  
}


void showWinner(int win){
  //Serial.println(win);
  if (win==WINNER_LEFT){
    Serial.println("Left Wins!");
  }else if(win==WINNER_RIGHT){
    Serial.println("Right Wins!");    
  }
}

void checkWands(){
    if (irrecvLeft.decode(&resultsLeft)) {
      Serial.println(resultsLeft.value, HEX);
      receiveWand(&resultsLeft);
      if(isValidWand(&resultsLeft)) {
        winner=WINNER_LEFT;
      }
      irrecvLeft.resume(); // Receive the next value
    }

    if (irrecvRight.decode(&resultsRight)) {
      Serial.println(resultsRight.value, HEX);
      receiveWand(&resultsRight);
      if(isValidWand(&resultsRight)) {
        winner=WINNER_RIGHT;
      }
      irrecvRight.resume(); // Receive the next value
    }

}

void red(){
    digitalWrite(10, HIGH);
    delay(500);               // wait for a second
    digitalWrite(10, LOW); 
    delay(500);               // wait for a second
  
}

void green(){
    digitalWrite(9, HIGH);
    delay(500);               // wait for a second
    digitalWrite(9, LOW); 
    delay(500);               // wait for a second 
}

void startGame(){
    red();
    red();
    red();
    green();
    GAME_ON = true;
}

void loop() {
    //Serial.println("h");
    if(GAME_ON){
      checkWands();
      if(winner != WINNER_NONE){
          showWinner(winner);
          winner = WINNER_NONE;
          GAME_ON = false;
          delay(2000);
      }
    }else{
      checkWands();
      if(winner != WINNER_NONE){
          winner = WINNER_NONE;
          startGame();
      }
    }

}
