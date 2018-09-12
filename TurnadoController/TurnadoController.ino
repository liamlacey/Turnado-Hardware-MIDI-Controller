#include "RotaryEncoder.h"
#include "SwitchControl.h"
#include "ThumbJoystick.h"
#include "PinAllocations.h"

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
//=========================================================================
//=========================================================================
void setup()
{
  Serial.begin(9600);
  delay(500);

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
  presetDownButton = new SwitchControl (PIN_PRESET_DOWN_BUTTON);

  randomiseButton = new SwitchControl (PIN_RANDOMISE_BUTTON);
  randomiseButton->onSwitchStateChange (processPushButtonChange);

}

//=========================================================================
//=========================================================================
//=========================================================================
void loop()
{
  for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
  {
    knobControllersEncoders[i]->update();
    knobControllersJoysticks[i]->update();
  }

  dictatorEncoder->update();
  dictatorJoystick->update();
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
      Serial.print ("Knob Controller ");
      Serial.print (i + 1);
      Serial.print (" encoder: ");
      Serial.println (enc_value);
    }

  } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

  for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)
  {
    if (enc == *lcdEncoders[i])
    {
      Serial.print ("LCD encoder ");
      Serial.print (i + 1);
      Serial.print (": ");
      Serial.println (enc_value);
    }

  } //for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)

  if (enc == *dictatorEncoder)
  {
    Serial.print ("Dictator encoder: ");
    Serial.println (enc_value);
  }

  else if (enc == *mixEncoder)
  {
    Serial.print ("Mix encoder: ");
    Serial.println (enc_value);
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
      Serial.print ("Knob Controller ");
      Serial.print (i + 1);
      Serial.print (" encoder switch: ");
      Serial.println (enc.getSwitchState());
    }

  } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

  for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)
  {
    if (enc == *lcdEncoders[i])
    {
      Serial.print ("Lcd encoder ");
      Serial.print (i + 1);
      Serial.print (" switch: ");
      Serial.println (enc.getSwitchState());
    }

  } //for (auto i = 0; i < NUM_OF_LCD_ENCS; i++)

  if (enc == *dictatorEncoder)
  {
    Serial.print ("Dictator encoder: ");
    Serial.println (enc.getSwitchState());
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void processPushButtonChange (SwitchControl &switchControl)
{
  if (switchControl == *presetUpButton)
  {
    Serial.print ("Preset Up Button: ");
    Serial.println (switchControl.getSwitchState());
  }

  else if (switchControl == *presetDownButton)
  {
    Serial.print ("Preset Down Button: ");
    Serial.println (switchControl.getSwitchState());
  }

  else if (switchControl == *randomiseButton)
  {
    Serial.print ("Randomise Button: ");
    Serial.println (switchControl.getSwitchState());
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
        Serial.print ("Knob Controller ");
        Serial.print (i + 1);
        Serial.print (" joystick: ");
        Serial.println (thumbJoystick.getYAxisValue());
      }

    } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

    if (thumbJoystick == *dictatorJoystick)
    {
      Serial.print ("Dictator Joystick: ");
      Serial.println (thumbJoystick.getYAxisValue());
    }

  } //if (isYAxis)
}
