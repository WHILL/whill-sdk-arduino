#include "packet.h"

#include <Arduino.h>

void WHILL::PacketParser::setParent(WHILL* whill){
  this->whill = whill;
}

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
        if(whill!=NULL){
          whill->fire_callback(CALLBACK_POWER_ON);
        }
        break;

      default:
        return -1;  // Unknown Command
  }

  return 0;

}


void WHILL::PacketParser::parseDataset0(Packet* packet){
  if(whill == NULL) return;
  whill->fire_callback(CALLBACK_DATA0);
}

void WHILL::PacketParser::parseDataset1(Packet* packet){
  if(whill == NULL)return;
  whill->fire_callback(CALLBACK_DATA1);
}
