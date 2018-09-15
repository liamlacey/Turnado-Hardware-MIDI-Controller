//=========================================================================
//DEV STUFF...
#define DEBUG 1
//#define DISABLE_USB_MIDI 1

//=========================================================================
#include "Globals.h"
#include "PinAllocations.h"
#include "Settings.h"
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

#ifndef DISABLE_USB_MIDI
  usbMIDI.setHandleControlChange(ProcessMidiControlChange);
#endif

}

//=========================================================================
//=========================================================================
//=========================================================================
void loop()
{

#ifndef DISABLE_USB_MIDI
  //Read from USB MIDI-in
  usbMIDI.read();
#endif

  updateControls();
  updateLcd();

  settingsUpdateEeprom();
}

//=========================================================================
//=========================================================================
//=========================================================================
void ProcessMidiControlChange (byte channel, byte control, byte value)
{
#ifdef DEBUG
  Serial.print ("MIDI-in CC: ");
  Serial.print (channel);
  Serial.print (" ");
  Serial.print (control);
  Serial.print (" ");
  Serial.println (value);
#endif
}

