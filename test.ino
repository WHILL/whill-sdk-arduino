#include "packet.h"


void callback(){
  Serial.println("callback");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Serial.println("Start");

//   unsigned char payload[] = {0x00,0x01,0x00,20,0};
//   WHILL::Packet packet(payload,sizeof(payload));
//   packet.build();

//   unsigned char data[20] = {0};
//   int length = packet.getRaw(data);

//   Serial.print("cs:");
//   Serial.println(packet.cs);

//   for(int i=0;i<length;i++){
//     Serial.print("0x");
//     if(data[i]<=0xF)Serial.print("0");
//     Serial.println(data[i],HEX);
//   }

//   Serial.println("\nParse");


//   WHILL::Packet packet2;
//   packet2.setRaw(data,length);

//   packet2.getRaw(data);
  
//   for(int i=0;i<length;i++){
//     Serial.print("0x");
//     if(data[i]<=0xF)Serial.print("0");
//     Serial.println(data[i],HEX);
//   }

//   Serial.println("cs:");
//   Serial.println(packet2.cs);

//   Serial.println("calcCS:");
//   Serial.println(packet2.getCalculatedCS());

//   Serial.print("valid:");
//   if(packet2.is_valid()){
//     Serial.println("valid");
//   }else{
//     Serial.println("invalid");
//   }


    unsigned char data[] = {0xAF,0x03,0x01,0x02,0x03,0xFF,0xFF,0xAF,0x04,0x01,0x02,0x03,0x04};

    WHILL::PacketReceiver receiver;

    receiver.register_callback(callback);

    for(int i=0;i<sizeof(data);i++){
        Serial.println(data[i],HEX);
        receiver.push(data[i]);
    }
 
}

void loop() {
  // put your main code here, to run repeatedly:

}
