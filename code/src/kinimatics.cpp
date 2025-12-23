
#include <Arduino.h>
#include "kinimatics.h"
#include "accel.h"
#include <cmath>


//constructors for one and two acceleromiters
kinimatics::kinimatics( float knownRadiusIn )  
{
    this -> knownRadius = knownRadiusIn;
}


kinimatics::kinimatics( float knownRadiusIn, float foundRadiusIn )
{
    this -> knownRadius = knownRadiusIn;
    this -> foudRadius = foundRadiusIn;
}


//finds the current angular velocity wihth one acceleromiter
//measured in g
float kinimatics::findCurrentSpeed( float accel1 )
{
    float mpsAccel = accel1 * 9.8;
    float omega = sqrt( mpsAccel / knownRadius );

    return omega;
}

//finds the current angular velocity wihth two acceleromiters
//measured in g
float kinimatics::findCurrentSpeed( float accel1, float accel2 )
{
    float mpsAccel1 = accel1 * 9.8;
    float mpsAccel2 = accel2 * 9.8;
    float angveloc1 = sqrt( mpsAccel1 / knownRadius );
    float angveloc2 = sqrt( mpsAccel2 / ( knownRadius + foudRadius ) );

    float omega = ( angveloc1 + angveloc2 ) / 2;

    return omega;
}


//finds the current angle wihth one acceleromiter
//measured in g
float kinimatics::findAngle( float accel1 )
{
    //find the change in time since last check
    this -> deltaTime = ( millis() / 1000 ) - this -> lastTime;
    this -> lastTime = millis() / 1000;

    //finds the angle through the change ain time and angular velocity
    float currentSpeed = findCurrentSpeed( accel1 );
    this -> currentAngle = currentSpeed * deltaTime;

    return this -> currentAngle;
}


//finds the current angle wihth two acceleromiters
//measured in g
float kinimatics::findAngle( float accel1, float accel2 )
{
    //find the change in time since last check
    this -> deltaTime = ( millis() / 1000 ) - this -> lastTime;
    this -> lastTime = millis() / 1000;

    //finds the angle through the change ain time and angular velocity
    float currentSpeed = findCurrentSpeed( accel1, accel2 );
    this -> currentAngle = currentSpeed * deltaTime;

    return this -> currentAngle;
}


//finds the speed of the motor based on many factors
//returns values between -1 and 1
void kinimatics::findMotorSpeeds( float motorSpeeds[2], float curentRads, float translationSpeed, float translationRads, float spinSpeed )
{
    // Calculate spin percentage based on user-defined spin speed
    float spinFactor = static_cast<float>( spinSpeed ) / 100.0f; // Normalized spin speed ( 0 to 1 )
    
    // Calculate speed contributions for both motors
    motorSpeeds[1] = sin( curentRads - translationRads ) * translationSpeed * spinFactor * ( spinSpeed / 2 ); // For one motor
    motorSpeeds[0] = -sin( curentRads - translationRads ) * translationSpeed * spinFactor * ( spinSpeed / 2 ); // For the other motor
                                                                                                         //

    // Scale values based on spin speed limits
    motorSpeeds[1] = constrain( motorSpeeds[1] + ( spinSpeed / 2 ), spinSpeed / 2, spinSpeed );
    motorSpeeds[0] = constrain( motorSpeeds[0] + ( spinSpeed / 2 ), spinSpeed / 2, spinSpeed );
}


//calibrate the unknown radius based on the two acceleromiters
//this will only really work if the two are on the same side of the bot
void kinimatics::calibrateRadius( Accel &accel1in, Accel &accel2in )
{
    //variables to be used later
    float tempRadius = 0;
    int i;
    int numOfPoints = 100;
    float accel1;
    float accel2;

    //loops over taking 100 points to average
    for( i = 0; i < numOfPoints; i++ )
    {
        accel1 = accel1in.getAccel();
        accel2 = accel2in.getAccel();
        tempRadius += ( accel1 * knownRadius ) / ( accel1 + accel2 );
    }

    //sets the variable equal to the average
    this -> foudRadius = tempRadius / numOfPoints; 
}