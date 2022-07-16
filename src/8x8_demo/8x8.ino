// ATtiny85 Output
// Entrada e Saída – Dado eletrônico

#include <LedControl.h>
#include <SoftwareSerial.h>
#include <Keypad.h>

#define F_CPU 16500000L

#define CS_PIN  22
#define CLK_PIN 23
#define DIN_PIN 24
#define BTN_PIN 25
#define RND_PIN A0

#define SERIAL_TX 0
#define SERIAL_RX 1

#define MAX_SEG 1

// set up keypad
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
char keys[ROWS][COLS] = {
  { 0x11, 0x12, 0x13, 0x14},
  { 0x21, 0x22, 0x23, 0x24 },
  { 0x31, 0x32, 0x33, 0x34 },
  { 0x41, 0x42, 0x43, 0x44 }
};

uint8_t colPins[4] = { 34,35,36,37 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[4] = { 30,31,32,33 }; // Pins connected to R1, R2, R3, R4
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// 4 tiles, each one is 4 bytes
#define NUM_TILES 4
#define TILE_HEIGHT 8
// https://xantorohara.github.io/led-matrix-editor/#000000000f030303
uint8_t tiles[NUM_TILES][TILE_HEIGHT] {
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

uint8_t framebuffer[8];

// Define os pinos da serial
SoftwareSerial mySerial(SERIAL_RX, SERIAL_TX);

// Define os pinos da matriz de LEDs
LedControl mat = LedControl(DIN_PIN, CLK_PIN, CS_PIN, MAX_SEG); // MAX7219

bool waitDice = false;
uint8_t numberDice = 0;
int tile;

void setup() {
  mySerial.begin(9600);
  mySerial.println("Press a button to place tile at X offset.");
  mySerial.println("(Hold button if it doesn't register)");

  pinMode(BTN_PIN, INPUT_PULLUP);

  mat.shutdown(0, false);

  // Para o random não repetir a sequência
  randomSeed(analogRead(RND_PIN));

  // Ajusta o brilho da matriz de LEDs para intensidade média
  mat.setIntensity(0, 7);

  // Limpa o display
  mat.clearDisplay(0);
  // clear the framebuffer
  for (int i=0; i<8; i++) {
    framebuffer[i] = 0x0;
  }

  tile=0;
}




void renderFrame() {
  for (byte i=0; i<8; i++) {
    mat.setRow(0, i, framebuffer[i]);
  }
}

void placeTile(byte tile, byte offset_x = 0, byte offset_y = 0) {
  // paste tile into framebuffer (at offset)
  for (byte i = 0; i < TILE_HEIGHT; i++) {
    framebuffer[i + offset_y] ^= tiles[tile][i] >> offset_x;  // XOR with previous value
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
  char key = keypad.getKey();
  byte tile, pos;
  if (key != NO_KEY) {
    tile = (key >> 4) - 1;
    pos = (key & B1111) - 1;
    mySerial.print("Tile,Pos: ");
    mySerial.print(tile);
    mySerial.print(", ");
    mySerial.println(pos);

    waitDice = true; // Trava o botão até o final da rolagem
    mySerial.print("Placing Tile: ");
    mySerial.println(tile);
    placeTile(tile, tileLocations[pos] >> 4);
    renderFrame();
    if (checkWinState()) {
      mySerial.println(">>>>> You Win!");
    }
    waitDice = false;
  }

  // Verifica o pressionamento do botão
  /*
  if (!digitalRead(BTN_PIN) && !waitDice) {
    waitDice = true; // Trava o botão até o final da rolagem
    mySerial.print("Tile: ");
    mySerial.println(tile);
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
