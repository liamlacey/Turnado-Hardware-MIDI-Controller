#include "RotaryEncoder.h"
#include "SwitchControl.h"
#include "ThumbJoystick.h"

//=========================================================================
//DEV STUFF...
#define DISABLE_JOYSTICKS 1

//=========================================================================
RotaryEncoder* knobControllersEncoders[NUM_OF_KNOB_CONTROLLERS];
ThumbJoystick* knobControllersJoysticks[NUM_OF_KNOB_CONTROLLERS];

RotaryEncoder* mixEncoder;

RotaryEncoder* lcdEncoders[NUM_OF_LCD_ENCS];

SwitchControl* presetUpButton;
SwitchControl* presetDownButton;

SwitchControl* randomiseButton;

//=========================================================================
struct KnobControllerData
{
  int16_t baseValue = 0;
  uint8_t prevBaseValue = 0;
  int16_t relativeValue = 0;
  int8_t prevRelativeValue = 0;
  int16_t combinedMidiValue = 0;
  uint8_t prevCombinedMidiValue = 0;
};

KnobControllerData knobControllerData[NUM_OF_KNOB_CONTROLLERS];

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
void setKnobControllerCombinedMidiValue (uint8_t index)
{
  if (knobControllerData[index].relativeValue > 0)
    knobControllerData[index].combinedMidiValue = map (knobControllerData[index].relativeValue, 0, 127, knobControllerData[index].baseValue, 127);
  else if (knobControllerData[index].relativeValue < 0)
    knobControllerData[index].combinedMidiValue = map (knobControllerData[index].relativeValue, 0, -128, knobControllerData[index].baseValue, 0);

  if (knobControllerData[index].combinedMidiValue != knobControllerData[index].prevCombinedMidiValue)
  {
    //send MIDI message
    byte channel = settingsData[index + 1].paramData[PARAM_INDEX_MIDI_CHAN].value;
    byte control = settingsData[index + 1].paramData[PARAM_INDEX_CC_NUM].value;
    byte value = knobControllerData[index].combinedMidiValue;
    sendMidiCcMessage (channel, control, value);

    //TODO: update LCD display

    knobControllerData[index].prevCombinedMidiValue = knobControllerData[index].combinedMidiValue;
  }
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

      knobControllerData[i].baseValue = constrain (knobControllerData[i].baseValue + enc_value, 0, 127);

      if (knobControllerData[i].baseValue != knobControllerData[i].prevBaseValue)
      {
        setKnobControllerCombinedMidiValue(i);
        knobControllerData[i].prevBaseValue = knobControllerData[i].baseValue;
      }

    } //if (enc == *knobControllersEncoders[i])

  } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

  if (enc == *mixEncoder)
  {
#ifdef DEBUG
    Serial.print ("Mix encoder: ");
    Serial.println (enc_value);
#endif
  }

  else if (enc == *lcdEncoders[LCD_ENC_CTRL])
  {
#ifdef DEBUG
    Serial.print ("LCD CTRL encoder: ");
    Serial.println (enc_value);
#endif

    lcdSetSelectedMenu (enc_value);
  }

  else if (enc == *lcdEncoders[LCD_ENC_PARAM])
  {
#ifdef DEBUG
    Serial.print ("LCD Param encoder: ");
    Serial.println (enc_value);
#endif

    lcdSetSelectedParam (enc_value);
  }

  else if (enc == *lcdEncoders[LCD_ENC_VAL])
  {
#ifdef DEBUG
    Serial.print ("LCD Value encoder: ");
    Serial.println (enc_value);
#endif

    lcdSetSelectedParamValue (enc_value);
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

  if (enc == *lcdEncoders[LCD_ENC_CTRL])
  {
#ifdef DEBUG
    Serial.print ("LCD CTRL encoder swich: ");
    Serial.println (enc.getSwitchState());
#endif

    //TODO: toggle LCD display mode
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

        knobControllerData[i].relativeValue = thumbJoystick.getYAxisValue();

        if (knobControllerData[i].relativeValue != knobControllerData[i].prevRelativeValue)
        {
          setKnobControllerCombinedMidiValue(i);
          knobControllerData[i].prevRelativeValue = knobControllerData[i].relativeValue;
        }

      } //if (thumbJoystick == *knobControllersJoysticks[i])

    } //for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)

  } //if (isYAxis)
}
