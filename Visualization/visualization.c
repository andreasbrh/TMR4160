#include <glad/glad.h>
#include "linmath.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include <visualization.h>

/* VIZUALIZATION.C
 *
 *
 * This file handles all visualization. It consists of error callback, event callback, declaration of shaders
 * and the main visualization loop.
 *
 */


// ERROR_CALLBACK - For printing errors
static void error_callback(int code, const char *description) {
    printf("%i: ", code);
    printf(description);
    printf("\n");
}



// VERTEX - Creates a construct for setting up the triangle(which represents the boat)
struct Vertex
{
    float x, y;
    float r, g, b;
} ;

// The actual Vertex used to draw.
struct Vertex BoatVertices[3] =
        {
                {  -1.35f, 1.0f, 1.f, 0.f, 0.f },
                {  -1.35f, 0.8f, 1.f, 0.f, 0.f },
                {   -1.0f,  0.9f, 0.f, 0.6f, 0.0f }
        };

// Declaration of shaders, vertex and fragment. Originally designed for rotation, so it has some extras.
static const char* vertex_shader_text =
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";
static const char* fragment_shader_text =
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";
// Key action for escape = Quit
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

}


// INIT -  Main vizualization loop
void *init(void *data) {
    // Currentpos goes from 0-1, while the pixels goes from -1.35 -> 1.35 in X. Conversion is needed
    BoatData* boatData = (BoatData*)data;
    float currentpos_visualized = boatData->currentPos*1.35;


    // Declare variables
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;


    // ErrorCallBack gives easy(er) to understand error messages
    glfwSetErrorCallback(error_callback);
    // Initialise glfw
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Set version number
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // KeyCallback is for events - like clicking escape to close window.
    glfwSetKeyCallback(window, key_callback);
    // MakeContextCurrent is part of setup
    glfwMakeContextCurrent(window);
    // Load shaders
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    // Interval for swapping buffers
    glfwSwapInterval(1);
    // Set up buffers
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BoatVertices), BoatVertices, GL_STATIC_DRAW);


    // Create shaders
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);


    // Create program and attach shaders
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // Sets background to blue
    glClearColor(0.0f,0.0f,0.8f,0.0f);

    // Tools for drawing
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    // Enable generic vertex attribute array for both vPos and vCol - see declaration of shader
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));

    // The actual drawing
    while (!glfwWindowShouldClose(window))
    {

        // finds buffersize for window
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        // Open program
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);

        // The actual drawing happens here
        mat4x4_identity(m);
        // Move the boat to current position
        mat4x4_translate(m, currentpos_visualized,0,0);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Same but for target position.
        mat4x4_identity(m);
        mat4x4_translate(m, boatData->targetPos,0,0);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Swapbuffers and check for events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Termination
    glfwDestroyWindow(window);
    glfwTerminate();
    // Following line makes the main loop in main.c close upon stopping program
    boatData->running = 0;

    exit(EXIT_SUCCESS);
}