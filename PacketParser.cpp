#include "packet.h"

#include <Arduino.h>

int WHILL::PacketParser::parsePacket(Packet* packet){
  if(!(packet->is_valid()))return -1;

  switch(packet->payload[0]){  // Read Command ID

      case 0x00:   // Data set 0
        parseDataset0(packet);
        break;

      case 0x01:   // Data set 1
        parseDataset1(packet);
        break;

      case 0x52:   // Response of power WHILL on.
        //if(whill!=NULL) whill->powered_on();
        break;

      default:
        return -1;  // Unknown Command
  }

  return 0;

}


void WHILL::PacketParser::parseDataset0(Packet* packet){
  Serial.println("Parse Data0");
}

void WHILL::PacketParser::parseDataset1(Packet* packet){
  Serial.println("Parse Data1");
}
