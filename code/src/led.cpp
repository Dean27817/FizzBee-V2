#include <Arduino.h>
#include "led.h"

LED::LED( int ledPinIn, int numPixlesIn )
{
    //set object variables
    this -> ledPin = ledPinIn;
    this -> numPixles = numPixlesIn;

    //create the object and begin
    pixles = new Adafruit_NeoPixel( this -> numPixles, this -> ledPin, NEO_GRB + NEO_KHZ800);
    pixles -> begin();
}


//the robot is stopped with no errors
void LED::stopped() 
{
    //makes sure there is delay
    if( ( lastTime - millis() ) >= 1000 )
    {
        //set the pixles
        pixles -> clear();

        //if we are turning on the lights, set it to green
        if( increasing )
        {
            pixles -> setPixelColor( currentLED, pixles -> Color( 0, 150, 0 ) );
        }
        else
        {
            pixles -> setPixelColor( currentLED, pixles -> Color( 0, 0, 0 ) );
        }

        pixles -> show();
        lastTime = millis();

        //increase the led count
        if( currentLED >= numPixles )
        {
            currentLED++;
        }
        else
        {
            currentLED = 0;
            increasing = !increasing;
        }
    }
}


//resets all the variables
void LED::reset()
{
    currentLED = 0;
    lastTime = 0;
    increasing = 1;
}