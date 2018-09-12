#include "PinAllocations.h"
#include "RotaryEncoder.h"
#include "SwitchControl.h"
#include "ThumbJoystick.h"

#include "ILI9341_t3.h"

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

//For LCD use hardware SPI (#13, #12, #11) and the custom allocated for CS/DC
ILI9341_t3 lcd = ILI9341_t3 (PIN_LCD_CS, PIN_LCD_DC);

const int LCD_FRAME_RATE = 30;
long lcdPreviousMillis = 0;
const int LCD_COLOUR_BCKGND = ILI9341_BLACK;

//=========================================================================
//=========================================================================
//=========================================================================
void setup()
{
  Serial.begin(9600);
  delay(500);

  lcd.begin();
  lcd.setRotation (3);
  lcd.fillScreen (LCD_COLOUR_BCKGND);

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

  usbMIDI.setHandleControlChange(ProcessMidiControlChange);

}

//=========================================================================
//=========================================================================
//=========================================================================
void loop()
{
  //Read from USB MIDI-in
  usbMIDI.read();

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

  //update the LCD display at the set frame rate
  if ((millis() - lcdPreviousMillis) > (1000.0 / (float)LCD_FRAME_RATE))
  {
    updateLcd();
    lcdPreviousMillis = millis();
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void updateLcd()
{
  //Do LCD drawing here...
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

//=========================================================================
//=========================================================================
//=========================================================================
void ProcessMidiControlChange (byte channel, byte control, byte value)
{
  Serial.print ("MIDI-in CC: ");
  Serial.print (channel);
  Serial.print (" ");
  Serial.print (control);
  Serial.print (" ");
  Serial.println (value);
}

