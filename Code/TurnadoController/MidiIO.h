//=========================================================================
//DEV STUFF...
//#define DISABLE_USB_MIDI 1

//=========================================================================


//=========================================================================
void ProcessMidiControlChange (byte channel, byte control, byte value);
void sendMidiCcMessage (byte channel, byte control, byte value);
void sendMidiProgramChangeMessage (byte channel, byte program);

//=========================================================================
//=========================================================================
//=========================================================================
void setupMidiIO()
{
#ifndef DISABLE_USB_MIDI
  usbMIDI.setHandleControlChange (ProcessMidiControlChange);
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

  //TODO: process MIDI-in from Turnado knobs, adjusting the internal knob controller MIDI values (and LCD display) accordingly.
}

//=========================================================================
//=========================================================================
//=========================================================================
void sendMidiCcMessage (byte channel, byte control, byte value)
{
  usbMIDI.sendControlChange (control, value, channel);
}

//=========================================================================
//=========================================================================
//=========================================================================
void sendMidiProgramChangeMessage (byte channel, byte program)
{
  usbMIDI.sendProgramChange (program, channel);
}
