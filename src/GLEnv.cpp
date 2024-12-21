#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLEnv/GLEnv.h>
#include <iostream>
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        glfwTerminate();
        exit(0);
    }
}

void resizeView(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Create a GLFW OpenGL environment using GLAD
GLFWwindow *createOpenGLEnv(const int width, const int height, const char *windowTitle)
{
    if (!glfwInit())
    {
        std::cout << "ERROR: GLFW initialization fail" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "ERROR: GLFW fail" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: GLAD fail" << std::endl;
        return nullptr;
    }

    glfwSetFramebufferSizeCallback(window, resizeView);
    return window;
}

void GLGeneralSetup(GLFWwindow *window, float *clearColor)
{
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}