#include <stdio.h>
#include <stdlib.h>
#include <phidget22.h>
#include <math.h>
#include <unistd.h>
#include <Windows.h>
#include <pthread.h>
#include <pid_controller.h>
#include "visualization.h"
#include "phidget_connections.h"
#include "pid_controller.h"
#include "Visualization/GLFW/glfw3.h"


int main(){
    // Initialize connection
    double currentpos = connection_creater();
    double targetpos;
    float time;
    // Open file find target position
    FILE *fpInput = fopen("setpoints.txt", "r");
    fscanf(fpInput, "%lf", &targetpos);
    //  Prepare file to write output data.
    FILE *fpOutput;
    fpOutput = fopen("Output.txt", "a");

    // Sets the initial boatvalues. See visualization.h for struct definition
    BoatData boatData = {currentpos,targetpos,targetpos-currentpos,1};

    // Create separate thread for visualization
    pthread_t visualizationThread;
    if(pthread_create(&visualizationThread, NULL, init, &boatData)) {

        printf("Thread error");
        return 1;

    }
    // Main loop
    while(boatData.running) {
        // Get data from PID
        PIDOutput data = pid_calculate(targetpos,currentpos);
        // Tell the boat to go
        setServoPosition(data.power);
        // Update current position
        currentpos = getPosition();
        // Update for visualization part.
        boatData.currentPos = currentpos;
        time += data.dt;
        // Print output to file
        fprintf(fpOutput,"%lf\t%lf\t%lf\t%lf\t%lf\n",time,data.error,boatData.currentPos, boatData.targetPos,data.power);

    }
    // Close files
    fclose(fpInput);
    fclose(fpOutput);
    return 0;
}
