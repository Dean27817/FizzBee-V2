#include <Arduino.h>
#include "motor.h"

//initialize the ledc channel for the motors
Motor::Motor( int motorPinIn, int ledcChannelIn, int freq )
{
    //sets the private values in the class
    this -> ledcChannelNum = ledcChannelIn;
    this -> motorPin = motorPinIn;

    //sets up the LEDC channel
    ledcSetup( ledcChannelNum, freq, 12 );
    ledcAttachPin( motorPin, ledcChannelNum );
}


//sets the speed of the motor based on the percent speed passed in
//passed a value between -1 and 1 for 100% backwards and 100% forwards
void Motor::setSpeed( float speed )
{
    //maps the percentage value to the one being used by the motor
    int ledcMicros = map( speed, 0, 100, 205, 410 ); 

    //write to the motor
    ledcWrite( this -> ledcChannelNum, ledcMicros );

}