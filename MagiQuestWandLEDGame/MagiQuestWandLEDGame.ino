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
  int PLAYER_LEFT = 1;
  int PLAYER_RGHT = 2;
  int WAND_LEFT = 1;
  int WAND_RGHT = 2;
  int PIN_LEFT = 7;
  int PIN_RGHT = 10;
  bool READY_LEFT = false;
  bool READY_RGHT = false;
  int WAND_NONE = 0;
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
   int player;
   bool isReady;
   bool isPlayer;
   bool isWinner;
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
    resetWands();
  }
}

void resetWands(){
  for (int i = 0; i < NUM_WANDS; i++) {
    wands[i].isReady = false;
    wands[i].isPlayer = false;
    wands[i].isWinner = false;
  }
}

void flashWands(){
  for (int i = 0; i < NUM_WANDS; i++) {
    flashWand(wands[i]);
  }
}

void flashWand(Wand wand){
  Serial.print("Flashing ");
  Serial.print(wand.wand_name);
  Serial.print("'s wand (");
  Serial.print(wand.wand_color);
  Serial.println(")");
  for (int i = 0; i < 2; i++) {
    digitalWrite(wand.led_pin, HIGH);
    delay(125);               // wait for a second
    digitalWrite(wand.led_pin, LOW); 
    delay(125);               // wait for a second
  }
}

void setup()
{
  irrecvRight.enableIRIn(); // Start the receiver
  irrecvLeft.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  Serial.println("starting...");

  setupWands();
  flashWands();
  
}



Wand findWand(decode_results *results){
  Serial.print("looking for wand ");
  int wandid = results->value;  
  Serial.println(wandid, HEX);
  
  for (int i = 0; i < NUM_WANDS; i++) {
    //Serial.print("looping for wand ");
    if (wands[i].wand_id == wandid){
      Serial.print(wands[i].wand_id, HEX);
      Serial.println(" found");
      return wands[i];
    }
    Serial.println(" ");
  }
  Serial.println("Wand Not Found");
}

void receiveWand(decode_results *results){
  int count = results->rawlen;

  if (results->decode_type == UNKNOWN) {
    Serial.println("Unknown encoding: ");
  } 
  #ifdef ENABLE_MagiQuest
  else if (results->decode_type == MAGIQUEST) {
      //Serial.print("Decoded MAGIQUEST - Magnitude=");
      //Serial.print(results->magiquestMagnitude, HEX);
      //Serial.print(", wand_id=");
  #endif
  }
}

void RightWinner(){
      Serial.println("Right Wins!");
}
void LeftWinner(){
      Serial.println("Left Wins!");
}


void RightReady(){
      Serial.println("Right Ready!");
}
void LeftReady(){
      Serial.println("Left Ready!");
}

void showWinner(int win){
  for (int i = 0; i < NUM_WANDS; i++) {
    if(wands[i].isWinner){
      if(wands[i].player == PLAYER_RGHT){
        RightWinner();
      } else if (wands[i].player == PLAYER_LEFT)
        LeftWinner();    
      }
   }
}

void printWand(Wand wand){
  Serial.print("Flashing ");
  Serial.print(wand.wand_name);
  Serial.print("'s wand (");
  Serial.print(wand.wand_color);
  Serial.print("ready (");
  Serial.print(wand.isReady);
  Serial.print(") player (");
  Serial.print(wand.player);
  Serial.println(")");

}

Wand receiveWands(){
    Wand wand;
    if (irrecvLeft.decode(&resultsLeft)) {
      Serial.println(resultsLeft.value, HEX);
      receiveWand(&resultsLeft);
      wand = findWand(&resultsLeft);
      wand.player=PLAYER_LEFT;
      irrecvLeft.resume(); // Receive the next value
    }
    //could be a case where righr always wins
    if (irrecvRight.decode(&resultsRight)) {
      Serial.println(resultsRight.value, HEX);
      receiveWand(&resultsRight);
      wand = findWand(&resultsRight);
      wand.player=PLAYER_RGHT;
      irrecvRight.resume(); // Receive the next value
    }
    return wand;
}

void green(){
    digitalWrite(13, HIGH);
    delay(500);               // wait for a second
    digitalWrite(13, LOW); 
    delay(500);           // wait for a second
    Serial.println("Go!");

}

void red(){
    digitalWrite(9, HIGH);
    delay(500);               // wait for a second
    digitalWrite(9, LOW); 
    delay(500);               // wait for a second 
    Serial.println("Ready...");
}

void startGame(){
    Serial.println("Starting Game!");
    red();
    red();
    red();
    green();
    GAME_ON = true;
}

/*
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
*/


bool checkWandsWinner(){
  Serial.println("Checking for Winner");
  Wand wand = receiveWands();
  return false;
  
}

bool checkWandsReady(){
  //Serial.println("Checking for Ready...");
  Wand wand = receiveWands();
//were looking for 2 wands ready and hopefully 1 right and 1 left
  for (int i = 0; i < NUM_WANDS; i++) {
    if(wands[i].wand_id == wand.wand_id){
        if(wands[i].player > 0){
          wands[i].isReady = true;
          wands[i].isPlayer = true;
        }
    }
    if(wands[i].isPlayer){
      if(wands[i].player == PLAYER_RGHT){
        RightReady();
      } else if (wands[i].player == PLAYER_LEFT)
        LeftReady();    
      }
   }  
   return false;
}
void bogus(){  
  Wand wand = receiveWands();
  int SIDE;
    if(SIDE == WAND_RGHT){
      READY_RGHT = true;
      digitalWrite(PIN_RGHT, HIGH);
      return;
    }
    if(SIDE == WAND_LEFT){
      READY_LEFT = true;
      digitalWrite(PIN_LEFT, HIGH);
      return;
    }
    return;
}

void resetWandsReady(){
      READY_RGHT = true;
      digitalWrite(PIN_RGHT, LOW);
      READY_LEFT = true;
      digitalWrite(PIN_LEFT, LOW);
 
}



void loop(){
/* 
 * if right wand is valid and ready 
 * indicate ready  
 * and left wand is valid and ready 
 * indicate ready
 * inidcate start of game
 * do the countdown
 * give go signal
 * check for input
 * first wand wins
 */

  if(GAME_ON == true){
      checkWandsWinner();
      GAME_ON = false;
  }else{
    if(checkWandsReady()){
      delay(500);
      resetWandsReady();
      delay(500);
      startGame();
    }
  }
}

