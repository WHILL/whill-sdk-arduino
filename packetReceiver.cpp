#include "packet.h"
#include <Arduino.h>

namespace WHILL{

    int PacketReceiver::push(unsigned char data){

        if(!recording){
            if(data != Packet::PROTOCOL_SIGN){
                index = 0;
                return -1;
            }else{
                recording = true;
            }
        }
         

        buf[index] = data;


        if(index >= 1 && remaining_bytes() == 0){
            //callback();
            Serial.println("full!");
            index = 0;
            recording = false;
            return 0;
        }

        index++;
        return remaining_bytes();
    }

    int PacketReceiver::remaining_bytes(){
        if(index == 0)return -1;
        if(!recording)return -1;

        int length = 2 + buf[1];  // Protocl sign + length + [len](payload + cs)

        return length-(index+1);
    }

}