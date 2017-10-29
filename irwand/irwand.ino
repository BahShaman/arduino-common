#include <ArduinoJson.h>
#include <IRremote.h>
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

int receiver = 11;    // Pin connected to receiver

IRrecv irrecv(receiver);
decode_results results;

void setup(){
  irrecv.enableIRIn(); // Start up receiver
  Serial.begin(9600);
  Serial.println("{\"message-type\":\"initialize\",\"message\":\"starting...\"}");
  //Serial.println('starting...');

  #ifdef ENABLE_MagiQuest
    else if (results->decode_type == MAGIQUEST) {
    #ifndef IS_AVTINY
      root["message-type"] = "wand";
      //Serial.print("Decoded MAGIQUEST - Magnitude=");
      //Serial.print(results->magiquestMagnitude, HEX);
      //Serial.print(", wand_id=");
    #endif 
  #endif 

  #ifdef ENABLE_RCMM
    else if (results->decode_type == RCMM) {
       Serial.print("Decoded RCMM: ");
    }
    root["owner"] = "Undefined";
    root["message"] = "";
    if(results->value==0x10CF2401){
      root["owner"] = "Ellie";
    }
    if(results->value==0x1B9BA421){
      root["owner"] = "Edison";
    }
    if(results->value==0x11911A81){
      root["owner"] = "Dad";
    }
    root["wand-id"] = String(results->value, HEX);
    root["bits"] = String(results->bits, DEC);
    root["magnitude"] = results->magiquestMagnitude;
    root.printTo(Serial);
    /*Serial.print("Raw (");
    Serial.print(count, DEC);
    Serial.print("): ");
  
    for (int i = 0; i < count; i++) {
      if ((i % 2) == 1) {
        Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
      } 
      else {
        Serial.print(-(long)results->rawbuf[i]*USECPERTICK, DEC);
      }
      Serial.print(" ");
    }*/
    Serial.println("");
  #endif //ENABLE_RCMM
}

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    dump(&results);
    delay(250);
    irrecv.resume(); // Receive the next value
  }
}
