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

#ifndef __WHILL_H__
#define __WHILL_H__

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>

class WHILL {
    class Packet {
       public:
        const static unsigned char PROTOCOL_SIGN = 0xAF;

        const static int MAX_LENGTH = 35;
        const static int HEADER_SIZE = 2;  // protocol_sign, len
        const static int FOOTER_SIZE = 1;  // checksum
        const static int MAX_PAYLOAD = MAX_LENGTH - (HEADER_SIZE + FOOTER_SIZE);

        Packet();
        Packet(unsigned char payload[], int size);

        bool is_valid();
        bool setRaw(unsigned char* raw, int len);
        int getRaw(unsigned char* raw);
        unsigned char getPayload(int index);

       private:
        unsigned char calculateChecksum();

        unsigned char protocol_sign;
        unsigned char len;
        unsigned char payload[MAX_PAYLOAD];
        unsigned char checksum;
    };

    class PacketParser {
       private:
        WHILL* whill = NULL;
        void parseDataset0(WHILL::Packet* packet);
        void parseDataset1(WHILL::Packet* packet);

       public:
        void setParent(WHILL* whill);
        int parsePacket(Packet* packet);
    };

    class PacketReceiver {
       private:
        unsigned char buf[Packet::MAX_LENGTH] = {0};
        unsigned char index = 0;
        bool recording = false;

        void (*callback)() = NULL;
        bool call_callback();

        PacketParser* obj = NULL;
        int (PacketParser::*method)(WHILL::Packet* packet) = NULL;

       public:
        int push(unsigned char data);
        int remaining_bytes();
        void register_callback(void (*callback)());
        void register_callback(PacketParser* obj, int (PacketParser::*method)(
                                                      WHILL::Packet* packet));
    };

   private:
    SoftwareSerial* serial;

    // Custom One byte transceiver
    int read(unsigned char* byte);
    int write(unsigned char byte);

    bool receivePacket();
    void transferPacket(Packet* packet);

    PacketReceiver receiver;
    PacketParser parser;

    void clearCache();
    unsigned long last_received_time = 0;
    unsigned int interval_ms = 200;

   public:
    WHILL(SoftwareSerial* ss);
    void begin(unsigned int interval);

    // Callback
    enum EVENT {
        CALLBACK_DATA0,
        CALLBACK_DATA1,
        CALLBACK_POWER_ON,
        EVENT_SIZE
    };
    typedef void (*Callback)(WHILL*);
    Callback callback_functions[EVENT_SIZE] = {NULL};
    void register_callback(Callback method, EVENT event);
    void fire_callback(EVENT event);

    void refresh();
    void updateSpeedProfile();

    void keep_joy_delay(unsigned long ms);
    void delay(unsigned long ms);

    typedef enum {
        SPEED_MODE_1 = 0,
        SPEED_MODE_2,
        SPEED_MODE_3,
        SPEED_MODE_4,
        SPEED_MODE_HOST,  // RS232C
        SPEED_MODE_APP,   // Smart-phone app
        SPEED_MODE_SIZE
    } SPEED_MODE;

    typedef struct {
        unsigned char forward_speed;
        unsigned char forward_acceleration;
        unsigned char forward_deceleration;

        unsigned char reverse_speed;
        unsigned char reverse_acceleration;
        unsigned char reverse_deceleration;

        unsigned char turn_speed;
        unsigned char turn_acceleration;
        unsigned char turn_deceleration;
    } SpeedProfile;

    typedef struct {
        int x;
        int y;
    } Joy;

    typedef struct {
        unsigned char level;
        bool buzzer;
    } BatterySaving;

    typedef struct {
        unsigned char level;
        signed long current;
        BatterySaving save;
    } Battery;

    typedef struct {
        float angle;
        int speed;
    } Motor;

    SpeedProfile speed_profile[SPEED_MODE_SIZE] = {0};
    Joy virtual_joy = {0};
    Joy joy = {0};
    Battery battery = {0};
    Motor left_motor = {0};
    Motor right_motor = {0};
    bool power = false;
    int speed_mode_indicator = -1;
    unsigned char error_code = 0x00;
    unsigned char angle_detect_counter = 0x00;

    // WHILL commands
    void startSendingData0(unsigned int interval_ms, unsigned char speed_mode);
    void startSendingData1(unsigned int interval_ms);
    void stopSendingData();
    void setJoystick(int x, int y);
    void setPower(bool power);
    void setBatteryVoltaegeOut(bool out);
    void setBatterySaving(int low_battery_level, bool sounds_buzzer);
    void setSpeedProfile(SpeedProfile* profile, unsigned char speed_mode);
    void setVelocity(int y, int x);

   private:
    typedef enum {
        SENDING_STATE_STOP = 0,
        SENDING_STATE_BOOKED,
        SENDING_STATE_RUN,
    } SENDING_STATE;

    void setSendingStateData0(unsigned char mode, SENDING_STATE state);
    void setSendingStateData1(SENDING_STATE state);
    void setSendingStateAll(SENDING_STATE state);
    void selectSendingData();

    SENDING_STATE sending_data0_state[SPEED_MODE_SIZE] = {SENDING_STATE_STOP};
    SENDING_STATE sending_data1_state = SENDING_STATE_STOP;
};

#endif