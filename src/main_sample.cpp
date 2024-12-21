#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void resize_callback(GLFWwindow *window, int width, int height);

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW Window object
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "OpenGL Sample", NULL, NULL);

    // Make the window the current OpenGL context
    glfwMakeContextCurrent(window);

    // Make a viewport
    // glViewport(0, 0, 1920, 1080);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

// resize callback
void resize_callback(GLFWwindow *window, int width, int height)
{
    // Make a viewport
    glViewport(0, 0, 1920, 1080);
}