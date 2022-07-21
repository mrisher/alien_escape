#include "matrixPulsarFsm.h"


void MatrixPulsarFSM::Setup() {
  // set up the FSM for pulsing
  // Follow the order of defined enumeration for the state definition (will be used as index)
  // Add States  => name, 		timeout, onEnter callback, onState cb, 	  onLeave cb
  this->AddState(matrixPulsarStateName[PULSAR_ON], BLINK_TIME, nullptr, nullptr, nullptr);
  this->AddState(matrixPulsarStateName[PULSAR_OFF], BLINK_TIME, nullptr, nullptr, nullptr);
  // Add transitions with related trigger input callback functions
  // In this example it's just a simple lambda function that return state timeout value
  AddTransition(PULSAR_ON, PULSAR_OFF, []()
                                { return YA_FSM::CurrentState()->timeout; });
  this->AddTransition(PULSAR_OFF, PULSAR_ON, [&]()
                                { return CurrentState()->timeout; });
  this->AddAction(PULSAR_ON, YA_FSM::S, matrixPulsarLedState);    // S = set to on
  this->AddAction(PULSAR_OFF, YA_FSM::R, matrixPulsarLedState);   // R = reset to false
    
}

uint16_t MatrixPulsarFSM::GetLedState() {
    return matrixPulsarLedState;    
}
