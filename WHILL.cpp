#include <SoftwareSerial.h>
#include <Arduino.h>

#include "WHILL.h"


WHILL::WHILL(SoftwareSerial* ss){
    this->serial = ss;
    this->serial->begin(38400);

    receiver.register_callback(&parser,&PacketParser::parsePacket);

    startSendingData1(1000);

        // unsigned char payload[] = {0x00,0x00,0x00,0x00};
        // Packet packet(payload,sizeof(payload));
        // packet.build();

        // unsigned char data[30];
        // int size = packet.getRaw(data);

        // for(int i=0;i<size;i++){
        //     Serial.print("0x");
        //     Serial.println(data[i],HEX);
        // }

        // transferPacket(&packet);
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



void WHILL::refresh(){
    // Scan the data from interface
    //unsigned char read_data;
    // while(this->read(&read_data) != -1){
    //     receiver.push((unsigned char)read_data);
    // }
   receivePacket();
}



