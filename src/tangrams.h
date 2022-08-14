#ifndef D4F27D7D_1436_423D_B814_0517E7280F4E
#define D4F27D7D_1436_423D_B814_0517E7280F4E

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
    B10000000
  }
};

// set correct Tile Locations (x = high 4 bits, y = low 4 bits)
// so tangram 1 belongs in port 2, 2->0, 3->3, 4->1
byte portToXYCoordinates[NUM_TANGRAMS] = {
  (2 << 4) + 0, (0 << 4) + 0, (3 << 4) + 0, (6 << 4) + 0
};

byte tangramPositions[NUM_TANGRAMS];

#endif /* D4F27D7D_1436_423D_B814_0517E7280F4E */
