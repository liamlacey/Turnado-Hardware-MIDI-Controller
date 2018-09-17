//=========================================================================
//DEV STUFF...
#define DEBUG 1

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

/*
 * _TODO:_
 * Test all untested code / features
 * Comment / document code
 * Implement rotary encoder acceleration in class, allow it to be turned off (LCD ctrl and param encoders maybe shouldn't use it)
 * Randomise + preset button combination to change global MIDI channel
 * Update settings on top bar of controls display in realtime
 * Processing of MIDI-in CCs from Turnado's knobs.
 * Add a centre bar to LCD mix slider to show centre point.
 * Consider improving LCD display general layouts, colours, etc...
 * 
 * 
 * _Future version feature ideas:_
 * Allow internal presets of settings that can be changed with a button combination (LCD ctrl switch + preset buttons?). Display preset number in controls display top bar.
 * Improve knob controller (and dictator) sliders to show the difference between the base value and relative value. E.g Base value shown with a bar, relative value shown as slider value starting at bar position.
 * Implement global setting for auto switching LCD display with control messages
 */
