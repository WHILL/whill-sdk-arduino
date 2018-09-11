#include "WHILL.h"
#include <SoftwareSerial.h>


SoftwareSerial ss(7,6);
WHILL whill(&ss);


void setup() {
  // put your setup code here, to run once:
  Serial.println("Start");
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  whill.refresh();

  for(int i=0;i<10;i++){
    whill.setJoystick(-50,0);
    delay(100);
  }

  for(int i=0;i<10;i++){
    whill.setJoystick(50,0);
    delay(100);
  }


}
