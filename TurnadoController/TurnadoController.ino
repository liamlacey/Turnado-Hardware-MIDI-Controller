#include "PinAllocations.h"
#include "Controls.h"
#include "Lcd.h"

//=========================================================================
//=========================================================================
//=========================================================================
void setup()
{
  Serial.begin(9600);
  delay(500);

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

