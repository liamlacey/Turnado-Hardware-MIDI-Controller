#include <EEPROM.h>

//=========================================================================
enum SettingsCategories
{
  SETTINGS_GLOBAL = 0,
  SETTINGS_KNOB_1,
  SETTINGS_KNOB_2,
  SETTINGS_KNOB_3,
  SETTINGS_KNOB_4,
  SETTINGS_KNOB_5,
  SETTINGS_KNOB_6,
  SETTINGS_KNOB_7,
  SETTINGS_KNOB_8,
  SETTINGS_DICTATOR,
  SETTINGS_MIX,
  SETTINGS_RANDOMISE,
  SETTINGS_PRESET,

  SETTINGS_NUM_OF_CATS
};

#define SETTINGS_MAX_NUM_PARAMS 16

#define PARAM_INDEX_MIDI_CHAN 0
#define PARAM_INDEX_CC_NUM 1
#define PARAM_INDEX_START_NUM 1

//=========================================================================

struct ParamData
{
  const char name[16];
  const uint8_t minVal;
  const uint8_t maxVal;
  const uint8_t memAddrOffset; //I dont think I need this, as the offset is the index of ParamData in an array

  uint8_t defaultValue;
  uint8_t value;

  bool needsSavingToEeprom = false;
};

const ParamData paramDataTemplateChannelGlobal = {"Channel", .minVal = 1, .maxVal = 16, .memAddrOffset = PARAM_INDEX_MIDI_CHAN, .defaultValue = 0, .value = 0, .needsSavingToEeprom = false};
const ParamData paramDataTemplateChannelControl = {"Channel", .minVal = 0, .maxVal = 16, .memAddrOffset = PARAM_INDEX_MIDI_CHAN, .defaultValue = 16, .value = 0, .needsSavingToEeprom = false};
const ParamData paramDataTemplateCcNumber = {"CC Num", .minVal = 0, .maxVal = 127, .memAddrOffset = PARAM_INDEX_CC_NUM, .defaultValue = 1, .value = 0, .needsSavingToEeprom = false};
const ParamData paramDataTemplatePrgmStartNumber = {"1st Prgm", .minVal = 0, .maxVal = 127, .memAddrOffset = PARAM_INDEX_START_NUM, .defaultValue = 0, .value = 0, .needsSavingToEeprom = false};

struct SettingsCategoryData
{
  const char name[16];
  const uint8_t numOfParams;

  ParamData paramData[SETTINGS_MAX_NUM_PARAMS];
};

SettingsCategoryData settingsData[SETTINGS_NUM_OF_CATS] =
{
  {
    "Global",
    .numOfParams = 1,
    {
      paramDataTemplateChannelGlobal
    },
  },

  {
    "Knob1",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Knob2",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Knob3",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Knob4",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },
  {
    "Knob5",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Knob6",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Knob7",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Knob8",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Dictator",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Mix",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Random",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplateCcNumber,
    },
  },

  {
    "Preset",
    .numOfParams = 2,
    {
      paramDataTemplateChannelControl,
      paramDataTemplatePrgmStartNumber,
    },
  },

};

long updateEepromTime = 0;

//=========================================================================
void settingsLoadAllFromEeprom();
void settingsClearEeprom();
void settingsSaveToEeprom (bool deltaSave);

//=========================================================================
//=========================================================================
//=========================================================================
void setupSettings()
{
  //TODO: set param default values in settingsData

  settingsLoadAllFromEeprom();
}

//=========================================================================
//=========================================================================
//=========================================================================
void settingsUpdateEeprom()
{
  //Only save settings to EEPROM at certain time intervals (rather than directly after each value change)
  //to prevent the number of writes to EEPROM (which there is a limited number of)
  if ((millis() - updateEepromTime) > 5000)
  {
    settingsSaveToEeprom (true);
    updateEepromTime = millis();

  } //if ((millis() - updateEepromTime) > 5000)

}

//=========================================================================
//=========================================================================
//=========================================================================
void settingsSaveToEeprom (bool deltaSave)
{
  for (auto cat = 0; cat < SETTINGS_NUM_OF_CATS; cat++)
  {
    for (auto param = 0; param < settingsData[cat].numOfParams; param++)
    {
      //if the param value needs saving (it has recently changed), or we want to do a full save
      if (settingsData[cat].paramData[param].needsSavingToEeprom || !deltaSave)
      {
        //Write the param value to EEPROM
        EEPROM.write ((cat * SETTINGS_MAX_NUM_PARAMS) + param, settingsData[cat].paramData[param].value);

#ifdef DEBUG
        Serial.print ("Writing to EEPROM: ");
        Serial.print (settingsData[cat].name);
        Serial.print (" ");
        Serial.print (settingsData[cat].paramData[param].name);
        Serial.print (" ");
        Serial.print (settingsData[cat].paramData[param].value);
        Serial.print (" (Address ");
        Serial.print ((cat * SETTINGS_MAX_NUM_PARAMS) + param);
        Serial.println (")");
#endif

        //flag that the param value has been saved
        settingsData[cat].paramData[param].needsSavingToEeprom = false;

      } //if (settingsData[cat].paramData[param].needsSavingToEeprom || !deltaSave)

    } //for (auto param = 0; param < settingsData[cat].numOfParams; param++)

  } //for (auto cat = 0; cat < SETTINGS_NUM_OF_CATS; cat++)
}

//=========================================================================
//=========================================================================
//=========================================================================
void settingsLoadAllFromEeprom()
{
#ifdef DEBUG
  Serial.println ("Settings loaded from EEPROM: ");
#endif

  for (auto cat = 0; cat < SETTINGS_NUM_OF_CATS; cat++)
  {
#ifdef DEBUG
    Serial.print (settingsData[cat].name);
    Serial.print (" - ");
#endif

    for (auto param = 0; param < settingsData[cat].numOfParams; param++)
    {
      //Load the param value from EEPROM
      settingsData[cat].paramData[param].value = EEPROM.read ((cat * SETTINGS_MAX_NUM_PARAMS) + param);

#ifdef DEBUG
      Serial.print (settingsData[cat].paramData[param].name);
      Serial.print (": ");
      Serial.print (settingsData[cat].paramData[param].value);
      Serial.print (", ");
#endif
    } //for (auto param = 0; param < settingsData[cat].numOfParams; param++)

#ifdef DEBUG
    Serial.println ();
#endif

  } //for (auto cat = 0; cat < SETTINGS_NUM_OF_CATS; cat++)

#ifdef DEBUG
  Serial.println ();
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


