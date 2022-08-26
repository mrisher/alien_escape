/*
Initial sketch for Alien Invasion game powered by Arduino MEGA.
mrisher(at)gmail.com
Requires Bounce2 library for pushbutton debouncing
*/

#define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
   #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
   #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
   #define DPRINT(...)     //now defines a blank line
   #define DPRINTLN(...)   //now defines a blank line
#endif

#include <Bounce2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <YA_FSM.h> // https://github.com/cotestatnt/YA_FSM
#include "tangrams.h"
#include "matrixPulsarFsm.h"
#include "alienNumber.h"

#define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
   #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
   #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
   #define DPRINT(...)     //now defines a blank line
   #define DPRINTLN(...)   //now defines a blank line
#endif

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
Adafruit_AlphaNum4 alphanum = Adafruit_AlphaNum4();

// set up pulsing led for 8x8 ready state
MatrixPulsarFSM matrixPulsarFSM(&matrix);

#define ARRAY_SIZE(array) ((sizeof(array)) / (sizeof(array[0])))
#define NULL_LOCATION 255

// Plugs (tangrams) & Columns (ports) Pins
const byte NUM_JUMPERS = 4;
const byte inputJumperPins[NUM_JUMPERS] = {30, 32, 34, 36};
const byte outputJumperPins[NUM_JUMPERS] = {31, 33, 35, 37};
<<<<<<< HEAD

struct GameState
{
  bool jumpersCorrect = false;
  byte height = 0;
};
=======
>>>>>>> adding_alphanum

void setup()
{
  Serial.begin(115200); // Any baud rate should work
  DPRINTLN("Starting setup()");

  // set up the Matrix
  matrix.begin(0x70); // pass in the address
  matrix.clear();
  DPRINTLN("Cleared matrix");
<<<<<<< HEAD
=======

  DPRINTLN("Setting up quad alphanumeric display");
  //set up the alphanum quad display
  /* alphanum.begin(0x71);  // pass in the address
  alphanum.clear();
  alphanum.writeDigitAscii(0, 'A');
  alphanum.writeDigitAscii(1, 'B');
  alphanum.writeDigitAscii(2, 'C');
  alphanum.writeDigitAscii(3, 'D'); */
  //alphanum.writeDisplay();   // this line crashes the display
  DPRINTLN("Finished setting up quad alphanum");
  

>>>>>>> adding_alphanum

  // zip through matrix as one-time boot animation
  DPRINTLN("Starting 8x8 animation");
  for (int i = 0; i < 8; i++)
  {
    matrix.drawLine(0, i, 7, i, LED_ON);
    matrix.writeDisplay(); // write the changes we just made to the display
    delay(25);
  }
  for (int i = 0; i < 8; i++)
  {
    matrix.drawLine(0, i, 7, i, LED_OFF);
    matrix.writeDisplay(); // write the changes we just made to the display
    delay(25);
  }
  DPRINTLN("Finished 8x8 animation");

  // set up plugs and holes
<<<<<<< HEAD
  DPRINTLN("Setting up pins and plugs");
=======
  DPRINTLN("Setting up tangram plugs and ports");
>>>>>>> adding_alphanum
  for (byte i = 0; i < NUM_JUMPERS; i++)
  {
    pinMode(outputJumperPins[i], OUTPUT);
    digitalWrite(outputJumperPins[i], HIGH); // hold HIGH until probing to avoid cross-wiring
    pinMode(inputJumperPins[i], INPUT_PULLUP);
  }

<<<<<<< HEAD
  DPRINTLN("setup() done");
=======
  DPRINTLN("Done with setup()");
>>>>>>> adding_alphanum
}


void loop()
{
  // For each plug (which defines a tangram) I check each of the ports
  // to see which one is LOW; that port will define the location as X,Y
  bool needsRefresh = false;
  for (byte plug = 0; plug < ARRAY_SIZE(outputJumperPins); plug++)
  {
    digitalWrite(outputJumperPins[plug], LOW);
    byte tile = plug;
    for (byte port = 0; port < ARRAY_SIZE(inputJumperPins); port++)
    {
      if (digitalRead(inputJumperPins[port]) == LOW)
      {
        byte xPosition = portToXYCoordinates[port] >> 4;
        if (tangramPositions[tile] != xPosition)
        {
          // needsRefresh is probably unnecessary (the removal below will always fire first)
          needsRefresh = true;
          tangramPositions[tile] = xPosition;
          DPRINT("Tile "); DPRINT(tile); DPRINT(" is now in pos "); DPRINTLN(xPosition);
        }
        break; // skip to next plug
      }
      else
      {
        if (tangramPositions[tile] != NULL_LOCATION)
        {
          needsRefresh = true;
          tangramPositions[tile] = NULL_LOCATION;
        }
      }
    }
    digitalWrite(outputJumperPins[plug], HIGH);
  }

  // clear board in case something removed
  if (needsRefresh)
    matrix.clear(); // in case something has been removed

  // draw the tangrams in their respective positions
  for (byte tile = 0; tile < NUM_TANGRAMS; tile++)
  {
    if (tangramPositions[tile] != NULL_LOCATION)
    {
      matrix.drawBitmap(tangramPositions[tile], 0, tangrams[tile], 8, 8, LED_ON);
    }
  }

  // Update pulsar State Machine
  if (matrixPulsarFSM.Update())
  {
<<<<<<< HEAD
    DPRINT("New active state: ");
    DPRINTLN(matrixPulsarFSM.ActiveStateName());
=======
    // DPRINT("New active state: ");
    // DPRINTLN(matrixPulsarFSM.ActiveStateName());
>>>>>>> adding_alphanum
  }

  // draw/erase the pulsar pixel
  //matrix.drawPixel(7,7,matrixPulsarFSM.GetLedState());
  matrixPulsarFSM.Paint();

  //update the LED matrix
  matrix.writeDisplay();

/*   if (alienNumber.ReadyForUpdate()) {
    alienNumber.DecValue();
    alienNumber.Paint();
  }
 */
  // check for win?
  bool win = true;
  for (byte tile = 0; tile < NUM_TANGRAMS; tile++)
  {
    if (tangramPositions[tile] != portToXYCoordinates[tile] >> 4)
    {
      win = false;
      break;
    }
  }
  if (win)
  {
    DPRINTLN("WINNER!");
  }
}
