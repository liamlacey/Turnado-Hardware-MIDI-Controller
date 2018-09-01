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

const int SLIDER_WIDTH = 20;
const int SLIDER_MAX_SIZE = 127;

void setup()
{
  tft.begin();
  tft.setRotation (3);
  tft.fillScreen (BCKGND_COLOUR);

  //draw a vertical rectangle 'slider' at the bottom of the screen
  tft.fillRect (0, tft.height() - SLIDER_MAX_SIZE, SLIDER_WIDTH, SLIDER_MAX_SIZE, SLIDER_BCKGND_COLOUR);

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
      //increase the size of the slider by drawing the value difference on the top
      tft.fillRect (0, tft.height() - midiCcValue, SLIDER_WIDTH, midiCcValue - prevMidiCcValue, SLIDER_COLOUR);
    }
    else
    {
      //decrease the size of the slider by 'clearing' the value difference from the top
      tft.fillRect (0, tft.height() - prevMidiCcValue, SLIDER_WIDTH, prevMidiCcValue - midiCcValue, SLIDER_BCKGND_COLOUR);
    }

    prevMidiCcValue = midiCcValue;
  }


}

