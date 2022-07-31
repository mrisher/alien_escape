#ifndef DD84705F_8451_4C30_B30B_677F1A582B1C
#define DD84705F_8451_4C30_B30B_677F1A582B1C

#include <Arduino.h>
#include <Adafruit_LEDBackpack.h>


// 14 Segment Display Animation Tool
// https://editor.p5js.org/gbose/sketches/1ljDhENv 
// https://gaspariscoding.github.io/Alphanumeric_Animation_Utility/
const uint16_t digitBitmasks[] = {
    //0b1100000000000000,     // dot
    0B1000000011000000,     // bar = 1
    0b1001001000000000,     // line = 2
    0b1010100000001000,     // triangle = 3
    0b1000000011011100      // square = 4
};

class AlienNumber {
    const int UpdateCycleInMillis = 1000;
    
    public:
        AlienNumber(Adafruit_AlphaNum4 *alphanum, int startVal = 0);
        void Paint();
        int DecValue(int difference=1);
        int IncValue(int increment=1);
        bool ReadyForUpdate();
    private:
        int value;
        void Setup();
        unsigned long lastUpdate;
        Adafruit_AlphaNum4* display;
};
#endif /* DD84705F_8451_4C30_B30B_677F1A582B1C */
