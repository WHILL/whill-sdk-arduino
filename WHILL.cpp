#include <SoftwareSerial.h>

#include "WHILL.h"


namespace WHILL{

    ModelCR::ModelCR(SoftwareSerial* ss){
        this->serial = ss;
    }

}