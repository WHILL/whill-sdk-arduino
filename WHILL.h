#ifndef __WHILL_H__
#define __WHILL_H__

#include <Arduino.h>
#include <stdio.h>

class SoftwareSerial;


class WHILL{

    class Packet{

        private:

        public:
            const static unsigned char PROTOCOL_SIGN = 0xAF;

            const static int MAX_LENGTH   = 35;
            const static int MAX_PAYLOAD  = MAX_LENGTH - 3;  // protocol_sign,len,cs

            Packet();
            Packet(unsigned char payload[],int size);

            unsigned char       getCalculatedCS();

            unsigned char       protocol_sign;
            unsigned char       len;
            unsigned char       payload[MAX_LENGTH];
            unsigned char       cs;

            bool is_valid();

            int  rawLength();

            bool setRaw(unsigned char* raw,int len);
            int  getRaw(unsigned char* raw);
            void build();
    };



    class PacketParser{

        private:
            WHILL* whill = NULL;
            void parseDataset0(WHILL::Packet* packet);
            void parseDataset1(WHILL::Packet* packet);

        public:
            void setParent(WHILL* whill);
            void setWHILLReceiver(WHILL* whill);
            int parsePacket(Packet* packet); 

    };


    class PacketReceiver{

        private:
            unsigned char buf[Packet::MAX_LENGTH]   = {0};
            unsigned char index     = 0;
            bool          recording = false;

            void(*callback)()       = NULL;
            bool          call_callback();

            PacketParser* obj       = NULL;
            int (PacketParser::*method)(WHILL::Packet* packet) = NULL;

        public:
            int push(unsigned char data);
            int remaining_bytes();
            void reset();
            void register_callback(void (*callback)());
            void register_callback(PacketParser* obj,int(PacketParser::*method)(WHILL::Packet* packet));
    };



private:
    SoftwareSerial* serial;

    // Custom One byte transceiver
    int read(unsigned char* byte);
    int write(unsigned char byte);

    void receivePacket();
    void transferPacket(Packet* packet);

    PacketReceiver receiver;
    PacketParser   parser;


public:

    WHILL(SoftwareSerial* ss);
 
    //Callback
    enum EVENT{
        CALLBACK_DATA0,
        CALLBACK_DATA1,
        CALLBACK_POWER_ON,
        EVENT_SIZE
    }; 
    typedef void (*Callback)(WHILL*);
    Callback callback_functions[EVENT_SIZE] = {NULL};
    void register_callback(Callback method,EVENT event);
    void fire_callback(EVENT event);

    void refresh();

    int virtual_joy_x = 0;
    int virtual_joy_y = 0;

    void keep_joy_delay(unsigned long ms);
    void delay(unsigned long ms);

    typedef struct{
        unsigned char forward_spped;
        unsigned char forward_acceleration;
        unsigned char forward_deceleration;

        unsigned char reverse_speed;
        unsigned char reverse_acceleration;
        unsigned char reverse_deceleration;

        unsigned char turn_speed;
        unsigned char turn_acceleration;
        unsigned char turn_deceleration;       
    }SpeedProfile;

    //WHILL commands
    void startSendingData0(unsigned int interval_ms,unsigned char speed_mode);
    void startSendingData1(unsigned int interval_ms);
    void stopSendingData();
    void setJoystick(int x,int y);
    void setPower(bool power);
    void setBatteryVoltaegeOut(bool out);
    void setSpeedProfile(SpeedProfile* profile,unsigned char speed_mode);

};


#endif