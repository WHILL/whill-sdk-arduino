#include "packet.h"

namespace WHILL{

  int parsePacket(Packet* packet){
    if(!(packet->is_valid()))return -1;

    switch(packet->payload[0]){
        case 0x00:
        default:
            break;
    }

    return 0;
  }


}