#include "tile.h"

void Tile::SetBitstring (byte bitstrings[]) {
  for (int i=0; i<TILE_BITMAP_DIMENSION; i++) {
    _bitmap[i] = bitstrings[i];
  }
}

void Tile::Print(byte row) {
  byte bitstring = _bitmap[row];
  digitalWrite(rows[row], HIGH);
  for (byte i=0; i < TILE_BITMAP_DIMENSION; i++) {
    //Serial.print(bitstring & (1 << (i-1)) ? '+' : '_');
    int pinState = (bitstring & (1 << (TILE_BITMAP_DIMENSION - i - 1)) ? LOW : HIGH);   // columns are cathode, so LOW = on
    digitalWrite(columns[i], pinState);
  }
  //Serial.println();   // newline
}
void Tile::SetBits(byte row, byte bits) {
  _bitmap[row] = bits;
}
