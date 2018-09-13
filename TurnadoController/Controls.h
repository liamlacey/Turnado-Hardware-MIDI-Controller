#include "RotaryEncoder.h"
#include "SwitchControl.h"
#include "ThumbJoystick.h"

//=========================================================================
//DEV STUFF...
#define DISABLE_JOYSTICKS 1

//=========================================================================
RotaryEncoder* knobControllersEncoders[NUM_OF_KNOB_CONTROLLERS];
ThumbJoystick* knobControllersJoysticks[NUM_OF_KNOB_CONTROLLERS];

RotaryEncoder* dictatorEncoder;
ThumbJoystick* dictatorJoystick;

RotaryEncoder* mixEncoder;

RotaryEncoder* lcdEncoders[NUM_OF_LCD_ENCS];

SwitchControl* presetUpButton;
SwitchControl* presetDownButton;

SwitchControl* randomiseButton;

//=========================================================================
void processEncoderChange (RotaryEncoder &enc, int enc_value);
void processEncoderSwitchChange (RotaryEncoder &enc);
void processPushButtonChange (SwitchControl &switchControl);
void processJoystickChange (ThumbJoystick &thumbJoystick, bool isYAxis);

//=========================================================================
//=========================================================================
//=========================================================================
void setupControls()
{
  for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
  {
    knobControllersEncoders[i] = new RotaryEncoder (PINS_KNOB_CTRL_ENCS[i].pinA, PINS_KNOB_CTRL_ENCS[i].pinB, PINS_KNOB_CTRL_ENCS[i].pinSwitch);
    knobControllersEncoders[i]->onEncoderChange (processEncoderChange);
    knobControllersEncoders[i]->onSwitchChange (processEncoderSwitchChange);

    knobControllersJoysticks[i] = new ThumbJoystick (PINS_KNOB_CTRL_JOYSTICKS[i]);
    knobControllersJoysticks[i]->onJoystickChange (processJoystickChange);
  }

  dictatorEncoder = new RotaryEncoder (PINS_DICTATOR_ENC.pinA, PINS_DICTATOR_ENC.pinB, PINS_DICTATOR_ENC.pinSwitch);
  dictatorEncoder->onEncoderChange (processEncoderChange);
  dictatorEncoder->onSwitchChange (processEncoderSwitchChange);

  dictatorJoystick = new ThumbJoystick (PIN_DICTATOR_JOYSTICK);
  dictatorJoystick->onJoystickChange (processJoystickChange);

  mixEncoder = new RotaryEncoder (PINS_MIX_ENC.pinA, PINS_MIX_ENC.pinB, PINS_MIX_ENC.pinSwitch);
  mixEncoder->onEncoderChange (processEncoderChange);

  for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)
  {
    lcdEncoders[i] = new RotaryEncoder (PINS_LCD_ENCS[i].pinA, PINS_LCD_ENCS[i].pinB, PINS_LCD_ENCS[i].pinSwitch);
    lcdEncoders[i]->onEncoderChange (processEncoderChange);
    lcdEncoders[i]->onSwitchChange (processEncoderSwitchChange);
  }

  presetUpButton = new SwitchControl (PIN_PRESET_UP_BUTTON);
  presetUpButton->onSwitchStateChange (processPushButtonChange);
  presetDownButton = new SwitchControl (PIN_PRESET_DOWN_BUTTON);
  presetDownButton->onSwitchStateChange (processPushButtonChange);

  randomiseButton = new SwitchControl (PIN_RANDOMISE_BUTTON);
  randomiseButton->onSwitchStateChange (processPushButtonChange);
}

//=========================================================================
//=========================================================================
//=========================================================================
void updateControls()
{
  for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
  {
    knobControllersEncoders[i]->update();

#ifndef DISABLE_JOYSTICKS
    knobControllersJoysticks[i]->update();
#endif
  }

  dictatorEncoder->update();

#ifndef DISABLE_JOYSTICKS
  dictatorJoystick->update();
#endif

  mixEncoder->update();

  for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)
  {
    lcdEncoders[i]->update();
  }

  presetUpButton->update();
  presetDownButton->update();
  randomiseButton->update();
}

//=========================================================================
//=========================================================================
//=========================================================================
void processEncoderChange (RotaryEncoder &enc, int enc_value)
{
  for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
  {
    if (enc == *knobControllersEncoders[i])
    {
#ifdef DEBUG
      Serial.print ("Knob Controller ");
      Serial.print (i + 1);
      Serial.print (" encoder: ");
      Serial.println (enc_value);
#endif
    }

  } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

  for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)
  {
    if (enc == *lcdEncoders[i])
    {
#ifdef DEBUG
      Serial.print ("LCD encoder ");
      Serial.print (i + 1);
      Serial.print (": ");
      Serial.println (enc_value);
#endif
    }

  } //for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)

  if (enc == *dictatorEncoder)
  {
#ifdef DEBUG
    Serial.print ("Dictator encoder: ");
    Serial.println (enc_value);
#endif
  }

  else if (enc == *mixEncoder)
  {
#ifdef DEBUG
    Serial.print ("Mix encoder: ");
    Serial.println (enc_value);
#endif
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void processEncoderSwitchChange (RotaryEncoder &enc)
{
  for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
  {
    if (enc == *knobControllersEncoders[i])
    {
#ifdef DEBUG
      Serial.print ("Knob Controller ");
      Serial.print (i + 1);
      Serial.print (" encoder switch: ");
      Serial.println (enc.getSwitchState());
#endif
    }

  } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

  for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)
  {
    if (enc == *lcdEncoders[i])
    {
#ifdef DEBUG
      Serial.print ("Lcd encoder ");
      Serial.print (i + 1);
      Serial.print (" switch: ");
      Serial.println (enc.getSwitchState());
#endif
    }

  } //for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)

  if (enc == *dictatorEncoder)
  {
#ifdef DEBUG
    Serial.print ("Dictator encoder switch: ");
    Serial.println (enc.getSwitchState());
#endif
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void processPushButtonChange (SwitchControl &switchControl)
{
  if (switchControl == *presetUpButton)
  {
#ifdef DEBUG
    Serial.print ("Preset Up Button: ");
    Serial.println (switchControl.getSwitchState());
#endif
  }

  else if (switchControl == *presetDownButton)
  {
#ifdef DEBUG
    Serial.print ("Preset Down Button: ");
    Serial.println (switchControl.getSwitchState());
#endif
  }

  else if (switchControl == *randomiseButton)
  {
#ifdef DEBUG
    Serial.print ("Randomise Button: ");
    Serial.println (switchControl.getSwitchState());
#endif
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void processJoystickChange (ThumbJoystick &thumbJoystick, bool isYAxis)
{
  if (isYAxis)
  {
    for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
    {
      if (thumbJoystick == *knobControllersJoysticks[i])
      {
#ifdef DEBUG
        Serial.print ("Knob Controller ");
        Serial.print (i + 1);
        Serial.print (" joystick: ");
        Serial.println (thumbJoystick.getYAxisValue());
#endif
      }

    } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

    if (thumbJoystick == *dictatorJoystick)
    {
#ifdef DEBUG
      Serial.print ("Dictator Joystick: ");
      Serial.println (thumbJoystick.getYAxisValue());
#endif
    }

  } //if (isYAxis)
}
