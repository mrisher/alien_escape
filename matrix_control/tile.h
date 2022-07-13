#ifndef BEF185D8_B6A5_462E_BEF0_C008981EA910
#define BEF185D8_B6A5_462E_BEF0_C008981EA910

#include <Arduino.h>

#define TILE_BITMAP_DIMENSION 5

class Tile {
  public:
    Tile();
    void SetBits (byte row, byte bits);
    void SetBitstring (byte bitstrings[]);
    void Print (byte row);
  private:
    byte _bitmap[TILE_BITMAP_DIMENSION];
};


#endif /* BEF185D8_B6A5_462E_BEF0_C008981EA910 */
