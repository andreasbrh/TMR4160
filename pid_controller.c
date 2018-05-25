#include <stdio.h>
#include "phidget_connections.h"
#include "pid_controller.h"
#include "visualization.h"
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>


/* PID_CONTROLLER
 * This file handles pid_calculations, resulting in plot values as well as the
 * value to set the servomotor too.
 */




// PID constants
static double Kp = 0.3;
static double Ki = 0.2;
static double Kd = 0.6;

// Range of what the motor of the boat accepts, found by tuning
static double center = 95;
static double radius = 15;
// Variables related to finding execution time of PID function
LARGE_INTEGER frequency;
LARGE_INTEGER start;
LARGE_INTEGER end;






/*
 * Short utility function used when you want a value to never be outside an interval.
 */

double clampValues(float value, double topBoundary, double botBoundary) {
    if(value > topBoundary) {
        value =  topBoundary;
    } else if (value < botBoundary) {
        value = botBoundary;
    }
    return value;
}


/* ------
 * PID FUNCTION - Calculates how much power to set the boat to use.
 * Returns a struct containing each term separately, proportional term, integral term, derivative term
 * as well as the result(power)
 */


PIDOutput pid_calculate(double target_pos, double current_pos) {
    // Create variables
    static double error, proportional, derivative, pwm, integral, last_error,dt;
    // Check if firstrun, to make sure theres no random numbers in memory.
    // setup of time
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    // makes sure time doesnt become zero. Dividing by zero is not recommended
    usleep(10000);

    // calculates the individual terms
    error = target_pos - current_pos;
    proportional = Kp * error;
    integral += error * Ki;
    derivative = error - last_error;

    // makes sure the time doesnt become negative
    if(end.QuadPart < start.QuadPart) {
        QueryPerformanceCounter(&end);
    }
    // calculate the time
    dt = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    // finds power
    pwm = proportional + integral*dt + (Kd * derivative)/dt;

    // clamps the power to be within -1 or 1
    pwm = clampValues(pwm,1,-1);
    // Converts pwm to the value to be given to servo
    pwm = center + pwm*radius;
    // Sets up the return value
    PIDOutput res = {pwm, error,dt};
    // find time at end of loop (to calculate execution time)
    QueryPerformanceCounter(&end);
    return res;
}




