// ATtiny85 Output
// Entrada e Saída – Dado eletrônico

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

#define CS_PIN  22
#define CLK_PIN 23
#define DIN_PIN 24

#define SERIAL_TX 0
#define SERIAL_RX 1

#define MAX_SEG 1

#define MAXBYTE 255

// 4 tiles, each one is 4 bytes
#define NUM_TILES 4
#define TILE_HEIGHT 8
// https://xantorohara.github.io/led-matrix-editor/#000000000f030303
static const PROGMEM uint8_t tiles[NUM_TILES][TILE_HEIGHT] {
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

byte tileLocations[NUM_TILES] = {
  (2 << 4) + 0, (0 << 4) + 0, (3 << 4) + 0, (6 << 4) + 0
};
// set correct Tile Locations (x = high 4 bits, y = low 4 bits)

byte tilePositions[NUM_TILES];

// set up keypad
const uint8_t NUM_PLUGS NUM_TILES;
const uint8_t NUM_HOLES NUM_TILES;

byte plugs[NUM_PLUGS] = { 30,31,32,33 };
byte holes[NUM_HOLES] = { 34,35,36,37 };

void setup() {
  Serial.begin(9600);
  Serial.println("Press a button to place tile at X offset.");
  Serial.println("(Hold button if it doesn't register)");

  matrix.begin(0x70);  // pass in the address
  
  
  // Limpa o display
  matrix.clear();
  
  for (int i=0; i<NUM_TILES; i++) {
    tilePositions[i] = MAXBYTE;     // null them out
  }

  for (int i=0;i<NUM_PLUGS;
}



void placeTile(byte tile, byte offset_x = 0, byte offset_y = 0) {
  // paste tile into framebuffer (at offset)
  for (byte i = 0; i < TILE_HEIGHT; i++) {
    framebuffer[i + offset_y] ^= pgm_read_byte_near(&tiles[tile][i]) >> offset_x;  // XOR with previous value
  }
}

bool checkWinState() {
  // if the grid is full, you win!
  bool winning = false;
  
  for (byte row=0; row < TILE_HEIGHT; row++) {
    if (framebuffer[row] == 0xFF) {
      winning = true;
    }
    else {
      return (false);
    }
  }
  return (winning);
}

void loop() {
  // scan jumpers
  for (int row=0; row<ROWS; row++) {
    for (int col=0; col<COLS; col++) {
      
    }
  }
  for (int tile=0; tile<NUM_TILES; tile++) {
  char key = keypad.getKey();
  byte tile, pos;
  if (key != NO_KEY) {
    tile = (key >> 4) - 1;
    pos = (key & B1111) - 1;
    Serial.print("Tile,Pos: ");
    Serial.print(tile);
    Serial.print(", ");
    Serial.println(pos);
    Serial.print("Placing Tile: ");
    Serial.println(tile);

    tilePositions[tile] = pos;
    
    matrix.drawBitmap(tileLocations[pos] >> 4, 0, tiles[tile], 8, 8, LED_ON);
    matrix.writeDisplay();
    
    if (checkWinState()) {
      Serial.println(">>>>> You Win!");
    }
  }

  // Verifica o pressionamento do botão
  /*
  if (!digitalRead(BTN_PIN) && !waitDice) {
    waitDice = true; // Trava o botão até o final da rolagem
    Serial.print("Tile: ");
    Serial.println(tile);
    placeTile(tile, tileLocations[tile] >> 4);
    // place randomly
    // int randomXLocation = tileLocations[random(0,4)] >> 4;
    // placeTile(tile, randomXLocation);
    renderFrame();
    waitDice = false;
    ++tile %= NUM_TILES;
  }
  */
}
