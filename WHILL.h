#ifndef __WHILL_H__
#define __WHILL_H__

#include <stdio.h>

class SoftwareSerial;


// typedef struct {

// } WHILL::Accelometer;

// typedef struct {

// } Gyro;

// typedef struct {

// } Joy;

// typedef struct {

// } Battery;

// typedef struct {

// } Motor;


class WHILL{


    class Packet{

        private:

        public:
            const static unsigned char PROTOCOL_SIGN = 0xAF;

            const static int MAX_LENGTH   = 20;
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

    void updateData0();
    void updateData1();
    void powered_on();

    PacketReceiver receiver;
    PacketParser   parser;

public:

    WHILL(SoftwareSerial* ss);

    void enableUpdate(int dataset_number);
    void disableUpdate();

    void register_callback();

    void setJoystick();
    void refresh();

};





#endif