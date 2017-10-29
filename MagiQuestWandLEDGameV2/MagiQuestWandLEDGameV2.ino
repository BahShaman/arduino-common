#include <IRremote.h>

  int LED_BLUE = 10;
  int LED_RED = 9;
  int LED_YELLOW = 7;
  int LED_GREEN = 13;

  int LED_RIGHT = 10;
  int LED_LEFT = 7; 

  int LEFT = 1;
  int RIGHT = 2;
  int NONE = -1;

  int RECV_PIN_0 = 11;
  int RECV_PIN_1 = 3u;

  #define ENABLE_MagiQuest
  IRrecv irrecvLeft(RECV_PIN_0);
  decode_results resultsLeft;
  IRrecv irrecvRight(RECV_PIN_1);
  decode_results resultsRight;

typedef struct {
   int id;
   String wand_name;
   String wand_color;
   long unsigned int wandid;
   int led_pin;
} Wand;

typedef struct { 
  bool over;
  bool win;
  int winnerWand;
  int winnerSide;
  int p_r;
  int p_l;
} Game;

Game game;

const int NUM_WANDS = 4;
/* 
 * Name and color are just for printing back to the serial  
 * Name, Color, ID, LedPin, last intensity (see struct)
 */
Wand wands[4] = {
  { 0, "Simon"    , "red"    , 0x0001     , LED_RED },
  { 1, "Haylie"   , "green"  , 0x7281     ,  LED_GREEN },
  { 2, "Josiah"   , "blue"   , 0xFFFFB681 , LED_BLUE },
  { 3, "Nehemiah" , "yellow" , 0xFFFFC001 ,  LED_YELLOW },
};

/*
 * setupWands()
 */
void setupWands(){
  for (int i = 0; i < NUM_WANDS; i++) {
    pinMode(wands[i].led_pin, OUTPUT);
  }
}

int findWand(decode_results *results){
  Serial.print("looking for wand ");
  int wandid = results->value;  
  Serial.println(wandid, HEX);
  
  for (int i = 0; i < NUM_WANDS; i++) {
    //Serial.print("looping for wand ");
    if (wands[i].wandid == wandid){
      Serial.print(wands[i].wandid, HEX);
      Serial.println(" found");
      return wands[i].id;
    }
  }
  Serial.println("Wand Not Found");
  return NONE;
}


void findWinner(){
  int wand = NONE;
  if (irrecvLeft.decode(&resultsLeft)) {
    wand = findWand(&resultsLeft);
    game.winnerWand = wand;
    game.winnerSide = LEFT;
    game.win = true;
    irrecvLeft.resume(); // Receive the next value
    return;
  }
  if (irrecvRight.decode(&resultsRight)) {
    wand = findWand(&resultsRight);
    game.winnerWand = wand;
    game.winnerSide = RIGHT;
    game.win = true;
    irrecvLeft.resume(); // Receive the next value
    return;
  }
}

void resetGame(){
  game.over = true;
  game.win = false;
  game.winnerWand = NONE;
  game.winnerSide = NONE;
  game.p_r = NONE;
  game.p_l = NONE;
  digitalWrite(LED_RIGHT, LOW);
  digitalWrite(LED_LEFT, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  delay(5000);
}

void green(){
  static unsigned long previousMillis;
  static int ledState;
  unsigned long interval = 250;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_GREEN, ledState);
  }
}

void red(){
    digitalWrite(LED_RED, HIGH);
    delay(500);               // wait for a second
    digitalWrite(LED_RED, LOW); 
    delay(500);               // wait for a second 
    Serial.println("Ready...");
}

void startGame(){
    Serial.println("Starting Game!");
    digitalWrite(LED_RIGHT, LOW);
    digitalWrite(LED_LEFT, LOW);
    red();
    red();
    red();
    green();
    irrecvRight.resume(); // Receive the next value
    irrecvLeft.resume(); // Receive the next value
    Serial.println("Game on!");
    game.over = false;
}


bool wandsReady(){
  int wand;
  //Serial.println("wandsReady wand[");
  
  if(irrecvLeft.decode(&resultsLeft)){
    Serial.println("Checking Left...");
    wand = findWand(&resultsLeft);
    if(wand > NONE){
      if(game.p_r != wand){
        game.p_l = wand;
        Serial.println("Found Wand Left");
      }
    }
    irrecvLeft.resume(); // Receive the next value
  }
  if(irrecvRight.decode(&resultsRight)) {
    Serial.println("Checking Right...");
    wand = findWand(&resultsRight);
    if(wand > NONE){
      if(game.p_l != wand){
        game.p_r = wand;
        Serial.println("Found Wand Right");
      }
    }
    irrecvRight.resume(); // Receive the next value
  }

  if((game.p_r > NONE) && (game.p_l > NONE) && (game.p_l != game.p_r)){
    //Serial.println("Wands Ready...");
    return true;
  }
  
  return false;
}


void waitingRight(){
  static unsigned long previousMillis;
  static int ledState;
  unsigned long interval = 500;
  unsigned long currentMillis = millis();

  if(game.p_r > NONE){
    digitalWrite(LED_RIGHT, HIGH);
    return;
  }

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_RIGHT, ledState);
  }
}

void waitingLeft(){
  static unsigned long previousMillis;
  static int ledState;
  unsigned long interval = 500;
  unsigned long currentMillis = millis();

  if(game.p_l > NONE){
    digitalWrite(LED_LEFT, HIGH);
    return;
  }
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_LEFT, ledState);
  }
}

void showWinner(){
  int winwand = game.winnerWand;
  int side = NONE;
  if(winwand == game.p_l){
    game.winnerSide = LEFT;
    side = LED_LEFT;
  }
  if(winwand == game.p_r){
    game.winnerSide = RIGHT;
    side = LED_RIGHT;
  }
  for(int i=0;i<6;i++){
    digitalWrite(side, LOW);
    delay(200);
    digitalWrite(side, HIGH);  
    delay(200);
  }
}


void debug(){
  static unsigned long previousMillis;
  unsigned long interval = 750;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("over[");
    Serial.print(game.over);
    Serial.print("] win[");
    Serial.print(game.win);
    Serial.print("] winwand[");
    Serial.print(game.winnerWand);
    Serial.print("] winside[");
    Serial.print(game.winnerSide);
    Serial.print("] pr[");
    Serial.print(game.p_r);
    Serial.print("] pl[");
    Serial.print(game.p_l);
    Serial.print("] sam[");
    Serial.print((game.p_l == game.p_r));
    Serial.print("] rdy[");
    Serial.print((game.p_r > NONE) && (game.p_l > NONE) && (game.p_l != game.p_r));
    Serial.println("]");
  }
}


void setup(){
  irrecvRight.enableIRIn(); // Start the receiver
  irrecvLeft.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  Serial.println("starting...");
  setupWands();
  resetGame();
}

void loop(){
  debug();
  if(game.win){
    showWinner();
    resetGame();
  }else if(!game.over){
    green();
    findWinner();
  }else{
    waitingRight();
    waitingLeft();
    if(wandsReady()){
      startGame();
    }
  }
  //wandsReady();
}
