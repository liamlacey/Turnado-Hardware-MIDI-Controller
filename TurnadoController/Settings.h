#include <EEPROM.h>

//=========================================================================
enum SettingsTypesEepromAddress
{
  SETTINGS_GLOBAL = 0,
  SETTINGS_KNOB_1 = 16,
  SETTINGS_KNOB_2 = 32,
  SETTINGS_KNOB_3 = 48,
  SETTINGS_KNOB_4 = 64,
  SETTINGS_KNOB_5 = 80,
  SETTINGS_KNOB_6 = 96,
  SETTINGS_KNOB_7 = 112,
  SETTINGS_KNOB_8 = 128,
  SETTINGS_DICTATOR = 144,
  SETTINGS_MIX = 160,
  SETTINGS_RANDOMISE = 176,
  SETTINGS_PRESET = 192
};

#define PARAM_ADDR_MIDI_CHAN 0
#define PARAM_ADDR_CC_NUM 1
#define PARAM_ADDR_START_NUM 1

//=========================================================================

struct GlobalSettings
{
  uint8_t midiChannel;
};

struct GenericControlSettings
{
  uint8_t midiChannel;
  uint8_t midiCcNumber;
};

struct PresetButtonSettings
{
  uint8_t midiChannel;
  uint8_t startProgramNumber;
};

GlobalSettings globalSettings;
GenericControlSettings knobControllerSettings[NUM_OF_KNOB_CONTROLLERS];
GenericControlSettings dictatorSettings;
GenericControlSettings mixSettings;
GenericControlSettings randomiseSettings;
PresetButtonSettings presetButtonSettings;

//=========================================================================
void settingsLoadAllFromEeprom();
void settingsClearEeprom();

//=========================================================================
//=========================================================================
//=========================================================================
void setupSettings()
{
  settingsLoadAllFromEeprom();
}

//=========================================================================
//=========================================================================
//=========================================================================
void settingsLoadAllFromEeprom()
{
  globalSettings.midiChannel = EEPROM.read (SETTINGS_GLOBAL + PARAM_ADDR_MIDI_CHAN);

  for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
  {
    knobControllerSettings[i].midiChannel = EEPROM.read ((SETTINGS_KNOB_1 * (i + 1)) + PARAM_ADDR_MIDI_CHAN);
    knobControllerSettings[i].midiCcNumber = EEPROM.read ((SETTINGS_KNOB_1 * (i + 1)) + PARAM_ADDR_CC_NUM);
  }

  dictatorSettings.midiChannel = EEPROM.read (SETTINGS_DICTATOR + PARAM_ADDR_MIDI_CHAN);
  dictatorSettings.midiCcNumber = EEPROM.read (SETTINGS_DICTATOR + PARAM_ADDR_CC_NUM);

  mixSettings.midiChannel = EEPROM.read (SETTINGS_MIX + PARAM_ADDR_MIDI_CHAN);
  mixSettings.midiCcNumber = EEPROM.read (SETTINGS_MIX + PARAM_ADDR_CC_NUM);

  randomiseSettings.midiChannel = EEPROM.read (SETTINGS_RANDOMISE + PARAM_ADDR_MIDI_CHAN);
  randomiseSettings.midiCcNumber = EEPROM.read (SETTINGS_RANDOMISE + PARAM_ADDR_CC_NUM);

  presetButtonSettings.midiChannel = EEPROM.read (SETTINGS_PRESET + PARAM_ADDR_MIDI_CHAN);
  presetButtonSettings.startProgramNumber = EEPROM.read (SETTINGS_PRESET + PARAM_ADDR_START_NUM);

#ifdef DEBUG
  Serial.println ("Settings loaded from EEPROM: ");

  Serial.print ("Global - MIDI channel: ");
  Serial.println (globalSettings.midiChannel);

  for (auto i = 0; i < NUM_OF_KNOB_CONTROLLERS; i++)
  {
    Serial.print ("Knob Controller ");
    Serial.print (i + 1);
    Serial.print (" - MIDI channel: ");
    Serial.print (knobControllerSettings[i].midiChannel);
    Serial.print (", MIDI CC number: ");
    Serial.println (knobControllerSettings[i].midiCcNumber);
  }

  Serial.print ("Dictator - MIDI channel: ");
  Serial.print (dictatorSettings.midiChannel);
  Serial.print (", MIDI CC number: ");
  Serial.println (dictatorSettings.midiCcNumber);

  Serial.print ("Mix - MIDI channel: ");
  Serial.print (mixSettings.midiChannel);
  Serial.print (", MIDI CC number: ");
  Serial.println (mixSettings.midiCcNumber);

  Serial.print ("Randomise - MIDI channel: ");
  Serial.print (randomiseSettings.midiChannel);
  Serial.print (", MIDI CC number: ");
  Serial.println (randomiseSettings.midiCcNumber);

  Serial.print ("Preset - MIDI channel: ");
  Serial.print (presetButtonSettings.midiChannel);
  Serial.print (", Start program number: ");
  Serial.println (presetButtonSettings.startProgramNumber);

  Serial.println();

#endif
}

//=========================================================================
//=========================================================================
//=========================================================================
void settingsClearEeprom()
{
  for (int i = 0 ; i < EEPROM.length() ; i++)
    EEPROM.write (i, 0);

}


