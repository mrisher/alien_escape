#ifndef B537F56B_AA3F_4CC3_A4E7_9A5C9E4A20D9
#define B537F56B_AA3F_4CC3_A4E7_9A5C9E4A20D9

#include <Arduino.h>

class Point {
    public:
        Point();
        Point(uint16_t x, uint16_t y);
        uint16_t GetX();
        uint16_t GetY();
        void SetX (uint16_t x);
        void SetY (uint16_t y);
    private:
        uint16_t _x;
        uint16_t _y;
};

#endif /* B537F56B_AA3F_4CC3_A4E7_9A5C9E4A20D9 */
