#include "RotaryEncoder.h"
#include "SwitchControl.h"
#include "ThumbJoystick.h"
#include "PinAllocations.h"

//RotaryEncoder encoder (5, 6, 4);
//SwitchControl pushButton (3);
//ThumbJoystick joystick (A0);

void setup() 
{
  Serial.begin(9600);
  delay(500);
  
//  encoder.onEncoderChange (processEncoderChange);
//  encoder.onSwitchChange (processEncoderSwitchChange);
//  pushButton.onSwitchStateChange (processPushButtonChange);
//  joystick.onJoystickChange (processJoystickChange);
}

void loop() 
{
//  encoder.update();
//  pushButton.update();
//  joystick.update();

}

void processEncoderChange (RotaryEncoder &enc, int enc_value)
{
//  if (enc == encoder)
//  {
//    Serial.print ("Enc1: ");
//    Serial.println (enc_value);
//  }
}

void processEncoderSwitchChange (RotaryEncoder &enc)
{
//  if (enc == encoder)
//  {
//    Serial.print ("Enc1 switch: ");
//    Serial.println (enc.getSwitchState());
//  }
}

void processPushButtonChange (SwitchControl &switchControl)
{
//  if (pushButton == switchControl)
//  {
//    Serial.print ("Push Button: ");
//    Serial.println (switchControl.getSwitchState());
//  }
}

void processJoystickChange (ThumbJoystick &thumbJoystick, bool isYAxis)
{
//  if (thumbJoystick == joystick && isYAxis)
//  {
//    Serial.print ("JS: ");
//    Serial.println (thumbJoystick.getYAxisValue());
//  } 
}
