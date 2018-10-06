#include "Globals.h"

//=========================================================================
int16_t currentMidiProgramNumber = 0;
//FIXME: could the below be replaced by knobControllerData and mixControllerData as arrays for each MIDI channel?
uint8_t deviceParamValuesForMidiChannel[16][NUM_OF_DEVICE_PARAMS] = {{0}};

//=========================================================================
#include "PinAllocations.h"
#include "Settings.h"

void setKnobControllerBaseValue (uint8_t index, uint8_t value, bool sendToMidiOut);
void setMixControllerValue (uint8_t value, bool sendToMidiOut);

#include "MidiIO.h"
#include "Lcd.h"
#include "Controls.h"

//=========================================================================
//=========================================================================
//=========================================================================
void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
  delay(500);
#endif

  setupSettings();
  setupLcd();
  setupControls();
  setupMidiIO();

  for (uint8_t chan = 0; chan < 16; chan++)
  {
    //assume mix value always starts at 127, and the rest start at 0.
    deviceParamValuesForMidiChannel[chan][DEVICE_PARAM_INDEX_MIX] = 127;
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void loop()
{
  updateMidiIO();
  updateControls();
  updateLcd();
  settingsUpdateEeprom();
}
