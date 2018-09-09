#ifndef EncoderSwitched_h
#define EncoderSwitched_h

#include <Encoder.h>
#include <Bounce.h>

class EncoderSwitched
{
    //=====================================================
  public:

    EncoderSwitched (uint8_t encPin1, uint8_t encPin2, uint8_t switchPin);
    ~EncoderSwitched();

    void update();

    void onEncoderChange( void (*)(EncoderSwitched &enc, int enc_value) );
    void onSwitchChange( void (*)(EncoderSwitched &enc, uint8_t switch_value) );

    uint8_t getSwitchState();

    bool operator==(EncoderSwitched& b)
    {
      return (this == &b);
    }

    //=====================================================
  private:

    void (*handle_encoder_change)(EncoderSwitched &enc, int enc_value) = NULL;
    void (*handle_switch_change)(EncoderSwitched &enc, uint8_t switch_value) = NULL;

    Encoder *encoder;
    Bounce *switchDebouncer;

    const int DEBOUNCE_TIME = 5;
    uint8_t switchState = 0;
};

#endif //EncoderSwitched_h
