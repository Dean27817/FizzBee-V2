#include "Arduino.h"
#include <IBusBM.h>

#ifndef RECIVER_H
#define RECIVER_H

//gets the values from the reciver
class Reciver
{
    public:

        //all of the input channels from the transmitter
        struct
        {
            float x;
            float y;
            float throttle;
            float throttleHori;
            float varA;
            float varB;
        } input;

        //begin the communication with the reciver
        Reciver( int RXPin );
        //updates all the values from the reciver
        void loop();

    private: 
        //the reciver object
        IBusBM ibus;

};
#endif
