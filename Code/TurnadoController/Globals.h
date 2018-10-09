//=========================================================================
//DEV STUFF...
//#define DEBUG 1

//=========================================================================
#define NUM_OF_KNOB_CONTROLLERS 9 //Includes dictator mode controller
#define DICTATOR_KNOB_CONTROLLER_INDEX 8
#define NUM_OF_ACTUAL_KNOB_CONTROLLERS 8

//Device params = knob controllers, dictator controller, and mix control
#define NUM_OF_DEVICE_PARAMS (NUM_OF_KNOB_CONTROLLERS + 1)
#define DEVICE_PARAM_INDEX_DICTATOR 8
#define DEVICE_PARAM_INDEX_MIX 9

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
   _TODO:_
   Comment / document code
   See other TODO's and FIXME's in code
   
   _Future version feature and changes ideas:_
   - Allow dictator encoder switch to 'stick' any current used knob joysticks if being used
   - Implement knob and dictator control time quantisation, where the activating and deactivating of controls is quantised to a set MIDI time / clock ticks value (configurable globally and per control). This will involve the controller repsonding to MIDI clock messages and MIDI start messages. 
   - Allow internal presets of settings that can be changed with a button combination (LCD ctrl switch + preset buttons?). Display preset number in controls display top bar.
   - Improve knob controller (and dictator) sliders on LCS to show the difference between the base value and relative value. E.g Base value shown with a bar, relative value shown as slider value starting at bar position.
   - Implement global setting for auto switching LCD display with control messages
   - Have a global settings option to set control settings to default settings
   - Consider improving LCD display general layouts, colours, etc...
   - Allow device to be a fully assignable generic MIDI controller, where each control (except for the LCD controls) have completely configurable MIDI messages
   - Change LCD drawing code to use relative positions of the LCD size (as opposed to absolute values)
   - Consider drawing menu display from 'update' function rather than on control changes
*/
