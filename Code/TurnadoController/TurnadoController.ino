//=========================================================================
int16_t currentMidiProgramNumber = 0;

//=========================================================================
#include "Globals.h"
#include "PinAllocations.h"
#include "Settings.h"

void setKnobControllerBaseValue (uint8_t index, uint8_t value, bool sendToMidiOut);

#include "MidiIO.h"
#include "Lcd.h"
#include "Controls.h"

//=========================================================================
//=========================================================================
//=========================================================================
void setup()
{
  Serial.begin(9600);
  delay(500);

  setupSettings();
  setupLcd();
  setupControls();
  setupMidiIO();
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
