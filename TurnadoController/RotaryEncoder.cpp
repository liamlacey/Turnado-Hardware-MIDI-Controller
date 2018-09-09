#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder (uint8_t encPin1, uint8_t encPin2, uint8_t switchPin)
{
  encoder = new Encoder (encPin1, encPin2);

  switchDebouncer = new Bounce (switchPin, DEBOUNCE_TIME);
  pinMode (switchPin, INPUT_PULLUP);
}

RotaryEncoder::~RotaryEncoder()
{
  delete encoder;
  delete switchDebouncer;
}

void RotaryEncoder::update()
{
  //Check for encoder turn
  //TODO: implement encoder acceleration

  int env_val = encoder->read();

  if (env_val >= 4 || env_val <= - 4)
  {
    env_val /= 4;
    this->handle_encoder_change (*this, env_val);
    encoder->write(0);
  }

  //Check for switch state change
  switchDebouncer->update();

  if (switchDebouncer->risingEdge())
  {
    switchState = 0;
    this->handle_switch_change (*this, switchState);
  }
  else if (switchDebouncer->fallingEdge())
  {
    switchState = 1;
    this->handle_switch_change (*this, switchState);
  }
}

uint8_t RotaryEncoder::getSwitchState()
{
  return switchState;
}

void RotaryEncoder::onEncoderChange( void (*function)(RotaryEncoder&, int) )
{
  this->handle_encoder_change = function;
}

void RotaryEncoder::onSwitchChange( void (*function)(RotaryEncoder&, uint8_t) )
{
  this->handle_switch_change = function;
}

bool RotaryEncoder::operator==(RotaryEncoder& b)
{
  return (this == &b);
}

