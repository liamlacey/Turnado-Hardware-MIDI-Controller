//=========================================================================
//DEV STUFF...
#define DEBUG 1
//#define DISABLE_USB_MIDI 1

//=========================================================================
void ProcessMidiControlChange (byte channel, byte control, byte value);

//=========================================================================
//=========================================================================
//=========================================================================
void setupMidiIO()
{
#ifndef DISABLE_USB_MIDI
  usbMIDI.setHandleControlChange(ProcessMidiControlChange);
#endif
}

//=========================================================================
//=========================================================================
//=========================================================================
void updateMidiIO()
{
#ifndef DISABLE_USB_MIDI
  //Read from USB MIDI-in
  usbMIDI.read();
#endif
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
