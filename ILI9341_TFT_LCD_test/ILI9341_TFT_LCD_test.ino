//#include "SPI.h"
#include "ILI9341_t3.h"

// TFT pins
const uint8_t TFT_DC = 9;
const uint8_t TFT_CS = 10;

// Use hardware SPI (#13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3 (TFT_CS, TFT_DC);

const int BCKGND_COLOUR = ILI9341_BLACK;
const int SLIDER_COLOUR = ILI9341_RED;
const int SLIDER_BCKGND_COLOUR = ILI9341_DARKGREY;

const int SLIDER_WIDTH = 20;
const int SLIDER_MAX_SIZE = 127;
const int SLIDER_SPACING = 40;

const uint8_t NUM_OF_SLIDERS = 8;
const uint8_t SLIDER_CC_NUMS[NUM_OF_SLIDERS] =  {2, 3, 4, 5, 6, 8, 9, 12};

int midiCcValue[NUM_OF_SLIDERS] = {0};
int prevMidiCcValue[NUM_OF_SLIDERS] = {0};

void setup()
{
  tft.begin();
  tft.setRotation (3);
  tft.fillScreen (BCKGND_COLOUR);

  //draw a vertical rectangle 'slider' at the bottom of the screen
  for (uint8_t i = 0; i < NUM_OF_SLIDERS; i++)
    tft.fillRect (i * SLIDER_SPACING, tft.height() - SLIDER_MAX_SIZE, SLIDER_WIDTH, SLIDER_MAX_SIZE, SLIDER_BCKGND_COLOUR);

  usbMIDI.setHandleControlChange(ProcessMidiControlChange);

}

void loop()
{
  //Read from USB MIDI-in
  usbMIDI.read();
}

void ProcessMidiControlChange (byte channel, byte control, byte value)
{
  for (uint8_t i = 0; i < NUM_OF_SLIDERS; i++)
  {
    if (control == SLIDER_CC_NUMS[i])
    {
      midiCcValue[i] = value;

      if (midiCcValue[i] != prevMidiCcValue[i])
      {
        if (midiCcValue[i] > prevMidiCcValue[i])
        {
          //increase the size of the slider by drawing the value difference on the top
          tft.fillRect (i * SLIDER_SPACING, tft.height() - midiCcValue[i], SLIDER_WIDTH, midiCcValue[i] - prevMidiCcValue[i], SLIDER_COLOUR);
        }
        else
        {
          //decrease the size of the slider by 'clearing' the value difference from the top
          tft.fillRect (i * SLIDER_SPACING, tft.height() - prevMidiCcValue[i], SLIDER_WIDTH, prevMidiCcValue[i] - midiCcValue[i], SLIDER_BCKGND_COLOUR);
        }

        prevMidiCcValue[i] = midiCcValue[i];
        
      } //if (midiCcValue[i] != prevMidiCcValue[i])

    } //if (control == SLIDER_CC_NUMS[i])

  } //for (uint8_t i = 0; i < NUM_OF_SLIDERS; i++)


}

