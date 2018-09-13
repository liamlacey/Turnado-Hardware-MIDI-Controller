
struct EncoderPins
{
  uint8_t pinA;
  uint8_t pinB;
  int8_t pinSwitch;
};

#define NUM_OF_KNOB_CONTROLLERS 8

const EncoderPins PINS_KNOB_CTRL_ENCS[NUM_OF_KNOB_CONTROLLERS] =
{
  {.pinA = 53, .pinB = 40, .pinSwitch = 52}, //1
  {.pinA = 41, .pinB = 51, .pinSwitch = 42}, //2
  {.pinA = 43, .pinB = 44, .pinSwitch = 48}, //3
  {.pinA = 45, .pinB = 47, .pinSwitch = 46}, //4
  {.pinA = 35, .pinB = 36, .pinSwitch = 37}, //5
  {.pinA = 38, .pinB = 39, .pinSwitch = 14}, //6
  {.pinA = 15, .pinB = 16, .pinSwitch = 17}, //7
  {.pinA = 19, .pinB = 20, .pinSwitch = 21}, //8
};

const EncoderPins PINS_DICTATOR_ENC =
{
  .pinA = 31,
  .pinB = 30,
  .pinSwitch = 29
};

const EncoderPins PINS_MIX_ENC =
{
  .pinA = 28,
  .pinB = 27,
  .pinSwitch = -1
};

enum LcdEncoderNames
{
  LCD_ENC_CTRL = 0,
  LCD_ENC_PARAM,
  LCD_ENC_VAL,

  NUM_OF_LCD_ENCS
};

const EncoderPins PINS_LCD_ENCS[NUM_OF_LCD_ENCS] =
{
  {.pinA = 26, .pinB = 25, .pinSwitch = 24},  //Control
  {.pinA = 8, .pinB = 7, .pinSwitch = -1},    //Paramater
  {.pinA = 6, .pinB = 5, .pinSwitch = -1},    //Value

};

const uint8_t PINS_KNOB_CTRL_JOYSTICKS[NUM_OF_KNOB_CONTROLLERS] =
{
  A24, //1
  A23, //2
  A14, //3
  A15, //4
  A21, //5
  A22, //6
  A4,  //7
  A8   //8
};

const uint8_t PIN_DICTATOR_JOYSTICK = A13;

const uint8_t PIN_PRESET_UP_BUTTON = 3;
const uint8_t PIN_PRESET_DOWN_BUTTON = 2;

const uint8_t PIN_RANDOMISE_BUTTON = 4;

const uint8_t PIN_LCD_DC = 9;
const uint8_t PIN_LCD_CS = 10;






