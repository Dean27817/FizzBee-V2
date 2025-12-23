#include <Arduino.h>
#include "accel.h"
#include <cmath>


#ifndef KINIMATICS_H
#define KINIMATICS_H

class kinimatics
{
    public:

    kinimatics( float knownRadiusIn );
    kinimatics( float knownRadiusIn, float foundRadiusIn );

    ///////////////////////////////////////////////////////////////////////////////////
    //////////////////                  Two acceleromiter variation     ///////////////
    ///////////////////////////////////////////////////////////////////////////////////

    //finds the angle based on the two acceleration values in g
    float findAngle( float accel1, float accel2 );

    //gets the current rotational velocity of the robot based on the radial acceleration and radius
    float findCurrentSpeed( float accel1, float accel2 );

    //estimates the radius from the center of the circule to the first acceleromiter
    float findRadius( float accel1, float accel2 );

    //calibrate the unknown radius based on the two acceleromiters
    //this will only really work if the two are on the same side of the bot
    void calibrateRadius( Accel &accel1in, Accel &accel2in );

    ///////////////////////////////////////////////////////////////////////////////////
    //////////////////                  One acceleromiter variation     ///////////////
    ///////////////////////////////////////////////////////////////////////////////////

    //finds the angle based on the two acceleration values in g
    float findAngle( float accel1 );

    //gets the current rotational velocity of the robot based on the radial acceleration and radius
    float findCurrentSpeed( float accel1 );

    //estimates the radius from the center of the circule to the first acceleromiter
    float findRadius( float accel1 );



    //finds the speed of the motor based on many factors
    //returns values between -1 and 1
    void findMotorSpeeds( float motorSpeeds[2], float curentRads, float translationSpeed, float translationRads, float spinSpeed );

    private:

    //the two radiuses
    //used to find angularVelocity from acceleration
    float knownRadius;
    float foudRadius;

    //time variables
    float deltaTime = 0;
    float lastTime = 0;

    //angle
    float currentAngle = 0;
};

#endif