#ifndef __WHILL_H__
#define __WHILL_H__

#include <SoftwareSerial.h>

namespace WHILL{
class WHILL{

    public:
        SoftwareSerial serial;

    public:

        WHILL(SoftwareSerial ss);

        void start();

        void setJoystick();
        void refresh();

};


}

#endif