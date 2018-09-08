class WHILL{

  public:
  class Packet{

    private:

    public:
      Packet();

      unsigned char       getCalculatedCS();

      unsigned char       protocol_sign;
      unsigned char       len;
      unsigned char       payload[20];
      unsigned char       cs;


      bool is_valid();

      int  rawLength();

      bool setRaw(unsigned char* raw,int len);
      int  getRaw(unsigned char* raw);
      void build();
  };
  
};

WHILL::Packet::Packet(){
  protocol_sign = 0xAF;
  len = 1;
  build();
}

bool WHILL::Packet::is_valid(){
  return getCalculatedCS() == cs;
}

int WHILL::Packet::rawLength(){
  return 2 + len;   // protocol_sign + len + (the length of payload and cs)
}

bool WHILL::Packet::setRaw(unsigned char* raw,int whole_length){

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

unsigned char WHILL::Packet::getCalculatedCS(){
  unsigned char cs = 0x00;

  cs ^= protocol_sign;
  cs ^= len;

  for(int i=0;i<len-1;i++){
    cs ^= payload[i];
  }

  return cs;
}

void WHILL::Packet::build(){
  this->cs = getCalculatedCS();
}

int WHILL::Packet::getRaw(unsigned char* raw){

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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Serial.println("Start");

  WHILL::Packet packet;
  packet.build();

  unsigned char data[20] = {0};
  int length = packet.getRaw(data);

  Serial.print("cs:");
  Serial.println(packet.cs);

  for(int i=0;i<length;i++){
    Serial.println(data[i],HEX);
  }

  Serial.println("\nParse");


  WHILL::Packet packet2;
  packet2.setRaw(data,length);

  packet2.getRaw(data);
  
  for(int i=0;i<length;i++){
    Serial.println(data[i],HEX);
  }

  Serial.println("cs:");
  Serial.println(packet2.cs);

  Serial.println("calcCS:");
  Serial.println(packet2.getCalculatedCS());

  Serial.print("valid:");
  if(packet2.is_valid()){
    Serial.println("valid");
  }else{
    Serial.println("invalid");
  }
 
}

void loop() {
  // put your main code here, to run repeatedly:

}
