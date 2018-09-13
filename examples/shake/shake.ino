#include <SoftwareSerial.h>
#include "WHILL.h"

SoftwareSerial ss(7,6);  // TX pin 7, RX pin 6
WHILL whill(&ss);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  whill.refresh();

  whill.setJoystick(50,0);    // Turn Right
  whill.keep_joy_delay(1000);

  whill.setJoystick(-50,0);   // Turn Left
  whill.keep_joy_delay(1000);
}