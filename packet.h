#ifndef __WHILL_PACKET_H__
#define __WHILL_PACKET_H__

#include <stdio.h>

namespace WHILL{

  class ModelCR;

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
        ModelCR* whill = NULL;
        void parseDataset0(Packet* packet);
        void parseDataset1(Packet* packet);

      public:
        void setWHILLReceiver(ModelCR* whill);
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
        void (PacketParser::*method)() = NULL;

    public:
        int push(unsigned char data);
        int remaining_bytes();
        void reset();
        void register_callback(void (*callback)());
        void register_callback(PacketParser* obj,void (PacketParser::*method)());
  };

};

#endif