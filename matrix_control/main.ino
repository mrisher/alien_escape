/*
Initial sketch for Alien Invasion game powered by Arduino MEGA.
mrisher(at)gmail.com
Requires Bounce2 library for pushbutton debouncing
*/

#include <Bounce2.h>
#include "point.h"

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))
#define MAX_BYTE 255

// Rows & Columns Pins 
const int rows[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int columns[] = {10,11,12,13,14};
const byte NUM_JUMPERS = 4;
const byte jumperSwitches[NUM_JUMPERS][2] = {
  {22,23},
  {24,25},
  {26,27},
  {28,29}
};


// DPad Buttons
const byte NUM_DPAD_BUTTONS = 4;
#define DPAD_UP 0
#define DPAD_DOWN 1
#define DPAD_LEFT 2
#define DPAD_RIGHT 3
// {32, 33, 34, 35};
const byte dPadButtonPins[NUM_DPAD_BUTTONS] = {32, 33, 34, 35};
Bounce dPadButtons[NUM_DPAD_BUTTONS] = {Bounce(), Bounce(), Bounce(), Bounce()};

// jumper success LEDs
const byte jumperRedLed = 30;
const byte jumperGreenLed = 31;
int pause=300;

struct GameState {
  bool jumpersCorrect = false;
  byte height = 0;
};

void ClearMatrix(bool off=true) {
  // set all pins to off (or on)
  for (int i=0; i<ARRAY_SIZE(rows); i++) {
    digitalWrite(rows[i], (off ? LOW : HIGH));      // off = LOW
  }
  for (int j=0; j<ARRAY_SIZE(columns); j++) {
    digitalWrite(columns[j], (off ? HIGH : LOW));   // off = HIGH
  }
}

void setup() {
  Serial.begin(9600); // Any baud rate should work
  Serial.println("Starting setup()");
  
  // set up display matrix
  for (int i=0; i<ARRAY_SIZE(rows); i++) {
    pinMode(rows[i], OUTPUT);
  }
  for (int j=0; j<ARRAY_SIZE(columns); j++) {
    pinMode(columns[j], OUTPUT);
  }
  
  ClearMatrix(false);
  delay (1000);
  
  // zip through one time
  for (int i=0; i<ARRAY_SIZE(rows); i++) {
    digitalWrite(rows[i], HIGH);
    for (int j=0; j<ARRAY_SIZE(columns); j++) {
      digitalWrite(columns[j], LOW);
      delay(25);
      digitalWrite(columns[j], HIGH);
    }
    digitalWrite(rows[i], LOW);
  }
  
  // set up switches
  for (byte i=0; i<NUM_JUMPERS; i++) {
    pinMode(jumperSwitches[i][0], OUTPUT);
    digitalWrite(jumperSwitches[i][0], HIGH);  // hold HIGH until probing to avoid cross-wiring
    pinMode(jumperSwitches[i][1], INPUT_PULLUP);
  }

  // set up DPad Buttons {32, 33, 34, 35};
  for (byte i=0; i<NUM_DPAD_BUTTONS; i++) {
    dPadButtons[i].attach(dPadButtonPins[i], INPUT_PULLUP);
    dPadButtons[i].interval(25);
  }
  
  // set up tiles (helper https://codepen.io/KjeldSchmidt/pen/KaRPzX)
  static const unsigned char PROGMEM tile0[] = {
    B11000000,
    B11000000,
    B11000000,
    B11110000,
    B11110000,
  };

  static const unsigned char PROGMEM tile1[] = {
    B00111000,
    B00111000,
    B00111000,
    B00001000,
    B00001000,
  };

  static const unsigned char PROGMEM tile2[] = {
    B11000000,
    B11000000,
    B11000000,
    B11110000,
    B11110000,
  };

  static const unsigned char PROGMEM tile3[] = {
    B00111000,
    B00111000,
    B00111000,
    B00001000,
    B00001000,
  };

  // display.drawBitmap(0, 0, bitmap_name, bitmap_name_width, bitmap_name_height, WHITE);
}

const byte plugPins[] = {22, 24, 26, 28};
const byte portPins[] = {23, 25, 27, 29};
//unsigned char* tangramBitmaps[];
Point* portLocations[] = {new Point(0,0), new Point(0,3), new Point(3,0), new Point(3,3)};

void loop() {
  static GameState gameState{};

  // For each plug (which defines a tangram) I check each of the ports
  // to see which one is HIGH; that port will define the location as X,Y
  for (byte plug=0; plug < ARRAY_SIZE(plugPins); plug++) {
    digitalWrite(plugPins[plug], HIGH);
    for (byte port=0; port < ARRAY_SIZE(portPins); port++) {
      if (digitalRead(portPins[port]) == HIGH) {
        // TODO: render bitmap[plug] at location[port]
      }
    }
    digitalWrite(plugPins[plug], LOW);
  }

//  gameState.jumpersCorrect = CheckJumperStatus();
/*
  switch (CheckButtons()) {
    case DPAD_UP:
      gameState.height++;
      break;
    case DPAD_DOWN:
      gameState.height--;
      break;
    case MAX_BYTE:
    default:
      break;
  }
  */
}
