#include "ILI9341_t3.h"

//For LCD use hardware SPI (#13, #12, #11) and the custom allocated for CS/DC
ILI9341_t3 lcd = ILI9341_t3 (PIN_LCD_CS, PIN_LCD_DC);

const int LCD_FRAME_RATE = 30;
long lcdPreviousMillis = 0;
const int LCD_COLOUR_BCKGND = ILI9341_BLACK;
const int LCD_COLOUR_TEXT = ILI9341_RED;
#define LCD_TEXT_LINE_SPACING 18

uint8_t lcdDisplayMode = LCD_DISPLAY_MODE_CONTROLS;

int8_t lcdCurrentlySelectedMenu = 0;
int8_t lcdPrevSelectedMenu = 0;
int8_t lcdCurrentSelectedMenuParam = 0;
int8_t lcdPrevSelectedMenuParam = 0;

//=========================================================================
void lcdDisplayCompleteMenu();

//=========================================================================
//=========================================================================
//=========================================================================
void setupLcd()
{
  lcd.begin();
  lcd.setRotation (3);
  lcd.fillScreen (LCD_COLOUR_BCKGND);

  //testing menu
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
    //TODO: draw to LCD here...

    lcdPreviousMillis = millis();
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetDisplayMode (uint8_t mode)
{
  //TODO: is display mode has changed, process this.

  lcdDisplayMode = mode;
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

  for (auto i = 0; i < SETTINGS_MAX_NUM_PARAMS; i++)
  {
    if (i == lcdCurrentSelectedMenuParam)
      lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
    else
      lcd.setTextColor (LCD_COLOUR_TEXT);

    lcd.setCursor (120, i * LCD_TEXT_LINE_SPACING);
    lcd.println (settingsData[lcdCurrentlySelectedMenu].paramData[i].name);
    
    
  } //for (auto i = 0; i < SETTINGS_MAX_NUM_PARAMS; i++)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdDisplayCompleteMenu()
{
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

    for (auto i = 0; i < SETTINGS_MAX_NUM_PARAMS; i++)
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
      }

    } //for (auto i = 0; i < LCD_MAX_NUM_OF_MENU_PARAMS; i++)
    
  } //if (lcdCurrentSelectedMenuParam != lcdPrevSelectedMenuParam)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetSelectedMenu (int8_t incVal)
{
  lcdCurrentlySelectedMenu = constrain (lcdCurrentlySelectedMenu + incVal, 0, SETTINGS_NUM_OF_CATS - 1);

  if (lcdCurrentlySelectedMenu != lcdPrevSelectedMenu)
  {
    lcdUpdateMenusDisplay();
    lcdPrevSelectedMenu = lcdCurrentlySelectedMenu;
  }
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetSelectedParam (int8_t incVal)
{
  lcdCurrentSelectedMenuParam = constrain (lcdCurrentSelectedMenuParam + incVal, 0, settingsData[lcdCurrentlySelectedMenu].numOfParams - 1);

  if (lcdCurrentSelectedMenuParam != lcdPrevSelectedMenuParam)
  {
    lcdUpdateMenuParamsAndValuesDisplay();
    lcdPrevSelectedMenuParam = lcdCurrentSelectedMenuParam;
  }
}


