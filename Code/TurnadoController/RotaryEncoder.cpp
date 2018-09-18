#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder (uint8_t encPin1, uint8_t encPin2, int8_t switchPin)
{
  encoder = new Encoder (encPin1, encPin2);

  if (switchPin >= 0)
  {
    switchDebouncer = new Bounce (switchPin, DEBOUNCE_TIME);
    pinMode (switchPin, INPUT_PULLUP);
  }
  else
  {
    switchEnabled = false;
  }
}

RotaryEncoder::~RotaryEncoder()
{
  delete encoder;

  if (switchEnabled)
    delete switchDebouncer;
}

void RotaryEncoder::update()
{
  //Check for encoder turn

  int env_val = encoder->read();

  //If there is an encoder value change
  if (env_val >= 4 || env_val <= - 4)
  {
    if (accelerationEnabled)
    {
      //get time interval between this change and previous change
      currentTime = millis();
      revolutionTime = currentTime - prevTime;
      prevTime = currentTime;

      // trigger acceleration only when encoder speed is sufficiently high (small time value)
      if (revolutionTime < revTimeThreshold)
      {
        rev = revTimeThreshold - revolutionTime;
        b = (int)a * rev + c; //slope of a line equation
        env_val = env_val * b; //apply acceleration
      }
      
    } //if (enableAcceleration)

    env_val /= 4;
    this->handle_encoder_change (*this, env_val);
    encoder->write(0);
    
  } //if (env_val >= 4 || env_val <= - 4)

  if (switchEnabled)
  {
    //Check for switch state change
    switchDebouncer->update();

    if (switchDebouncer->risingEdge())
    {
      switchState = 0;
      this->handle_switch_change (*this);
    }
    else if (switchDebouncer->fallingEdge())
    {
      switchState = 1;
      this->handle_switch_change (*this);
    }

  }//if (switchEnabled)

}

uint8_t RotaryEncoder::getSwitchState()
{
  return switchState;
}

void RotaryEncoder::onEncoderChange( void (*function)(RotaryEncoder&, int) )
{
  this->handle_encoder_change = function;
}

void RotaryEncoder::onSwitchChange( void (*function)(RotaryEncoder&) )
{
  this->handle_switch_change = function;
}

bool RotaryEncoder::operator==(RotaryEncoder& b)
{
  return (this == &b);
}

void RotaryEncoder::enableAcceleration (bool shouldEnable)
{
  accelerationEnabled = shouldEnable;
}

