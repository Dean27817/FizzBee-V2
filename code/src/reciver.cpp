#include "Arduino.h"
#include "reciver.h"
#include <IBusBM.h>

//sets up the input for the reciver
Reciver::Reciver( int RXPin )
{

     Serial.begin(115200);
     Serial1.begin(115200, SERIAL_8N1, RXPin); // RX pin
     this -> ibus.begin(Serial1);
}

//reads the input from the IBus reciver
void Reciver::loop()
{
     input.x = float( ibus.readChannel(0) ) / 500  - 3 ;
     input.y = float( ibus.readChannel(1) ) / 500  - 3 ;
     input.throttle = float( ibus.readChannel(2) ) / 500  - 3 ;
     input.throttleHori = float( ibus.readChannel(3) ) / 500  - 3 ;
     input.varA = float( ibus.readChannel(4) ) / 500  - 3 ;
     input.varB = float( ibus.readChannel(5) ) / 500  - 3 ;
}

