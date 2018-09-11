#include "WHILL.h"

void WHILL::startSendingData0(unsigned int interval_ms,unsigned char speed_mode){
    unsigned char payload[] =  {0x00,
                                0x00,
                                (unsigned char)(interval_ms<<8 & 0xFF),
                                (unsigned char)(interval_ms<<0 & 0xFF),
                                speed_mode};
    Packet packet(payload,sizeof(payload));
    packet.build();
    transferPacket(&packet);
}

void WHILL::startSendingData1(unsigned int interval_ms){
    unsigned char payload[] =  {0x00,
                                0x01,
                                (unsigned char)(interval_ms<<8 & 0xFF),
                                (unsigned char)(interval_ms<<0 & 0xFF),
                                0x00};
    Packet packet(payload,sizeof(payload));
    packet.build();
    transferPacket(&packet);
}

void WHILL::setJoystick(int x,int y){
     unsigned char payload[] = {0x03,
                                0x00,   // Enable Host control
                                (unsigned char)(char)(y),
                                (unsigned char)(char)(x)};
    Packet packet(payload,sizeof(payload));
    packet.build();
    transferPacket(&packet);       
}

void setPower(bool power){

}


void SetBatteryVoltaegeOut(bool out){

}