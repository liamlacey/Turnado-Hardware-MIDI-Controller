#include "ILI9341_t3.h"

//For LCD use hardware SPI (#13, #12, #11) and the custom allocated for CS/DC
ILI9341_t3 lcd = ILI9341_t3 (PIN_LCD_CS, PIN_LCD_DC);

const int LCD_FRAME_RATE = 30;
long lcdPreviousMillis = 0;
const int LCD_COLOUR_BCKGND = ILI9341_BLACK;

uint8_t displayMode = LCD_DISPLAY_MODE_CONTROLS;

//=========================================================================
//=========================================================================
//=========================================================================
void setupLcd()
{
  lcd.begin();
  lcd.setRotation (3);
  lcd.fillScreen (LCD_COLOUR_BCKGND);

  lcd.setTextColor ( ILI9341_RED);
  lcd.setTextSize (2);
  lcd.println ("Hello, how are you?");
  lcd.setCursor (50, 30);
  lcd.setTextColor (LCD_COLOUR_BCKGND, ILI9341_RED);
  lcd.println ("I'm good, and you?");

//  lcd.drawChar(10, 10, 'P', ILI9341_RED, LCD_COLOUR_BCKGND, 2);
//  lcd.drawChar(25, 10, 'O', ILI9341_RED, LCD_COLOUR_BCKGND, 2);
//  lcd.drawChar(40, 10, 'O', ILI9341_RED, LCD_COLOUR_BCKGND, 2);
//  lcd.drawChar(55, 10, 'N', ILI9341_RED, LCD_COLOUR_BCKGND, 2);
//  lcd.drawChar(70, 10, 'A', ILI9341_RED, LCD_COLOUR_BCKGND, 2);
//  lcd.drawChar(85, 10, 'D', ILI9341_RED, LCD_COLOUR_BCKGND, 2);
//  lcd.drawChar(100, 10, 'O', ILI9341_RED, LCD_COLOUR_BCKGND, 2);
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

  
  displayMode = mode;
}



