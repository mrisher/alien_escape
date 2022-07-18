#ifndef D976A36F_9AB8_424E_A104_8E50E495F99D
#define D976A36F_9AB8_424E_A104_8E50E495F99D

#include <YA_FSM.h>

#define BLINK_TIME 1000

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
        void Setup();
        uint16_t GetLedState();
    private:
        bool matrixPulsarLedState = false;
};

#endif /* D976A36F_9AB8_424E_A104_8E50E495F99D */
