#include "GraphicsClass.h"

Graphics::Graphics(){}
Graphics::~Graphics(){}

int Graphics::GraphicsRun(Chip chip){
    // Init the libraries
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Vertices coordinates
    GLfloat vertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
	    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
    };

	// Indices for vertices order
	GLuint indices[] =
	{
        0, 2, 1, //Upper Traingle
        0, 3, 2 //Lower Triagnke
	};


    //Need to do this to make
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(1024, 768, "Chip8", NULL, NULL);

    if(!window){
        std::cout << "FAiled to create Wuindow" << std::endl;
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 1024, 768);

    //Don't know why I have to copy the full path
    Shader shaderProgram("/Users/cursedrock17/Documents/Coding/CPP/Chip8/src/Graphics/default.vert", "/Users/cursedrock17/Documents/Coding/CPP/Chip8/src/Graphics/default.frag");

    //Create and bind the starting array
    VAO VAO1;
    VAO1.Bind();
    
    //Create our other obects
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    //Creating the Texure
    GLuint img_width = 64;
    GLuint img_height = 32;
    GLuint numColCh = 3;

    //Clears the screen buy setting all the bits to black
    for(int w = 0; w < 64; w++){
        for(int h = 0; h < 32; h++){
            screenData[h][w][0] = screenData[h][w][1] = screenData[h][w][2] = 0;
        }
    }

    GLuint texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //Generate the textrure
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_INT, screenData);
    
    //Going to need to change the image when its moved around and closer
    //We can use soemthing like GL_NEATest with glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //Might as well free the data from memory
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex_uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex_uni, 0);

// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.0f, 0.2f, 0.5f, 0.0f); 
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

        //Can check to see if we need to color each pixel
        //GraphicsUpdate(chip);
        
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
        //You can only create a scale after creating the shader program
        glUniform1f(uniID, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture);
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
    glDeleteTextures(1, &texture);

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
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
}