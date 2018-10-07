//=========================================================================
//DEV STUFF...
//#define DISABLE_USB_MIDI 1

//=========================================================================

//FIXME: make the below a global setting
const uint16_t MIDI_CC_LOOPBACK_TIMEOUT = 250;

long prevKnobControllerMidiSendTime[NUM_OF_KNOB_CONTROLLERS] = {0};

//=========================================================================
void ProcessMidiControlChange (byte channel, byte control, byte value);
void sendMidiCcMessage (byte channel, byte control, byte value, int8_t deviceParamIndex);
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
  //If a Turnado knob CC (the only MIDI-in CCs we care about)
  //Would be great if Turnado sent MIDI out for dictator and mix controls. Maybe one day...
  if (control >= 1 && control <= 8)
  {
    //if received outside of a certain time frame since this controller sent out a CC to control the same knob,
    //assume it isn't a looped back MIDI CC (that we want to ignore) and process it.
    if (millis() - prevKnobControllerMidiSendTime[control - 1] > MIDI_CC_LOOPBACK_TIMEOUT)
    {

#ifdef DEBUG
      Serial.print ("MIDI-in CC: ");
      Serial.print (channel);
      Serial.print (" ");
      Serial.print (control);
      Serial.print (" ");
      Serial.println (value);
#endif

      byte knobControllerChan = settingsData[control].paramData[PARAM_INDEX_MIDI_CHAN].value;
      if (knobControllerChan == 0)
        knobControllerChan = settingsData[SETTINGS_GLOBAL].paramData[PARAM_INDEX_MIDI_CHAN].value;

      //if the CC channel matches that of the knob controller channel
      if (channel == knobControllerChan)
      {
        //set knob controller value (base value only)
        setKnobControllerBaseValue (control - 1, value, false);
      }

      //Store the MIDI-in CC value for this channel, so that if changing
      //the channel of a knob controller (either directly or through changing global channel)
      //the base value (and LCD display) for the knob controller is updated with the stored value.
      deviceParamValuesForMidiChannel[channel - 1][control - 1] = value;

    } //if (millis() - prevKnobControllerMidiSendTime[control - 1] > MIDI_CC_LOOPBACK_TIMEOUT)

  } //if (control >= 1 && control <= 8)

}

//=========================================================================
//=========================================================================
//=========================================================================
void sendMidiCcMessage (byte channel, byte control, byte value, int8_t deviceParamIndex)
{
  //FIXME: should I only be sending MIDI-out messages at a certain rate (e.g. only output the latest value for each CC number every x ms?)
  //The output of Turnado/Ableton seems like they do this.

  //if sending a CC for one of the device parameters
  if (deviceParamIndex != -1)
  {
    //if from one of the knob controllers store the time of transmission (for ignoring the possible looped back CC)
    if (deviceParamIndex < DEVICE_PARAM_INDEX_DICTATOR)
      prevKnobControllerMidiSendTime[deviceParamIndex] = millis();

    //store the value being sent out (for handling control MIDI channel switching)
    deviceParamValuesForMidiChannel[channel - 1][deviceParamIndex] = value;
    
  } //if (deviceParamIndex != -1)
  
  usbMIDI.sendControlChange (control, value, channel);
}

//=========================================================================
//=========================================================================
//=========================================================================
void sendMidiProgramChangeMessage (byte channel, byte program)
{
  usbMIDI.sendProgramChange (program, channel);
}
