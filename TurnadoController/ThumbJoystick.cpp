#include "ThumbJoystick.h"

ThumbJoystick::ThumbJoystick (uint8_t yAxisPin_)
{
  yAxisPin = yAxisPin_;
}

ThumbJoystick::~ThumbJoystick()
{

}

void ThumbJoystick::update()
{
  uint16_t value = analogRead (yAxisPin);

  //Create a plateau around the centre point.
  if ((value > 512 - (JS_CENTRE_PLATEAU_VAL / 2)) &&
      (value < 512 + (JS_CENTRE_PLATEAU_VAL / 2)))
  {
    value = 512;
  }

  //if we've got a new Y-axis raw value within a range of +/-hysteresis_val
  if ((value - JS_Y_HYSTERESIS_VAL > yAxisRawValue) ||
      (value + JS_Y_HYSTERESIS_VAL < yAxisRawValue))
  {
    yAxisRawValue = value;
    
    //map and contrain raw value to user value of +/-127 with plateay values at each end
    value = map (value, JS_EDGE_PLATEAU_VAL, 1023 - JS_EDGE_PLATEAU_VAL, - 127, 127);
    value = constrain (value, -127, 127);

    if (value != yAxisUserValue)
    {
      yAxisUserValue = value;
      this->handle_joystick_change (*this, true);
    }
    
  } //if (new Y axis value)
}

void ThumbJoystick::onJoystickChange( void (*function)(ThumbJoystick &thumbJoystick, bool isYAxis) )
{
  this->handle_joystick_change = function;
}

int16_t ThumbJoystick::getYAxisValue()
{
  return yAxisUserValue;
}

bool ThumbJoystick::operator==(ThumbJoystick& t)
{
  return (this == &t);
}

