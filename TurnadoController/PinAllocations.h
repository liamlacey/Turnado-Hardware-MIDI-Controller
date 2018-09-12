
struct EncoderPins
{
  uint8_t pinA;
  uint8_t pinB;
  int8_t pinSwitch;
};

const EncoderPins PINS_KNOB_CTRL_ENCS[8] = 
{
  {.pinA = 2, .pinB = 3, .pinSwitch = 41}, //1
  {.pinA = 4, .pinB = 5, .pinSwitch = 42}, //2
  {.pinA = 6, .pinB = 7, .pinSwitch = 43}, //3
  {.pinA = 8, .pinB = 14, .pinSwitch = 44}, //4
  {.pinA = 15, .pinB = 16, .pinSwitch = 45}, //5
  {.pinA = 17, .pinB = 18, .pinSwitch = 46}, //6
  {.pinA = 19, .pinB = 20, .pinSwitch = 47}, //7
  {.pinA = 21, .pinB = 22, .pinSwitch = 48}, //8
};

const EncoderPins PINS_DICTATOR_ENC = 
{
  .pinA = 23,
  .pinB = 24,
  .pinSwitch = 49
};

const EncoderPins PINS_MIX_ENC = 
{
  .pinA = 25,
  .pinB = 26,
  .pinSwitch = -1
};

enum LcdControls
{
  LCD_CTRL_CTRL = 0,
  LCD_CTRL_PARAM,
  LCD_CTRL_VAL,

  NUM_OF_LCD_CTRLS
};

const EncoderPins PINS_LCD_ENCS[NUM_OF_LCD_CTRLS] = 
{
  {.pinA = 27, .pinB = 28, .pinSwitch = 52}, //Control
  {.pinA = 29, .pinB = 30, .pinSwitch = -1}, //Paramater
  {.pinA = 31, .pinB = 32, .pinSwitch = -1}, //Value
  
};

const uint8_t PINS_KNOB_CTRL_JOYSTICKS[8] = 
{
  A14, //1
  A15, //2
  A16, //3
  A17, //4
  A18, //5
  A19, //6
  A20, //7
  A21 //8
};

const uint8_t PIN_DICTATOR_JOYSTICK = A22;

const uint8_t PIN_PRESET_UP_BUTTON = 50;
const uint8_t PIN_PRESET_DOWN_BUTTON = 51;

const uint8_t PIN_RANDOMISE_BUTTON = 40;

const uint8_t PIN_LCD_DC = 9;
const uint8_t PIN_LCD_CS = 10;






