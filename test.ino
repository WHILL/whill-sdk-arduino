#include <SoftwareSerial.h>
#include "WHILL.h"

SoftwareSerial ss(7,6);
WHILL whill(&ss);


void callback_data0(WHILL* caller)
{
  Serial.println("call0");
}

void callback_data1(WHILL* caller)
{
  Serial.println("call1");
}

void callback_powered_on(WHILL* caller)
{
  Serial.println("power_on");
}

void setup() {
  // put your setup code here, to run once:
  whill.register_callback(callback_data1,WHILL::EVENT::CALLBACK_DATA1);
  whill.register_callback(callback_data0,WHILL::EVENT::CALLBACK_DATA0);
  whill.register_callback(callback_powered_on,WHILL::EVENT::CALLBACK_POWER_ON);

  //whill.startSendingData0(1000,0);
  whill.stopSendingData();

  Serial.println("Start");
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  whill.refresh();
  whill.setPower(true);
  whill.delay(5000);
  whill.setPower(false);
  whill.delay(2000);
}
