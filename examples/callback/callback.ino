#include <SoftwareSerial.h>
#include "WHILL.h"

SoftwareSerial ss(7,6);
WHILL whill(&ss);


void callback_data1(WHILL* caller)
{
  Serial.println("Updated");
  Serial.println(caller->joy.x);
  Serial.println(caller->joy.y);
}

void callback_powered_on(WHILL* caller)
{
  Serial.println("power_on");
}

void setup() {
  // put your setup code here, to run once:

  //Register callbacks
  whill.register_callback(callback_data1,WHILL::EVENT::CALLBACK_DATA1);
  whill.register_callback(callback_powered_on,WHILL::EVENT::CALLBACK_POWER_ON);

  whill.begin(200);   // Set sending interval(ms) of WHILL
}

void loop() {
  // put your main code here, to run repeatedly:
  whill.refresh();
}

