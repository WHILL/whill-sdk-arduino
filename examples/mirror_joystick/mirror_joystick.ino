#include <SoftwareSerial.h>

#include "WHILL.h"

SoftwareSerial ss(7, 6);
WHILL whill(&ss);

void callback_data1(WHILL* caller) {
    // This function is called when receive Joy,etc.
    int joy_x = -caller->joy.x;         // mirror
    int joy_y = -caller->joy.y;         // mirror
    caller->setJoystick(joy_x, joy_y);  // Set mirrored joystick
}

void setup() {
    // put your setup code here, to run once:

    // Register callbacks
    whill.register_callback(callback_data1, WHILL::EVENT::CALLBACK_DATA1);

    whill.begin(50);  // Set refresh interval of WHILL
}

void loop() {
    // put your main code here, to run repeatedly:
    whill.refresh();
}
