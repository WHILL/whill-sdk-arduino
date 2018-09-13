#include <SoftwareSerial.h>
#include "WHILL.h"

SoftwareSerial ss(7,6);
WHILL whill(&ss);

void callback_powered_on(WHILL* caller)
{
  // This function is called when receive Joy/Accelerometer/Gyro,etc.
  Serial.print("powered_on");
}

void setup() {
  // put your setup code here, to run once:

  //Register callbacks
  whill.register_callback(callback_powered_on,WHILL::EVENT::CALLBACK_POWER_ON);

  whill.begin(50);   // Set refresh interval of WHILL
}

void loop() {
  // put your main code here, to run repeatedly:
  whill.refresh();
  whill.setPower(true);  // Turn WHILL on
  whill.delay(5000);

  whill.setPower(false);  // Turn WHILL off
  whill.delay(5000);  
}
