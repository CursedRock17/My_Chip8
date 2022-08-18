#ifndef GRAPHICSCLASS_h
#define GRAPHICSCLASS_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "../chip8.h"

#include <cmath>
#include <iostream>

class Graphics {
public:
    Graphics();
    ~Graphics();

    int GraphicsRun(Chip chip);
    void TerminateGraphics();

private:
    GLFWwindow* window;
    GLuint screenData[32][64][3];

    void GraphicsUpdate(const Chip& c8);

};

#endif