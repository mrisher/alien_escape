#include "alienNumber.h"

#define MAXVAL 256+64+16+4
    // in base 4, the positions are [256 64 16 4]
const int positions[4] = {256, 64, 16, 4};

AlienNumber::AlienNumber(Adafruit_AlphaNum4 *alphanum, int startVal) {
    display = alphanum;
    value = 0;
    if (startVal > 0)
        value = IncValue(startVal);       // handle overflow in a central place
    lastUpdate = 0;

    this->Setup();
}

void AlienNumber::Setup() {

  
}

void AlienNumber::Paint() {
    int output[4];

    int tempNum = value;
    for (int i=3; i>=0; i--) {
        output[i] = tempNum % positions[i];
        tempNum = tempNum / positions[i];
        display->writeDigitRaw(i, digitBitmasks[output[i]]);
    }

    display->writeDisplay();
    lastUpdate = millis();
}

int AlienNumber::IncValue(int increment) {
    value+=increment;
    if (value > MAXVAL)
        value = 0;
    
    return (value);    
}

int AlienNumber::DecValue(int difference) {
    value-=difference;
    if (value < 0)
        value = MAXVAL;
    
    return (value);    
}

bool AlienNumber::ReadyForUpdate() {
    return (millis() - lastUpdate >= UpdateCycleInMillis);
}
