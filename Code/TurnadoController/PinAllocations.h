
struct EncoderPins
{
  uint8_t pinA;
  uint8_t pinB;
  int8_t pinSwitch;
};

const EncoderPins PINS_KNOB_CTRL_ENCS[NUM_OF_KNOB_CONTROLLERS] =
{
  {.pinA = 40, .pinB = 53, .pinSwitch = 52}, //1
  {.pinA = 51, .pinB = 41, .pinSwitch = 42}, //2
  {.pinA = 44, .pinB = 43, .pinSwitch = 48}, //3
  {.pinA = 47, .pinB = 45, .pinSwitch = 46}, //4
  {.pinA = 36, .pinB = 35, .pinSwitch = 37}, //5
  {.pinA = 39, .pinB = 38, .pinSwitch = 14}, //6
  {.pinA = 16, .pinB = 15, .pinSwitch = 17}, //7
  {.pinA = 20, .pinB = 19, .pinSwitch = 21}, //8
  {.pinA = 30, .pinB = 31, .pinSwitch = 29}  //dictator
};

const EncoderPins PINS_MIX_ENC =
{
  .pinA = 27,
  .pinB = 28,
  .pinSwitch = -1
};

const EncoderPins PINS_LCD_ENCS[NUM_OF_LCD_ENCS] =
{
  {.pinA = 25, .pinB = 26, .pinSwitch = 24},  //Control
  {.pinA = 7, .pinB = 8, .pinSwitch = -1},    //Paramater
  {.pinA = 5, .pinB = 6, .pinSwitch = -1},    //Value

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
  A8,  //8
  A13, //dictator
};

const uint8_t PIN_PRESET_UP_BUTTON = 3;
const uint8_t PIN_PRESET_DOWN_BUTTON = 2;

const uint8_t PIN_RANDOMISE_BUTTON = 4;

const uint8_t PIN_LCD_DC = 9;
const uint8_t PIN_LCD_CS = 10;






