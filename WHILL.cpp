#include <SoftwareSerial.h>
#include <Arduino.h>

#include "WHILL.h"


WHILL::WHILL(SoftwareSerial* ss){
    this->serial = ss;
    this->serial->begin(38400);

    parser.setParent(this);

    receiver.register_callback(&parser,&PacketParser::parsePacket);
}

int WHILL::read(unsigned char* byte){    // Implementation of read interaface to WHILL
    if(serial == NULL) return -1;
    
    int data = serial->read();
    if(data == -1) return -1;  //Nothing read
    
    *byte = data;

    return 1;
}

int WHILL::write(unsigned char byte){   // Implementation of write interface to WHILL
    if(serial == NULL) return -1;
    serial->write(byte);
}


void WHILL::transferPacket(Packet* packet){
    unsigned char buffer[Packet::MAX_LENGTH] = {0};
    int size = packet->getRaw(buffer);
    if(size > 1){
        for(int i = 0; i < size;i++){
            write(buffer[i]);
        }
    }
}

void WHILL::receivePacket(){
    unsigned char data;
    while(read(&data) != -1){
        receiver.push(data);
    }
}

void WHILL::keep_joy_delay(unsigned long ms){
    unsigned long counter = 0;
    while(counter < ms){
        refresh();
        if(counter%80 == 0){
            this->setJoystick(virtual_joy_x,virtual_joy_y);
        }
        counter++;
        ::delay(1);
    }  
}


void WHILL::delay(unsigned long ms){
    while(ms>0){
        refresh();
        ms--;
        ::delay(1);
    }
}


void WHILL::refresh(){
    // Scan the data from interface
   receivePacket();
}


void WHILL::register_callback(Callback method,EVENT event){
    callback_functions[event] = method;
}

void WHILL::fire_callback(EVENT event){
    if(callback_functions[event]==NULL)return;
    callback_functions[event](this);
}