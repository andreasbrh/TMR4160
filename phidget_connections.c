#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#include <phidget22.h>
#endif

/* This file creates and manages the phidget connections. It consists of 3 functions:
 * connection_creater is initial setup, and returns the initial position of the boat
 * getPosition() finds the current position of the boat. It converts the position from 5-3.5 to 0-1 instead for
 * better scale.
 * setServoPosition(double) tells the ship how much power it should use.
 */

// global handles, is used to communicate with motor/sensor
static PhidgetVoltageInputHandle handle;
static PhidgetRCServoHandle servoHandle;


// Connections are created by telling handles that a connection will come, then wait for it to be attached
int connection_creater(void) {

    // Setup sensor connection
    PhidgetVoltageInput_create(&handle); // Phidgetsensor
    Phidget_openWaitForAttachment((PhidgetHandle) handle, 10000);
    double voltage;
    PhidgetVoltageInput_getVoltage(handle,&voltage);


    // Setup servo connection
    PhidgetRCServo_create(&servoHandle);
    Phidget_openWaitForAttachment((PhidgetHandle) servoHandle, 10000);
    setServoPosition(voltage);
    return &voltage;
}

// Get position from sensor
double getPosition() {
    double position;
    PhidgetVoltageInput_getVoltage(handle, &position);
    // conversion to 0-1 scale (0 being base positon)
    printf("%lf\t",position);
    position = -1*(((position-3.5)/1.5 - 1));
    return position;
}

// Tells motor how much power it should use
void setServoPosition(double power) {
    PhidgetRCServo_setTargetPosition((PhidgetRCServoHandle) servoHandle, power);
    PhidgetRCServo_setEngaged(servoHandle, 1);

}




