#ifndef D976A36F_9AB8_424E_A104_8E50E495F99D
#define D976A36F_9AB8_424E_A104_8E50E495F99D

#include <YA_FSM.h>
#include <Arduino.h>
#include <Adafruit_LEDBackpack.h>

const int BLINK_TIME=1000;

// State Alias
enum matrixPulsarState
{
  PULSAR_ON,
  PULSAR_OFF
};
// Helper for print labels instead integer when state change
const char *const matrixPulsarStateName[] PROGMEM = {"Pulsar On", "Pulsar Off"};

class MatrixPulsarFSM : public YA_FSM {
    public:
        MatrixPulsarFSM(Adafruit_8x8matrix* m, int x=7, int y=7);
        void Paint();
    private:
        bool matrixPulsarLedState = false;
        int pulsarX;    // coordinates
        int pulsarY;
        Adafruit_8x8matrix* matrix;
        void Setup();
};

#endif /* D976A36F_9AB8_424E_A104_8E50E495F99D */
