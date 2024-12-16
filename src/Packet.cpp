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

WHILL::Packet::Packet() {
    protocol_sign = PROTOCOL_SIGN;
    len = FOOTER_SIZE;
    checksum = calculateChecksum();
}

WHILL::Packet::Packet(unsigned char payload[], int size) {
    protocol_sign = PROTOCOL_SIGN;
    len = size + FOOTER_SIZE;
    memcpy(this->payload, payload, size);
    checksum = calculateChecksum();
}

bool WHILL::Packet::is_valid() { return calculateChecksum() == checksum; }

bool WHILL::Packet::setRaw(unsigned char* raw, int whole_length) {
    int idx = 0;

    protocol_sign = raw[idx++];
    len = raw[idx++];
    for (int i = 0; i < len - FOOTER_SIZE; i++) {
        payload[i] = raw[idx++];
    }
    checksum = raw[idx++];

    return is_valid();
}

int WHILL::Packet::getRaw(unsigned char* raw) {
    int idx = 0;

    raw[idx++] = protocol_sign;
    raw[idx++] = len;
    for (int i = 0; i < len - FOOTER_SIZE; i++) {
        raw[idx++] = payload[i];
    }
    raw[idx++] = checksum;

    return idx;
}

unsigned char WHILL::Packet::getPayload(int index) {
    if (index >= MAX_PAYLOAD) return 0;
    return payload[index];
}

unsigned char WHILL::Packet::calculateChecksum() {
    unsigned char checksum = 0x00;

    checksum ^= protocol_sign;
    checksum ^= len;
    for (int i = 0; i < len - FOOTER_SIZE; i++) {
        checksum ^= payload[i];
    }

    return checksum;
}
