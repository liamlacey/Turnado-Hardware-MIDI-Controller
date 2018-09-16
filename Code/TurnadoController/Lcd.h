#include "ILI9341_t3.h"

//For LCD use hardware SPI (#13, #12, #11) and the custom allocated for CS/DC
ILI9341_t3 lcd = ILI9341_t3 (PIN_LCD_CS, PIN_LCD_DC);

//=========================================================================
//global LCD stuff...

const int LCD_FRAME_RATE = 30;
long lcdPreviousMillis = 0;

const int LCD_COLOUR_BCKGND = ILI9341_BLACK;
const int LCD_COLOUR_TEXT = ILI9341_RED;
const int LCD_COLOUR_SLIDERS_VALUE = ILI9341_RED;
const int LCD_COLOUR_SLIDERS_BCKGND = ILI9341_DARKGREY;

#define LCD_TEXT_LINE_SPACING 18

uint8_t lcdDisplayMode = LCD_DISPLAY_MODE_CONTROLS;

//=========================================================================
//controls display stuff...

const uint8_t LCD_SLIDER_WIDTH = 20;

const uint8_t LCD_KNOB_SLIDER_LENGTH = 127;
const uint8_t LCD_KNOB_SLIDER_SPACING = 43;

const uint8_t LCD_DICT_SLIDER_Y_POS = 60;
const uint8_t LCD_MIX_SLIDER_Y_POS = 25;

const uint8_t LCD_NUM_OF_SLIDERS = 10;
const uint8_t LCD_SLIDER_DICTATOR_INDEX = 8;
const uint8_t LCD_SLIDER_MIX_INDEX = 9;

//below arrays store values as midi / 7-bit values.
uint8_t lcdSliderValue[LCD_NUM_OF_SLIDERS] = {0};
uint8_t lcdPrevSliderValue[LCD_NUM_OF_SLIDERS] = {0};

//=========================================================================
//menu display stuff...

//TODO: make the below variable an adjustable global setting?
const bool lcdAutoSwitchToMenuDisplay = true;

int8_t lcdCurrentlySelectedMenu = 0;
int8_t lcdPrevSelectedMenu = 0;
int8_t lcdCurrentSelectedMenuParam = 0;
int8_t lcdPrevSelectedMenuParam = 0;

//=========================================================================
void lcdDisplayControls();
void lcdDisplayCompleteMenu();
void lcdPrintParamValueToDisplay (uint8_t menu, uint8_t param);

//=========================================================================
//=========================================================================
//=========================================================================
void setupLcd()
{
  lcd.begin();
  lcd.setRotation (3);
  lcd.fillScreen (LCD_COLOUR_BCKGND);

  if (lcdDisplayMode == LCD_DISPLAY_MODE_CONTROLS)
    lcdDisplayControls();
  else
    lcdDisplayCompleteMenu();
}

