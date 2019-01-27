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

    } //if (accelerationEnabled)

    env_val /= 4;

    //constrain value to expected range to get rid of occasional incorrect large encoder readings
    env_val = constrain (env_val, -4, 4);

    if (shouldSendEncoderValue (env_val))
    {
      this->handle_encoder_change (*this, env_val);
    }

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

bool RotaryEncoder::shouldSendEncoderValue (int incVal)
{
  //Only send the encoder value if it's the first turn after a certain time period or if
  //the direction is the same as with the previous x amount of turns.
  //This is done to prevent stray sets of positive values within fast negative turns from being processed.

  bool shouldSend = false;

  //if the first encoder turn after a certain time period
  if (millis() - prevIncTime >= 50)
  {
    shouldSend = true;

    //set all prevIncDirs to match incVal to reset process of checking previous values
    for (uint8_t i = 0; i < NUM_OF_PREV_VALS; i++)
      incVal > 0 ? prevIncDirs[i] = 1 : prevIncDirs[i] = -1;
  }

  //else if the value direction matches that of the previous set of values
  else if (doesValDirMatchPrevVals (incVal))
  {
    shouldSend = true;

    //move all values in prevIncDirs backward (removing end/oldest value)
    //and add new value based on incVal at the beginning

    for (uint8_t i = NUM_OF_PREV_VALS - 1; i > 0; i--)
      prevIncDirs[i] = prevIncDirs[i - 1];

    incVal > 0 ? prevIncDirs[0] = 1 : prevIncDirs[0] = -1;
  }

  prevIncTime = millis();
  return shouldSend;
}

bool RotaryEncoder::doesValDirMatchPrevVals (int incVal)
{
  int8_t valDir = incVal > 0 ? 1 : -1;

  for (uint8_t i = 0; i < NUM_OF_PREV_VALS; i++)
  {
    if (valDir != prevIncDirs[i])
      return false;
  }

  return true;
}
