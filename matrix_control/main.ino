/*
Initial sketch for Alien Invasion game powered by Arduino MEGA.
mrisher(at)gmail.com
Requires Bounce2 library for pushbutton debouncing
*/

#include <Bounce2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Fsm.h>

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))
#define NULL_LOCATION 255

// Rows & Columns Pins 
const byte NUM_JUMPERS = 4;
const byte outputJumperPins[NUM_JUMPERS] = {30,32,34,36};
const byte inputJumperPins[NUM_JUMPERS] = {31,33,35,37};

// 4 tangrams, each one is 4 bytes
#define NUM_TANGRAMS 4
#define TANGRAM_HEIGHT 8
// https://xantorohara.github.io/led-matrix-editor/#000000000f030303
static const PROGMEM uint8_t tangrams[NUM_TANGRAMS][TANGRAM_HEIGHT] {
  {
    B10000000,
    B10000000,
    B11100000,
    B01100000,
    B11100000,
    B10000000,
    B10000000,
    B10000000
  },
  {
    B11000000,
    B11000000,
    B11000000,
    B11100000,
    B11000000,
    B11000000,
    B11000000,
    B11000000
  },
  {
    B11110000,
    B11110000,
    B00100000,
    B00100000,
    B00100000,
    B11100000,
    B11100000,
    B11100000
  },
  {
    B01000000,
    B01000000,
    B11000000,
    B11000000,
    B11000000,
    B11000000,
    B11000000,
    B11000000
  }
};

// set correct Tile Locations (x = high 4 bits, y = low 4 bits)
byte portLocations[NUM_TANGRAMS] = {
  (2 << 4) + 0, (0 << 4) + 0, (3 << 4) + 0, (6 << 4) + 0
};

byte tilePositions[NUM_TANGRAMS];

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

  // For each plug (which defines a tangram) I check each of the ports
  // to see which one is HIGH; that port will define the location as X,Y
  for (byte plug=0; plug < ARRAY_SIZE(outputJumperPins); plug++) {
    digitalWrite(outputJumperPins[plug], LOW);
    byte tile = plug;
    for (byte port=0; port < ARRAY_SIZE(inputJumperPins); port++) {
      if (digitalRead(inputJumperPins[port]) == LOW) {
        byte position = portLocations[port] >> 4;
        tilePositions[tile] = position;
        /*
        Serial.print("Placing tile ");
        Serial.print(tile);
        Serial.print(" at position ");
        Serial.println(position);
        */
        break;   // skip to next plug   
      }
      else
        tilePositions[tile] = NULL_LOCATION;
    }
    digitalWrite(outputJumperPins[plug], HIGH);
  }

  // draw the tangrams in their respective positions
  matrix.clear();
  for (byte tile=0; tile<NUM_TANGRAMS; tile++) {
    if (tilePositions[tile] != NULL_LOCATION) {
      matrix.drawBitmap(tilePositions[tile], 0, tangrams[tile], 8, 8, LED_ON);
    }
  }
  matrix.writeDisplay();  

  // check for win?
  bool win = true;
  for (byte tile=0; tile<NUM_TANGRAMS; tile++) {
    if (tilePositions[tile] != portLocations[tile] >> 4) {
      win = false;
      break;
    }
  }
  if (win) {
    Serial.println("WINNER!");
  }
}
