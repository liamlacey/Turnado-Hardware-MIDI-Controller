//#include "SPI.h"
#include "ILI9341_t3.h"

// TFT pins
const uint8_t TFT_DC = 9;
const uint8_t TFT_CS = 10;

// Use hardware SPI (#13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3 (TFT_CS, TFT_DC);

int midiCcValue = 0;
int prevMidiCcValue = 0;

const int BCKGND_COLOUR = ILI9341_BLACK;
const int SLIDER_COLOUR = ILI9341_RED;
const int SLIDER_BCKGND_COLOUR = ILI9341_DARKGREY;

void setup()
{
  tft.begin();
  tft.setRotation (3);
  tft.fillScreen (BCKGND_COLOUR);
  
  tft.fillRect (0, 0, 127 * 2, 20, SLIDER_BCKGND_COLOUR);
  
  //tft.fillCircle (50, 50, 20, SLIDER_COLOUR);

  usbMIDI.setHandleControlChange(ProcessMidiControlChange);

}

void loop()
{
  //Read from USB MIDI-in
  usbMIDI.read();
}

void ProcessMidiControlChange (byte channel, byte control, byte value)
{
  midiCcValue = value;

  if (midiCcValue != prevMidiCcValue)
  {
    if (midiCcValue > prevMidiCcValue)
    {
      tft.fillRect (prevMidiCcValue * 2, 0, (midiCcValue - prevMidiCcValue) * 2, 20, SLIDER_COLOUR);
    }
    else
    {
      tft.fillRect (midiCcValue * 2, 0, (prevMidiCcValue - midiCcValue) * 2, 20, SLIDER_BCKGND_COLOUR);
    }

    prevMidiCcValue = midiCcValue;
  }


}

