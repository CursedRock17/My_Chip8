#ifndef GRAPHICSCLASS_h
#define GRAPHICSCLASS_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>

class Graphics {
public:
    Graphics();
    ~Graphics();

    int GraphicsRun();

private:
    GLFWwindow* window;

    void GraphicsUpdate();
};

#endif