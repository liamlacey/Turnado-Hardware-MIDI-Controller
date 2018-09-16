//=========================================================================
//DEV STUFF...
//#define DEBUG 1

//=========================================================================
#define NUM_OF_KNOB_CONTROLLERS 9 //Includes dictator mode controller
//#define DICTATOR_KNOB_CONTROLLER_INDEX 8
#define NUM_OF_ACTUAL_KNOB_CONTROLLERS 8

enum LcdEncoderNames
{
  LCD_ENC_CTRL = 0,
  LCD_ENC_PARAM,
  LCD_ENC_VAL,

  NUM_OF_LCD_ENCS
};

enum LcdDisplayModes
{
  LCD_DISPLAY_MODE_CONTROLS = 0,
  LCD_DISPLAY_MODE_SETTINGS_MENU
};
