#include <string.h>

#include "WHILL.h"
#include "packet.h"

namespace WHILL{

  Packet::Packet(){
    protocol_sign = 0xAF;
    len = 1;
    build();
  }

  Packet::Packet(unsigned char payload[],int size){
    protocol_sign = 0xAF;
    len = size + 1;
    memcpy(this->payload,payload,size);
    build();
  }

  bool Packet::is_valid(){
    return getCalculatedCS() == cs;
  }

  int Packet::rawLength(){
    return 2 + len;   // protocol_sign + len + (the length of payload and cs)
  }

  bool Packet::setRaw(unsigned char* raw,int whole_length){

      protocol_sign = raw[0];
      len           = raw[1];

      int prefix = 2;
      int i = 0;
      for(i=0;i<len-1;i++){
        payload[i]= raw[prefix+i];
      }

      cs = raw[prefix+i];
      
      return is_valid();
  }

  unsigned char Packet::getCalculatedCS(){
    unsigned char cs = 0x00;

    cs ^= protocol_sign;
    cs ^= len;

    for(int i=0;i<len-1;i++){
      cs ^= payload[i];
    }

    return cs;
  }

  void Packet::build(){
    this->cs = getCalculatedCS();
  }

  int Packet::getRaw(unsigned char* raw){

    int whole_length = 0;

    raw[0] = protocol_sign;
    raw[1] = len;

    int prefix = 2;
    int i = 0;
    for(i=0;i<len-1;i++){
      raw[prefix+i] = payload[i];
    }

    raw[prefix+i] = cs;
    
    return rawLength();
  }

}