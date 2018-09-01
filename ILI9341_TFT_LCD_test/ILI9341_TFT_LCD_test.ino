//#include "SPI.h"
#include "ILI9341_t3.h"

// TFT pins
const uint8_t TFT_DC = 9;
const uint8_t TFT_CS = 10;

// Use hardware SPI (#13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3 (TFT_CS, TFT_DC);

const int LCD_FRAME_RATE = 30;
long previousMillis = 0;

const int BCKGND_COLOUR = ILI9341_BLACK;
const int SLIDER_COLOUR = ILI9341_RED;
const int SLIDER_BCKGND_COLOUR = ILI9341_DARKGREY;

const int SLIDER_WIDTH = 20;
const int SLIDER_MAX_SIZE = 127;
const int SLIDER_SPACING = 40;

const uint8_t NUM_OF_SLIDERS = 8;
const uint8_t SLIDER_CC_NUMS[NUM_OF_SLIDERS] =  {2, 3, 4, 5, 6, 8, 9, 12};

int sliderValue[NUM_OF_SLIDERS] = {0};
int prevSliderValue[NUM_OF_SLIDERS] = {0};

uint8_t midiCcValue[NUM_OF_SLIDERS] = {0};
uint8_t prevMidiCcValue[NUM_OF_SLIDERS] = {0};

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

  //update the LCD display at the set frame rate
  if ((millis() - previousMillis) > (1000.0 / (float)LCD_FRAME_RATE))
  {
    updateDisplay();
    previousMillis = millis();
  }
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
        //set the new value for the slider
        sliderValue[i] = midiCcValue[i];

        prevMidiCcValue[i] = midiCcValue[i];

      } //if (midiCcValue[i] != prevMidiCcValue[i])

    } //if (control == SLIDER_CC_NUMS[i])

  } //for (uint8_t i = 0; i < NUM_OF_SLIDERS; i++)

}

void updateDisplay()
{
  for (uint8_t i = 0; i < NUM_OF_SLIDERS; i++)
  {
    //if the slider value needs updating
    if (sliderValue[i] != prevSliderValue[i])
    {
      if (sliderValue[i] > prevSliderValue[i])
      {
        //increase the size of the slider by drawing the value difference on the top
        tft.fillRect (i * SLIDER_SPACING, tft.height() - sliderValue[i], SLIDER_WIDTH, sliderValue[i] - prevSliderValue[i], SLIDER_COLOUR);
      }
      else
      {
        //decrease the size of the slider by 'clearing' the value difference from the top
        tft.fillRect (i * SLIDER_SPACING, tft.height() - prevSliderValue[i], SLIDER_WIDTH, prevSliderValue[i] - sliderValue[i], SLIDER_BCKGND_COLOUR);
      }

      prevSliderValue[i] = sliderValue[i];

    } //if (sliderValue[i] != prevSliderValue[i])

  } //for (uint8_t i = 0; i < NUM_OF_SLIDERS; i++)

}

