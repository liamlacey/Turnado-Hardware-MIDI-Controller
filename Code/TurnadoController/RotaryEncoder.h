/*
  RotaryEncoder.h - Class for processing switched rotary encoders,
  built on top of the Teensy Encoder and Bounce classes.

  Created by Liam Lacey, September 2018.
*/

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include "Arduino.h"
#include <Encoder.h>
#include <Bounce.h>

/**
    A Teensy/Arduino class for processing standard switched rotary encoders.
    Features:
    - Processes encoder turns and push switch state changes
    - Provides encoder values as stateless values - +1 for clockwise or -1 for anticlockwise
    - Callback functions for all value changes
    - Switch debouncer
    - Coming soon - encoder acceleration

    To use, simply created instances of the class in your Teensy sketch, assign callback functions
    to the on...() functions, and call the update() function within your loop() function.
*/
class RotaryEncoder
{
    //=====================================================
  public:

    /** Initialises the object to work with a switched rotary encoder.

        @param encPin1 - Encoder pin 1
        @param encPin2 - Encoder pin 2
        @param switchPin - Switch pin. Set to -1 if not using the switch.
    */
    RotaryEncoder (uint8_t encPin1, uint8_t encPin2, int8_t switchPin);
    ~RotaryEncoder();

    /** Reads and updates all control values.

        You must call this function from the loop() function in your sketch.
    */
    void update();

    /** Assigns the function you want to be called when the encoder is turned.

        @param enc - The instance of this class that has detected a change
        @param enc_value - The encoder change, where 1 = clockwise and -1 = anticlockwise
    */

    void onEncoderChange( void (*)(RotaryEncoder &enc, int enc_value) );

    /** Assigns the function you want to be called when the switch is pressed/released.

        @param enc - The instance of this class that has detected a change
    */
    void onSwitchChange( void (*)(RotaryEncoder &enc) );

    /** Returns the current state of the switch (0-1)
    */
    uint8_t getSwitchState();

    /** Compares the memory addresses of instances of the class
    */
    bool operator==(RotaryEncoder& b);

    //=====================================================
  private:

    void (*handle_encoder_change)(RotaryEncoder &enc, int enc_value) = NULL;
    void (*handle_switch_change)(RotaryEncoder &enc) = NULL;

    Encoder *encoder;
    Bounce *switchDebouncer;

    bool switchEnabled = true;
    const int DEBOUNCE_TIME = 5;
    uint8_t switchState = 0;
};

#endif //RotaryEncoder_h
