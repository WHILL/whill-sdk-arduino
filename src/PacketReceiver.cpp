#include "WHILL.h"

int WHILL::PacketReceiver::push(unsigned char data){

    if(!recording){
        if(data != WHILL::Packet::PROTOCOL_SIGN){
            index = 0;
            return -1;
        }else{
            recording = true;
        }
    }
        

    buf[index] = data;

    if(index >= 1 && remaining_bytes() == 0){
        call_callback();
        index = 0;
        recording = false;
        return 0;
    }

    index++;
    return remaining_bytes();
}

int WHILL::PacketReceiver::remaining_bytes(){
    if(index == 0)return -1;
    if(!recording)return -1;

    int length = 2 + buf[1];  // Protocl sign + length + [len](payload + cs)

    return length-(index+1);
}

void WHILL::PacketReceiver::register_callback(void (*callback)()){
    this->obj    = NULL;
    this->method = NULL;
    this->callback = callback;
}

void WHILL::PacketReceiver::register_callback(PacketParser* obj,int(PacketParser::*method)(WHILL::Packet* packet)){
    this->obj = obj;
    this->method = method;
    this->callback = NULL;
}



bool WHILL::PacketReceiver::call_callback(){

    Packet packet;
    packet.setRaw(buf,index+1);


    if(callback != NULL){
        this->callback();
        return true;
    }

    if(obj != NULL && method != NULL){
        (obj->*method)(&packet);
        return true;
    }

    return false;
}