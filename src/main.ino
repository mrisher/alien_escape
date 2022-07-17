/*
Initial sketch for Alien Invasion game powered by Arduino MEGA.
mrisher(at)gmail.com
Requires Bounce2 library for pushbutton debouncing
*/

#include <Bounce2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <YA_FSM.h>
#include "tangrams.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// set up pulsing led for 8x8 ready state
YA_FSM matrixPulsarFSM;
#define BLINK_TIME 500
// State Alias 
enum matrixPulsarState {PULSAR_ON, PULSAR_OFF};
// Helper for print labels instead integer when state change
const char * const matrixPulsarStateName[] PROGMEM = { "Pulsar On", "Pulsar Off"};

// display the appropriate LED value
void onEnterMatrixOnState() {
  Serial.println("onEnterMatrixOnState()");
  matrix.drawPixel(7, 7, LED_ON);
  matrix.writeDisplay();
}
void onEnterMatrixOffState() {
  Serial.println("onEnterMatrixOffState()");
  matrix.drawPixel(7, 7, LED_OFF);
  matrix.writeDisplay();
}


#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))
#define NULL_LOCATION 255

// Rows & Columns Pins 
const byte NUM_JUMPERS = 4;
const byte outputJumperPins[NUM_JUMPERS] = {30,32,34,36};
const byte inputJumperPins[NUM_JUMPERS] = {31,33,35,37};

struct GameState {
  bool jumpersCorrect = false;
  byte height = 0;
};


void setup() {
  Serial.begin(115200); // Any baud rate should work
  Serial.println("Starting setup()");

// set up the Matrix
  matrix.begin(0x70);  // pass in the address
  matrix.clear();

  // set up the FSM for pulsing
  // Follow the order of defined enumeration for the state definition (will be used as index)
  //Add States  => name, 		timeout, onEnter callback, onState cb, 	  onLeave cb	
  matrixPulsarFSM.AddState(matrixPulsarStateName[PULSAR_ON], BLINK_TIME, onEnterMatrixOnState, nullptr, nullptr);
  matrixPulsarFSM.AddState(matrixPulsarStateName[PULSAR_OFF], BLINK_TIME, onEnterMatrixOffState, nullptr, nullptr);
  // Add transitions with related trigger input callback functions
  // In this example it's just a simple lambda function that return state timeout value
  matrixPulsarFSM.AddTransition(PULSAR_ON, PULSAR_OFF, [](){return matrixPulsarFSM.CurrentState()->timeout;} );    
  matrixPulsarFSM.AddTransition(PULSAR_OFF, PULSAR_ON, [](){return matrixPulsarFSM.CurrentState()->timeout;} );    


  // zip through matrix as one-time boot animation
  for (int i=0; i<8; i++) {
    matrix.drawLine(0,i,7,i,LED_ON);
    matrix.writeDisplay();  // write the changes we just made to the display
    delay(25);
  }
  for (int i=0; i<8; i++) {
    matrix.drawLine(0,i,7,i,LED_OFF);
    matrix.writeDisplay();  // write the changes we just made to the display
    delay(25);
  }
  
  // set up plugs and holes
  for (byte i=0; i<NUM_JUMPERS; i++) {
    pinMode(outputJumperPins[i], OUTPUT);
    digitalWrite(outputJumperPins[i], HIGH);  // hold HIGH until probing to avoid cross-wiring
    pinMode(inputJumperPins[i], INPUT_PULLUP);
  }
}

void loop() {
  static GameState gameState{};

  // Update State Machine
  if(matrixPulsarFSM.Update()){
    Serial.print(F("New active state: "));
    Serial.println(matrixPulsarFSM.ActiveStateName());
  }

  // For each plug (which defines a tangram) I check each of the ports
  // to see which one is HIGH; that port will define the location as X,Y
  for (byte plug=0; plug < ARRAY_SIZE(outputJumperPins); plug++) {
    digitalWrite(outputJumperPins[plug], LOW);
    byte tile = plug;
    for (byte port=0; port < ARRAY_SIZE(inputJumperPins); port++) {
      if (digitalRead(inputJumperPins[port]) == LOW) {
        byte position = portLocations[port] >> 4;
        tangramPositions[tile] = position;
        /*
        Serial.print("Placing tile ");
        Serial.print(tile);
        Serial.print(" at position ");
        Serial.println(position);
        */
        break;   // skip to next plug   
      }
      else
        tangramPositions[tile] = NULL_LOCATION;
    }
    digitalWrite(outputJumperPins[plug], HIGH);
  }

  // draw the tangrams in their respective positions
  matrix.clear();
  for (byte tile=0; tile<NUM_TANGRAMS; tile++) {
    if (tangramPositions[tile] != NULL_LOCATION) {
      matrix.drawBitmap(tangramPositions[tile], 0, tangrams[tile], 8, 8, LED_ON);
    }
  }
  matrix.writeDisplay();  

  // check for win?
  bool win = true;
  for (byte tile=0; tile<NUM_TANGRAMS; tile++) {
    if (tangramPositions[tile] != portLocations[tile] >> 4) {
      win = false;
      break;
    }
  }
  if (win) {
    Serial.println("WINNER!");
  }
}
