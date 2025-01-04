#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLEnv/GLEnv.h>
#include <iostream>

// Handle user input, e.g. close when esc key is clicked
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        glfwTerminate();
        exit(0);
    }
}

// callback to redraw when resizing view
void resizeView(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Create a GLFW OpenGL environment using GLAD, returning a GLFWwindow object
GLFWwindow *createOpenGLEnv(const int width, const int height, const char *windowTitle)
{
    if (!glfwInit())
    {
        std::cout << "ERROR: GLFW initialization fail" << std::endl;
        return nullptr;
    }

    // Set which OpenGL to use: 3.3 with the CORE profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object
    GLFWwindow *window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Error handling
    if (window == NULL)
    {
        std::cout << "ERROR: GLFW fail" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Make the new window the current contxt
    glfwMakeContextCurrent(window);

    // Load glad for using OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: GLAD fail" << std::endl;
        return nullptr;
    }

    // redraw the view using a resizeView callback whenever resizing window
    glfwSetFramebufferSizeCallback(window, resizeView);
    return window;
}

// Set the clear color
void GLGeneralSetup(GLFWwindow *window, float *clearColor)
{
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}