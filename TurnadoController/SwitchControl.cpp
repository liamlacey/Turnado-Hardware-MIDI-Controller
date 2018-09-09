#include "SwitchControl.h"

SwitchControl::SwitchControl (uint8_t switchPin)
{
  switchDebouncer = new Bounce (switchPin, DEBOUNCE_TIME);
  pinMode (switchPin, INPUT_PULLUP);
}

SwitchControl::~SwitchControl()
{
  delete switchDebouncer;
}

void SwitchControl::update()
{
  switchDebouncer->update();

  if (switchDebouncer->risingEdge())
  {
    switchState = 0;
    this->handle_switch_state_change (*this);
  }
  else if (switchDebouncer->fallingEdge())
  {
    switchState = 1;
    this->handle_switch_state_change (*this);
  }
}

void SwitchControl::onSwitchStateChange( void (*function)(SwitchControl&) )
{
  this->handle_switch_state_change = function;
}

uint8_t SwitchControl::getSwitchState()
{
  return switchState;
}

bool SwitchControl::operator==(SwitchControl& b)
{
  return (this == &b);
}

