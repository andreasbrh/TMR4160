#ifndef DATAMETODER_VISUALIZATION_H
#define DATAMETODER_VISUALIZATION_H


#include "../Visualization/GLFW/glfw3.h"
typedef struct
{
    double currentPos;
    double targetPos;
    float error;
    int running;
} BoatData;


GLFWwindow* window;
void *init(void*);

#endif //DATAMETODER_VISUALIZATION_H
