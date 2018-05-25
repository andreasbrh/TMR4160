//
// Created by Andreas on 09.05.2018.
//

#ifndef DATAMETODER_PID_CONTROLLER_H
#define DATAMETODER_PID_CONTROLLER_H
typedef struct
{
    float power;
    float error;
    float dt;
} PIDOutput;

PIDOutput pid_calculate(double, double);
#endif //DATAMETODER_PID_CONTROLLER_H
