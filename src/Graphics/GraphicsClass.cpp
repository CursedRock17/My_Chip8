#include "GraphicsClass.h"

Graphics::Graphics(){}
Graphics::~Graphics(){}

int Graphics::GraphicsRun(Chip chip){
    // Init the libraries
    if(!glfwInit())
        return -1;
    
    // Create a windowed mode window and contexr
    window = glfwCreateWindow(640, 480, "Chip8", NULL, NULL);

    if(!window){
        glfwTerminate();
        return -1;
    }

        // Clear screen
    for (int y = 0; y < 32; ++y)
        for (int x = 0; x < 64; ++x)
            screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;

    // Create a texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)screenData);

    // Set up the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Enable textures
    glEnable(GL_TEXTURE_2D);

    //Make the current window's context
    glfwMakeContextCurrent(window);

    /* Loop unitl user closes the window */

    while(!glfwWindowShouldClose(window))
    {
        //Render then swap buffers, then poll for events
        //glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        GraphicsUpdate(chip);

        glfwPollEvents();
    }

    glfwTerminate();


    return 1;
}

void Graphics::GraphicsUpdate(const Chip& c8)
{
    // Update pixels
    for (int y = 0; y < 32; ++y)
        for (int x = 0; x < 64; ++x)
            if(0 == c8.graphics[y*64 + x])
                screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0; // Black
            else
                screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0xFF; // White

    // Update texture
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)screenData);


    glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
        glTexCoord2d(1.0, 0.0); glVertex2d(64, 0.0);
        glTexCoord2d(1.0, 1.0); glVertex2d(64, 32);
        glTexCoord2d(0.0, 1.0); glVertex2d(0.0, 32);
    glEnd();
}