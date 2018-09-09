#ifndef __WHILL_H__
#define __WHILL_H__

class SoftwareSerial;


namespace WHILL{

    class Packet;

    typedef struct {

    } Accelometer;

    typedef struct {

    } Gyro;

    typedef struct {

    } Joy;

    typedef struct {

    } Battery;

    typedef struct {

    } Motor;


    class ModelCR{

    private:
        SoftwareSerial* serial;
        void transferPacket(Packet* packet);
    public:

        ModelCR(SoftwareSerial* ss);

        void start();

        void setJoystick();
        void refresh();



        void updateData0();
        void updateData1();
        void powered_on();

    };


}

#endif