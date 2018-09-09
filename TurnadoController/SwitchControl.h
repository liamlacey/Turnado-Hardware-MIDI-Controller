/*
  SwitchControl.h - Class for processing binary switches/buttons,
  built on top of the Teensy Bounce class.

  Created by Liam Lacey, September 2018.
*/

#ifndef SwitchControl_h
#define SwitchControl_h

#include "Arduino.h"
#include <Bounce.h>

/**
    A Teensy/Arduino class for processing standard binary switches/buttons.

    To use, simply created instances of the class in your Teensy sketch, assign a callback function
    to the on...() function, and call the update() function within your loop() function.
*/
class SwitchControl
{
  public:

    /** Initialises the object to work with a switch or button.

          @param buttonPin - Button pin
    */
    SwitchControl (uint8_t switchPin);
    ~SwitchControl();

    /** Reads and updates the switch state.

        You must call this function from the loop() function in your sketch.
    */
    void update();

    /** Assigns the function you want to be called when the switch state changes.

         @param SwitchControl - The instance of this class that has detected a change
    */
    void onSwitchStateChange( void (*)(SwitchControl &switchControl) );

    /** Returns the current state of the switch, where 1 = on/pressed and 0 = off/released
    */
    uint8_t getSwitchState();

    /** Compares the memory addresses of instances of the class
    */
    bool operator==(SwitchControl& b);

  private:

    void (*handle_switch_state_change)(SwitchControl &switchControl) = NULL;

    Bounce *switchDebouncer;

    const int DEBOUNCE_TIME = 5;
    uint8_t switchState = 0;
};

#endif //SwitchControl_h