//=========================================================================
//=========================================================================
//=========================================================================
void updateLcd()
{
  //update the LCD display at the set frame rate
  if ((millis() - lcdPreviousMillis) > (1000.0 / (float)LCD_FRAME_RATE))
  {
    //FIXME: should I be doing menu drawing here too? Current it is just drawn as control changes are made.

    //if in control display mode, update the controls display if needed
    if (lcdDisplayMode == LCD_DISPLAY_MODE_CONTROLS)
    {
      for (uint8_t i = 0; i < LCD_NUM_OF_SLIDERS; i++)
      {
        //if the slider value needs updating
        if (lcdSliderValue[i] != lcdPrevSliderValue[i])
        {
          //=========================================================================
          //if one of the horizontal knob controller sliders
          if (i < LCD_SLIDER_DICTATOR_INDEX)
          {
            //if slider value has increased
            if (lcdSliderValue[i] > lcdPrevSliderValue[i])
            {
              //increase the size of the slider by drawing the value difference on the top
              lcd.fillRect (i * LCD_KNOB_SLIDER_SPACING,
                            (lcd.height() - lcdSliderValue[i]) - (LCD_TEXT_LINE_SPACING + 2),
                            LCD_SLIDER_WIDTH,
                            lcdSliderValue[i] - lcdPrevSliderValue[i],
                            LCD_COLOUR_SLIDERS_VALUE);
            }
            //if slider value has decreased
            else
            {
              //decrease the size of the slider by 'clearing' the value difference from the top
              lcd.fillRect (i * LCD_KNOB_SLIDER_SPACING,
                            (lcd.height() - lcdPrevSliderValue[i]) - (LCD_TEXT_LINE_SPACING + 2),
                            LCD_SLIDER_WIDTH,
                            lcdPrevSliderValue[i] - lcdSliderValue[i],
                            LCD_COLOUR_SLIDERS_BCKGND);
            }

          } //if (i < LCD_SLIDER_DICTATOR_INDEX)

          //=========================================================================

          //=========================================================================

          //=========================================================================
          lcdPrevSliderValue[i] = lcdSliderValue[i];

        } //if (lcdSliderValue[i] != lcdPrevSliderValue[i])

      } //for (uint8_t i = 0; i < LCD_NUM_OF_SLIDERS; i++)

    } //if (lcdDisplayMode == LCD_DISPLAY_MODE_CONTROLS)

    lcdPreviousMillis = millis();

  } //if ((millis() - lcdPreviousMillis) > (1000.0 / (float)LCD_FRAME_RATE))
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetDisplayMode (uint8_t mode)
{
  if (mode != lcdDisplayMode)
  {
    lcdDisplayMode = mode;

    if (lcdDisplayMode == LCD_DISPLAY_MODE_CONTROLS)
      lcdDisplayControls();
    else
      lcdDisplayCompleteMenu();

  } //if (mode != lcdDisplayMode)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdToggleDisplayMode()
{
  lcdSetDisplayMode (!lcdDisplayMode);
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdDisplayControls()
{
  //FIXME: would be better if the controls display was drawn using positions relative to the LCD size, rather than using absolute values.

  lcd.fillScreen (LCD_COLOUR_BCKGND);
  lcd.setTextColor (LCD_COLOUR_TEXT);
  lcd.setTextSize (2);

  //draw the 8 knob controller values as vertical 'sliders' with numbers underneath at the bottom of the display
  for (uint8_t i = 0; i < NUM_OF_ACTUAL_KNOB_CONTROLLERS; i++)
  {
    lcd.fillRect (i * LCD_KNOB_SLIDER_SPACING,
                  (lcd.height() - LCD_KNOB_SLIDER_LENGTH) - (LCD_TEXT_LINE_SPACING + 2),
                  LCD_SLIDER_WIDTH,
                  LCD_KNOB_SLIDER_LENGTH,
                  LCD_COLOUR_SLIDERS_BCKGND);

    lcd.setCursor ((i * LCD_KNOB_SLIDER_SPACING) + 5, lcd.height() - LCD_TEXT_LINE_SPACING);
    lcd.println (i + 1);

  } //for (uint8_t i = 0; i < NUM_OF_ACTUAL_KNOB_CONTROLLERS; i++)

  //draw the dictator mode value as a horizontal 'slider' above the knob controller sliders
  //with the parameter name on the left-hand side.

  lcd.setCursor (0, LCD_DICT_SLIDER_Y_POS);
  lcd.println ("Dcttr:");

  lcd.fillRect (LCD_KNOB_SLIDER_SPACING * 2,
                LCD_DICT_SLIDER_Y_POS,
                lcd.width() - (LCD_KNOB_SLIDER_SPACING * 2),
                LCD_SLIDER_WIDTH,
                LCD_COLOUR_SLIDERS_BCKGND);

  //draw the mix value as a horizontal 'slider' above the dictator slider
  //with the parameter name on the left-hand side.

  lcd.setCursor (0, LCD_MIX_SLIDER_Y_POS);
  lcd.println ("Mix:");

  lcd.fillRect (LCD_KNOB_SLIDER_SPACING * 2,
                LCD_MIX_SLIDER_Y_POS,
                lcd.width() - (LCD_KNOB_SLIDER_SPACING * 2),
                LCD_SLIDER_WIDTH,
                LCD_COLOUR_SLIDERS_BCKGND);
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetSliderValue (uint8_t sliderNum, uint8_t paramMidiVal)
{
  lcdSliderValue[sliderNum] = paramMidiVal;
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdDisplayMenus()
{
  for (auto i = 0; i < SETTINGS_NUM_OF_CATS; i++)
  {
    if (i == lcdCurrentlySelectedMenu)
      lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
    else
      lcd.setTextColor (LCD_COLOUR_TEXT);

    lcd.setCursor (0, i * LCD_TEXT_LINE_SPACING);
    lcd.println (settingsData[i].name);

  }//for (auto i = 0; i < SETTINGS_NUM_OF_CATS; i++)
}
//=========================================================================
//=========================================================================
//=========================================================================
void lcdDisplayMenuParamsAndValues()
{
  lcd.fillRect (120, 0, 105, lcd.height(), LCD_COLOUR_BCKGND);
  lcd.fillRect (240, 0, 80, lcd.height(), LCD_COLOUR_BCKGND);

  for (auto i = 0; i < settingsData[lcdCurrentlySelectedMenu].numOfParams; i++)
  {
    if (i == lcdCurrentSelectedMenuParam)
      lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
    else
      lcd.setTextColor (LCD_COLOUR_TEXT);

    lcd.setCursor (120, i * LCD_TEXT_LINE_SPACING);
    lcd.println (settingsData[lcdCurrentlySelectedMenu].paramData[i].name);

    lcd.setCursor (240, i * LCD_TEXT_LINE_SPACING);
    lcdPrintParamValueToDisplay (lcdCurrentlySelectedMenu, i);

  } //for (auto i = 0; i < settingsData[lcdCurrentlySelectedMenu].numOfParams; i++)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdDisplayCompleteMenu()
{
  //FIXME: would be better if the menu display was drawn using positions relative to the LCD size, rather than using absolute values.

  lcd.fillScreen (LCD_COLOUR_BCKGND);
  lcd.setTextSize (2);

  lcdDisplayMenus();
  lcdDisplayMenuParamsAndValues();

  lcd.fillRect (105, 0, 2, lcd.height(), LCD_COLOUR_TEXT);
  lcd.fillRect (225, 0, 2, lcd.height(), LCD_COLOUR_TEXT);
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdUpdateMenusDisplay()
{
  if (lcdCurrentlySelectedMenu != lcdPrevSelectedMenu)
  {
    for (auto i = 0; i < SETTINGS_NUM_OF_CATS; i++)
    {
      bool updateText = false;

      if (i == lcdPrevSelectedMenu)
      {
        updateText = true;
        lcd.setTextColor (LCD_COLOUR_TEXT, LCD_COLOUR_BCKGND);
      }
      if (i == lcdCurrentlySelectedMenu)
      {
        updateText = true;
        lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
      }

      if (updateText)
      {
        lcd.setCursor (0, i * LCD_TEXT_LINE_SPACING);
        lcd.println (settingsData[i].name);

        lcdDisplayMenuParamsAndValues();
      }

    } //for (auto i = 0; i < SETTINGS_NUM_OF_CATS; i++)

  } //if (lcdCurrentlySelectedMenu != lcdPrevSelectedMenu)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdUpdateMenuParamsAndValuesDisplay()
{
  if (lcdCurrentSelectedMenuParam != lcdPrevSelectedMenuParam)
  {
    lcd.fillRect (120, 0, 105, lcd.height(), LCD_COLOUR_BCKGND);
    lcd.fillRect (240, 0, 80, lcd.height(), LCD_COLOUR_BCKGND);

    for (auto i = 0; i < settingsData[lcdCurrentlySelectedMenu].numOfParams; i++)
    {
      bool updateText = false;

      if (i == lcdPrevSelectedMenuParam)
      {
        updateText = true;
        lcd.setTextColor (LCD_COLOUR_TEXT, LCD_COLOUR_BCKGND);
      }
      if (i == lcdCurrentSelectedMenuParam)
      {
        updateText = true;
        lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
      }

      if (updateText)
      {
        lcd.setCursor (120, i * LCD_TEXT_LINE_SPACING);
        lcd.println (settingsData[lcdCurrentlySelectedMenu].paramData[i].name);

        lcd.setCursor (240, i * LCD_TEXT_LINE_SPACING);
        lcdPrintParamValueToDisplay (lcdCurrentlySelectedMenu, i);
      }

    } //for (auto i = 0; i < settingsData[lcdCurrentlySelectedMenu].numOfParams; i++)

  } //if (lcdCurrentSelectedMenuParam != lcdPrevSelectedMenuParam)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdUpdateMenuSelectedValue()
{
  lcd.fillRect (240, lcdCurrentSelectedMenuParam * LCD_TEXT_LINE_SPACING, 80, LCD_TEXT_LINE_SPACING, LCD_COLOUR_BCKGND);

  lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
  lcd.setCursor (240, lcdCurrentSelectedMenuParam * LCD_TEXT_LINE_SPACING);

  lcdPrintParamValueToDisplay (lcdCurrentlySelectedMenu, lcdCurrentSelectedMenuParam);
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdPrintParamValueToDisplay (uint8_t menu, uint8_t param)
{
  if (param == PARAM_INDEX_MIDI_CHAN &&
      menu != SETTINGS_GLOBAL &&
      settingsData[menu].paramData[param].value == 0)
  {
    lcd.println ("Global");
  }
  else
  {
    lcd.println (settingsData[menu].paramData[param].value);
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetSelectedMenu (int8_t incVal)
{
  if (lcdDisplayMode == LCD_DISPLAY_MODE_SETTINGS_MENU || lcdAutoSwitchToMenuDisplay)
  {
    if (lcdDisplayMode != LCD_DISPLAY_MODE_SETTINGS_MENU)
      lcdSetDisplayMode (LCD_DISPLAY_MODE_SETTINGS_MENU);

    lcdCurrentlySelectedMenu = constrain (lcdCurrentlySelectedMenu + incVal, 0, SETTINGS_NUM_OF_CATS - 1);

    if (lcdCurrentlySelectedMenu != lcdPrevSelectedMenu)
    {
      lcdUpdateMenusDisplay();
      lcdPrevSelectedMenu = lcdCurrentlySelectedMenu;
    }
  } //if (lcdDisplayMode = LCD_DISPLAY_MODE_SETTINGS_MENU || lcdAutoSwitchToMenuDisplay)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetSelectedParam (int8_t incVal)
{
  if (lcdDisplayMode == LCD_DISPLAY_MODE_SETTINGS_MENU || lcdAutoSwitchToMenuDisplay)
  {
    if (lcdDisplayMode != LCD_DISPLAY_MODE_SETTINGS_MENU)
      lcdSetDisplayMode (LCD_DISPLAY_MODE_SETTINGS_MENU);

    lcdCurrentSelectedMenuParam = constrain (lcdCurrentSelectedMenuParam + incVal, 0, settingsData[lcdCurrentlySelectedMenu].numOfParams - 1);

    if (lcdCurrentSelectedMenuParam != lcdPrevSelectedMenuParam)
    {
      lcdUpdateMenuParamsAndValuesDisplay();
      lcdPrevSelectedMenuParam = lcdCurrentSelectedMenuParam;
    }

  } //if (lcdDisplayMode = LCD_DISPLAY_MODE_SETTINGS_MENU || lcdAutoSwitchToMenuDisplay)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetSelectedParamValue (int8_t incVal)
{
  if (lcdDisplayMode == LCD_DISPLAY_MODE_SETTINGS_MENU || lcdAutoSwitchToMenuDisplay)
  {
    if (lcdDisplayMode != LCD_DISPLAY_MODE_SETTINGS_MENU)
      lcdSetDisplayMode (LCD_DISPLAY_MODE_SETTINGS_MENU);

    uint8_t currentVal = settingsData[lcdCurrentlySelectedMenu].paramData[lcdCurrentSelectedMenuParam].value;
    uint8_t minVal = settingsData[lcdCurrentlySelectedMenu].paramData[lcdCurrentSelectedMenuParam].minVal;
    uint8_t maxVal = settingsData[lcdCurrentlySelectedMenu].paramData[lcdCurrentSelectedMenuParam].maxVal;

    uint8_t newVal = constrain (currentVal + incVal, minVal, maxVal);

    if (newVal != currentVal)
    {
      settingsData[lcdCurrentlySelectedMenu].paramData[lcdCurrentSelectedMenuParam].value = newVal;
      lcdUpdateMenuSelectedValue();

      //flag that the new value needs saving to EEPROM
      settingsData[lcdCurrentlySelectedMenu].paramData[lcdCurrentSelectedMenuParam].needsSavingToEeprom = true;
    }

  } //if (lcdDisplayMode = LCD_DISPLAY_MODE_SETTINGS_MENU || lcdAutoSwitchToMenuDisplay)
}


