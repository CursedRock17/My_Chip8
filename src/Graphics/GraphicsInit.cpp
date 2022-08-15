#include <GLFW/glfw3.h>

int GraphicsInit(){
    
    GLFWwindow* window;

    // Init the libraries
    if(!glfwInit())
        return -1;
    
    // Create a windowed mode window and contexr
    window = glfwCreateWindow(640, 480, "Chip8", NULL, NULL);

    if(!window){
        glfwTerminate();
        return -1;
    }

    //Make the current window's context
    glfwMakeContextCurrent(window);

    /* Loop unitl user closes the window */

    while(!glfwWindowShouldClose(window))
    {
        //Render then swap buffers, then poll for events
        //glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}