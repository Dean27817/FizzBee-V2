#include "Arduino.h"
#include "reciver.h"
#include "motor.h"
#include "kinimatics.h"
#include "accel.h"
#include "OTAUpdates.h"
#include "led.h"

//Reciver object
Reciver controller( 44 );

//motor objects and variables
Motor motor1( 13, 1, 50 );
Motor motor2( 11, 2, 50 );
float motorSpeeds[2];

//acceleromiter objects and variables
Accel accel1;
Accel accel2;
float accel1Mag;
float accel2Mag;
float currentAngle;
TwoWire I2CBus = TwoWire( 0 );

//kinimatics object
///////////////////////////////ONE IS A PLACEHOLDER HERE, REPLACE LATER!!!!!
kinimatics kine( 1 );

//heading LED objects
LED header1( 9 );//, 5 );
LED header2( 8 );//, 5 );

//otaUpdate object
OTAUpdates OTA;
bool wifiEnabled = 0;

//the second loop, used for multithreading
void loop2( void *pvParameters );


void setup() 
{

    //begin communication with the acceleromiters
    I2CBus.begin( 5, 4, 100000);
    accel1.start( I2CBus, 0x19 );
    accel2.start( I2CBus, 0x18 );

    //start motors at 0
    motor1.setSpeed( 0 );
    motor2.setSpeed( 0 );

        xTaskCreatePinnedToCore
    (
        loop2,
        "Loop2",
        1000,
        NULL,
        10,
        NULL,
        0
    );

    // Load persisted kinimatics tuning values after filesystem is mounted
    kine.loadValues();

    while( controller.input.throttle > -0.9 )
    {
        delay( 100 );
    }
}

void loop() 
{
    //variables to be used later
    float left;
    float right; 
    float stickMag;
    float stickAngle;

    //code that has to run every update
    OTA.loop();

    //tank drive mode
    if( controller.input.throttle < -0.8 )
    {
        left = controller.input.x + controller.input.y;
        right = controller.input.x - controller.input.y;
        //set the motor speeds
        motor1.setSpeed( left );
        motor2.setSpeed( right );
        header1.stopped();
        header2.stopped();
    }

    //melty mode
    else
    {
        //find the radial acceleration of both acceleromiters
        accel1Mag = accel1.getAccel();
        accel2Mag = accel2.getAccel();

        //find the rotational velocity based on the two accelerations
        currentAngle = kine.findAngle( accel1Mag, accel2Mag );

        //gets values from the stick to polar coordanates
        stickMag = sqrt( pow( controller.input.x, 2)  + pow ( controller.input.y, 2 ) );
        stickAngle = atan2( controller.input.y, controller.input.x );

        //find the speeds the motors need to be at any given instant
        kine.findMotorSpeeds
        (
            motorSpeeds,
            currentAngle,
            stickMag,
            stickAngle, 
            controller.input.throttle
        );

        //set the motors to the speeds calculated above
        motor1.setSpeed( motorSpeeds[0] );
        motor2.setSpeed( motorSpeeds[1] );
    }
}

void loop2( void *pvParameters )
{
    while( 1 )
    {
        controller.loop();
            //find the radial acceleration of both acceleromiters
        accel1Mag = accel1.getAccel();
        accel2Mag = accel2.getAccel();

        //find the current angle based on the two accelerations
        currentAngle = kine.findAngle( accel1Mag, accel2Mag );

        if( controller.input.throttle > -0.8 )
        {
            //update the heading LED
            header1.onLoop( currentAngle );
        }
    }

}
