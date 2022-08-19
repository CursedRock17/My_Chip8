#ifndef GRAPHICSCLASS_h
#define GRAPHICSCLASS_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Shading/shaderClass.h"
#include"../Shading/VAO.h"
#include"../Shading/VBO.h"
#include"../Shading/EBO.h"
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

    //Creating the Texure
    GLuint img_width = 64;
    GLuint img_height = 32;

};

#endif