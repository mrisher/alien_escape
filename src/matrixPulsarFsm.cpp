#include "matrixPulsarFsm.h"


void MatrixPulsarFSM::Setup() {
  // set up the FSM for pulsing
  // Follow the order of defined enumeration for the state definition (will be used as index)
  // Add States  => name, 		timeout, onEnter callback, onState cb, 	  onLeave cb
  this->AddState(matrixPulsarStateName[PULSAR_ON], BLINK_TIME, nullptr, nullptr, nullptr);
  this->AddState(matrixPulsarStateName[PULSAR_OFF], BLINK_TIME, nullptr, nullptr, nullptr);
  // Add transitions with related time-based trigger
  this->AddTimedTransition(PULSAR_ON, PULSAR_OFF);
  this->AddTimedTransition(PULSAR_OFF, PULSAR_ON);
  this->AddAction(PULSAR_ON, YA_FSM::S, matrixPulsarLedState);    // S = set to on
  this->AddAction(PULSAR_OFF, YA_FSM::R, matrixPulsarLedState);   // R = reset to false
    
}

uint16_t MatrixPulsarFSM::GetLedState() {
    return matrixPulsarLedState;    
}
