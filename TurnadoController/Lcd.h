#include "ILI9341_t3.h"

//For LCD use hardware SPI (#13, #12, #11) and the custom allocated for CS/DC
ILI9341_t3 lcd = ILI9341_t3 (PIN_LCD_CS, PIN_LCD_DC);

const int LCD_FRAME_RATE = 30;
long lcdPreviousMillis = 0;
const int LCD_COLOUR_BCKGND = ILI9341_BLACK;
const int LCD_COLOUR_TEXT = ILI9341_RED;

uint8_t lcdDisplayMode = LCD_DISPLAY_MODE_CONTROLS;

//TODO: use settingsData and other defines in Settings.h for LCD menu stuff instead of the below

#define LCD_NUM_OF_MENUS 13
#define LCD_MAX_NUM_OF_MENU_PARAMS 8

struct LcdMenuData
{
  const char menuName[16];
  const uint8_t numOfParams;
  const char paramNames[LCD_MAX_NUM_OF_MENU_PARAMS][16];
};

LcdMenuData lcdMenuData[LCD_NUM_OF_MENUS] =
{
  {"Global", .numOfParams = 1, {"Channel"}},
  {"Knob1", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Knob2", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Knob3", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Knob4", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Knob5", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Knob6", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Knob7", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Knob8", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Dictator", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Mix", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Random", .numOfParams = 2, {"Channel", "CC Num"}},
  {"Preset", .numOfParams = 2, {"Channel", "1st Prgm"}}
};


#define LCD_TEXT_LINE_SPACING 18

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
  for (auto i = 0; i < LCD_NUM_OF_MENUS; i++)
  {
    if (i == lcdCurrentlySelectedMenu)
      lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
    else
      lcd.setTextColor (LCD_COLOUR_TEXT);

    lcd.setCursor (0, i * LCD_TEXT_LINE_SPACING);
    lcd.println (lcdMenuData[i].menuName);
  }
}
//=========================================================================
//=========================================================================
//=========================================================================
void lcdDisplayMenuParamsAndValues()
{
  lcd.fillRect (120, 0, 105, lcd.height(), LCD_COLOUR_BCKGND);

  for (auto i = 0; i < LCD_MAX_NUM_OF_MENU_PARAMS; i++)
  {
    if (i == lcdCurrentSelectedMenuParam)
      lcd.setTextColor (LCD_COLOUR_BCKGND, LCD_COLOUR_TEXT);
    else
      lcd.setTextColor (LCD_COLOUR_TEXT);

    lcd.setCursor (120, i * LCD_TEXT_LINE_SPACING);
    lcd.println (lcdMenuData[lcdCurrentlySelectedMenu].paramNames[i]);
  }
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
    for (auto i = 0; i < LCD_NUM_OF_MENUS; i++)
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
        lcd.println (lcdMenuData[i].menuName);

        lcdDisplayMenuParamsAndValues();
      }

    } //for (auto i = 0; i < LCD_NUM_OF_MENUS; i++)

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

    for (auto i = 0; i < LCD_MAX_NUM_OF_MENU_PARAMS; i++)
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
        lcd.println (lcdMenuData[lcdCurrentlySelectedMenu].paramNames[i]);
      }

    } //for (auto i = 0; i < LCD_MAX_NUM_OF_MENU_PARAMS; i++)
    
  } //if (lcdCurrentSelectedMenuParam != lcdPrevSelectedMenuParam)
}

//=========================================================================
//=========================================================================
//=========================================================================
void lcdSetSelectedMenu (int8_t incVal)
{
  lcdCurrentlySelectedMenu = constrain (lcdCurrentlySelectedMenu + incVal, 0, LCD_NUM_OF_MENUS - 1);

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
  lcdCurrentSelectedMenuParam = constrain (lcdCurrentSelectedMenuParam + incVal, 0, lcdMenuData[lcdCurrentlySelectedMenu].numOfParams - 1);

  if (lcdCurrentSelectedMenuParam != lcdPrevSelectedMenuParam)
  {
    lcdUpdateMenuParamsAndValuesDisplay();
    lcdPrevSelectedMenuParam = lcdCurrentSelectedMenuParam;
  }
}





