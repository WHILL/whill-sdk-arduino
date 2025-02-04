/*
The MIT License (MIT)

Copyright (c) 2018 WHILL,

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "WHILL.h"

void WHILL::PacketParser::setParent(WHILL* whill) { this->whill = whill; }

int WHILL::PacketParser::parsePacket(Packet* packet) {
    if (!(packet->is_valid())) return -1;

    switch (packet->getPayload(0)) {  // Read Command ID

        case 0x00:  // Data set 0
            parseDataset0(packet);
            if (whill != NULL) {
                whill->fire_callback(CALLBACK_DATA0);
            }
            break;

        case 0x01:  // Data set 1
            parseDataset1(packet);
            if (whill != NULL) {
                whill->fire_callback(CALLBACK_DATA1);
            }
            break;

        case 0x52:  // Response of power WHILL on.
            if (whill != NULL) {
                whill->fire_callback(CALLBACK_POWER_ON);
            }
            break;

        default:
            return -1;  // Unknown Command
    }

    return 0;
}

void WHILL::PacketParser::parseDataset0(Packet* packet) {
    if (whill == NULL) return;

    unsigned char mode = packet->getPayload(1);
    if (mode >= SPEED_MODE_SIZE) return;

    whill->speed_profile[mode].forward_speed = packet->getPayload(2);
    whill->speed_profile[mode].forward_acceleration = packet->getPayload(3);
    whill->speed_profile[mode].forward_deceleration = packet->getPayload(4);
    whill->speed_profile[mode].reverse_speed = packet->getPayload(5);
    whill->speed_profile[mode].reverse_acceleration = packet->getPayload(6);
    whill->speed_profile[mode].reverse_deceleration = packet->getPayload(7);
    whill->speed_profile[mode].turn_speed = packet->getPayload(8);
    whill->speed_profile[mode].turn_acceleration = packet->getPayload(9);
    whill->speed_profile[mode].turn_deceleration = packet->getPayload(10);

    whill->onReceivedData0(mode);
}

void WHILL::PacketParser::parseDataset1(Packet* packet) {
    if (whill == NULL) return;

    whill->battery.save.level = packet->getPayload(1);
    whill->battery.save.buzzer = (packet->getPayload(2) == 0x01) ? true : false;

    whill->joy.y = (int)(signed char)packet->getPayload(13);
    whill->joy.x = (int)(signed char)packet->getPayload(14);

    whill->battery.level = (unsigned char)packet->getPayload(15);
    whill->battery.current =
        (signed short)((packet->getPayload(16) << 8) + (packet->getPayload(17))) *
        2;  // Unit : 2mA

    whill->right_motor.angle =
        (float)((signed short)((packet->getPayload(18) << 8) +
                               (packet->getPayload(19))) *
                0.001);
    whill->left_motor.angle =
        (float)((signed short)((packet->getPayload(20) << 8) +
                               (packet->getPayload(21))) *
                0.001);

    whill->right_motor.speed =
        (signed short)((packet->getPayload(22) << 8) + (packet->getPayload(23))) * 4;
    whill->left_motor.speed =
        (signed short)((packet->getPayload(24) << 8) + (packet->getPayload(25))) * 4;

    whill->power = packet->getPayload(26) == 0x00 ? false : true;
    whill->speed_mode_indicator = packet->getPayload(27);
    whill->error_code = packet->getPayload(28);
    whill->angle_detect_counter = packet->getPayload(29);

    whill->onReceivedData1();
}
