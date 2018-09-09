/*
  ThumbJoystick.h - Class for processing thumb joysticks.
  Current just handles 1 axis and no switch.

  Created by Liam Lacey, September 2018.
*/

#ifndef ThumbJoystick_h
#define ThumbJoystick_h

#include "Arduino.h"

/**
    A Teensy/Arduino class for processing thumb joystick.
    Currently only supports 1 axis and no switch.

    To use, simply created instances of the class in your Teensy sketch, assign a callback function
    to the on...() function, and call the update() function within your loop() function.
*/
class ThumbJoystick
{
  public:
    ThumbJoystick (uint8_t yAxisPin);
    ~ThumbJoystick();

    void update();

    void onJoystickChange( void (*)(ThumbJoystick &thumbJoystick, bool isYAxis) );

    int16_t getYAxisValue();

    bool operator==(ThumbJoystick& t);

  private:

    void (*handle_joystick_change)(ThumbJoystick &thumbJoystick, bool isYAxis) = NULL;

    //Joystick hysteresis value.
    //Used to prevent the analogue values bouncing, but reduces resolution.
    //This could also be considered as a 'sensitivity' value.
    const int JS_Y_HYSTERESIS_VAL = 3;
    //Joystick centre plateau value.
    //Increase to add more dead space around the centre if joystick isn't centring.
    const int JS_CENTRE_PLATEAU_VAL = 50;
    //Joystick edge plateau value.
    //Increase to add more dead space at the edge if joystick isn't reaching end values.
    const int JS_EDGE_PLATEAU_VAL = 4;

    uint8_t yAxisPin;
    uint16_t yAxisRawValue = 511;
    int16_t yAxisUserValue = 0; 
};

#endif //ThumbJoystick_h


