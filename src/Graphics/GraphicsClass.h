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
private:
    GLFWwindow* window;
    GLuint screenData[32][64][3];

    void GraphicsUpdate(const Chip& c8);

    //Creating the Texure
    GLuint img_width = 64;
    GLuint img_height = 32;
    const std::uint16_t modifier = 10;

    //General Window Uses
    int WINDOW_HEIGHT = img_height * modifier;
    int WINDOW_WIDTH = img_width * modifier;
    int PIXEL_SIZE = 12;
};

//For the GLFW input, needs to be SDL
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void return_action(int hex);
#endif