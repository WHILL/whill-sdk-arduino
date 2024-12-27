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

WHILL::WHILL(WhillSerial* serial) {
    this->serial = serial;
    this->serial->begin(38400);

    parser.setParent(this);

    receiver.register_callback(&parser, &PacketParser::parsePacket);
}

void WHILL::begin(unsigned int interval) {
    interval_ms = interval;
    this->updateSpeedProfile();
}

void WHILL::register_callback(Callback method, EVENT event) {
    callback_functions[event] = method;
}

void WHILL::refresh() {
    this->switchDataset();

    unsigned long now_time = millis();
    if (receivePacket()) {
        last_received_time = now_time;
        return;
    }

    if ((now_time - last_received_time) > (unsigned long)(interval_ms * 2)) {
        this->clearCache();
    }
}

void WHILL::keep_joy_delay(unsigned long ms) {
    while (ms > 0) {
        refresh();
        if (ms % 100 == 0) {
            this->setJoystick(virtual_joy.x, virtual_joy.y);
        }
        ms--;
        ::delay(1);
    }
}

void WHILL::delay(unsigned long ms) {
    while (ms > 0) {
        refresh();
        ms--;
        ::delay(1);
    }
}

void WHILL::updateSpeedProfile() {
    this->setSendingStateAll(SENDING_STATE_BOOKED);
}

void WHILL::fire_callback(EVENT event) {
    if (callback_functions[event] == NULL) return;
    callback_functions[event](this);
}

int WHILL::read(
    unsigned char* byte) {  // Implementation of read interaface to WHILL
    if (serial == NULL) return -1;

    int data = serial->read();
    if (data == -1) return -1;  // Nothing read

    *byte = data;

    return 1;
}

int WHILL::write(
    unsigned char byte) {  // Implementation of write interface to WHILL
    if (serial == NULL) return -1;
    serial->write(byte);

    return 1;
}

bool WHILL::receivePacket() {
    bool is_received = false;
    unsigned char data = 0;

    while (read(&data) != -1) {
        receiver.push(data);
        is_received = true;
    }
    return is_received;
}

void WHILL::transferPacket(Packet* packet) {
    unsigned char buffer[Packet::MAX_LENGTH] = {0};
    int size = packet->getRaw(buffer);
    if (size > 1) {
        for (int i = 0; i < size; i++) {
            write(buffer[i]);
        }
    }
}

void WHILL::switchDataset() {
    // If there is no reply, retry to send.
    bool should_resend = false;
    unsigned long now_time = millis();
    if ((now_time - last_sent_time) > (unsigned long)(interval_ms * 2)) {
        should_resend = true;
    }

    // update dataset0
    for (int mode = 0; mode < (int)SPEED_MODE_SIZE; mode++) {
        switch (sending_data0_state[mode]) {
            case SENDING_STATE_BOOKED:
                sending_data0_state[mode] = SENDING_STATE_SENT;
                this->startSendingData0(interval_ms, mode);
                last_sent_time = now_time;
                return;
            case SENDING_STATE_SENT:
                if (should_resend) {
                    sending_data0_state[mode] = SENDING_STATE_BOOKED;
                }
                return;
            default:
                break;
        }
    }

    // update dataset1
    switch (sending_data1_state) {
        case SENDING_STATE_BOOKED:
            sending_data1_state = SENDING_STATE_SENT;
            this->startSendingData1(interval_ms);
            last_sent_time = now_time;
            return;
        case SENDING_STATE_SENT:
            if (should_resend) {
                sending_data1_state = SENDING_STATE_BOOKED;
            }
            return;
        default:
            break;
    }
}

void WHILL::clearCache() {
    joy.y = 0;
    joy.x = 0;
    battery.current = 0;
    right_motor.speed = 0;
    left_motor.speed = 0;
    power = false;

    // Never set to zero, as these are power state independent.
    // --------------------------------------------------------
    // speed_profile[]
    // battery.level
    // battery.save.level
    // battery.save.buzzer
    // right_motor.angle
    // left_motor.angle
    // speed_mode_indicator
    // error_code
    // angle_detect_counter
}

void WHILL::onReceivedData0(unsigned char mode) {
    if (mode >= SPEED_MODE_SIZE) return;
    if (sending_data0_state[mode] != SENDING_STATE_SENT) return;
    sending_data0_state[mode] = SENDING_STATE_RECEIVED;
}

void WHILL::onReceivedData1() {
    if (sending_data1_state != SENDING_STATE_SENT) return;
    sending_data1_state = SENDING_STATE_RECEIVED;
}

void WHILL::setSendingStateAll(SENDING_STATE state) {
    for (int mode = 0; mode < (int)SPEED_MODE_SIZE; mode++) {
        sending_data0_state[mode] = state;
    }
    sending_data1_state = state;
}
