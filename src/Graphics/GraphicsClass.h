#ifndef GRAPHICSCLASS_h
#define GRAPHICSCLASS_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include "../chip8.h"

class Graphics {
public:
    Graphics();
    ~Graphics();

    int GraphicsRun(Chip chip);
    void TerminateGraphics();

private:
    GLFWwindow* window;
    uint8_t screenData[32][64][3];

    void GraphicsUpdate(const Chip& c8);
};

#endif