#include "point.h"

Point::Point() {
    _x = 0;
    _y = 0;
}

Point::Point(uint16_t x, uint16_t y) {
    _x = x;
    _y = y;
}

uint16_t Point::GetX() {
    return _x;
}

uint16_t Point::GetY() {
    return _y;
}

void Point::SetX(uint16_t x) {
    _x = x;
}

void Point::SetY(uint16_t y) {
    _y = y;
}

