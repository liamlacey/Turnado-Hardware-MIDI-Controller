//=========================================================================
//DEV STUFF...
//#define DISABLE_USB_MIDI 1

//=========================================================================

const uint8_t MIDI_CC_LOOPBACK_TIMEOUT = 50;

long prevKnobControllerMidiSendTime[NUM_OF_KNOB_CONTROLLERS] = {0};

//=========================================================================
void ProcessMidiControlChange (byte channel, byte control, byte value);
void sendMidiCcMessage (byte channel, byte control, byte value, int8_t knobControllerIndex);
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
  //if recieved a Turnado Knob CC (CC1-8) within a certain time frame since
  //this controller sent out a CC to control the same knob, assume it is
  //the looped back MIDI CC and ignore it.
  if ((control >= 1 && control <= 8) && 
  (millis() - prevKnobControllerMidiSendTime[control - 1] < MIDI_CC_LOOPBACK_TIMEOUT))
    return;

#ifdef DEBUG
  Serial.print ("MIDI-in CC: ");
  Serial.print (channel);
  Serial.print (" ");
  Serial.print (control);
  Serial.print (" ");
  Serial.println (value);
#endif

if (control >= 1 && control <= 8)
{
  setKnobControllerBaseValue (control - 1, value, false);
}

  //TODO: process MIDI-in from Turnado knobs, adjusting the internal knob controller MIDI values (and LCD display) accordingly.
}

//=========================================================================
//=========================================================================
//=========================================================================
void sendMidiCcMessage (byte channel, byte control, byte value, int8_t knobControllerIndex)
{
  //FIXME: should I only be sending MIDI-out messages at a certain rate (e.g. only output the latest value for each CC number every x ms?)
  //The output of Turnado/Ableton seems like they do this.

  //if sending a CC for an actual knob controller, store the time of transmission (for ignoring the possible looped back CC)
  if (knobControllerIndex != -1 && knobControllerIndex != DICTATOR_KNOB_CONTROLLER_INDEX)
    prevKnobControllerMidiSendTime[knobControllerIndex] = millis();

  usbMIDI.sendControlChange (control, value, channel);
}

//=========================================================================
//=========================================================================
//=========================================================================
void sendMidiProgramChangeMessage (byte channel, byte program)
{
  usbMIDI.sendProgramChange (program, channel);
}
