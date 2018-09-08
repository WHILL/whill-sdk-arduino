#ifndef __WHILL_PACKET_H__
#define __WHILL_PACKET_H__

namespace WHILL{

    
  class Packet{



    private:

    public:
        const static unsigned char PROTOCOL_SIGN = 0xAF;

        Packet();
        Packet(unsigned char payload[],int size);

        unsigned char       getCalculatedCS();

        unsigned char       protocol_sign;
        unsigned char       len;
        unsigned char       payload[20];
        unsigned char       cs;

        bool is_valid();

        int  rawLength();

        bool setRaw(unsigned char* raw,int len);
        int  getRaw(unsigned char* raw);
        void build();
  };

  class PacketReceiver{

    private:
        unsigned char buf[30]   = {0};
        unsigned char index     = 0;
        bool          recording = false;

    public:
        int push(unsigned char data);
        int remaining_bytes();
        void reset();
  };
  
};

#endif