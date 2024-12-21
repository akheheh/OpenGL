#ifndef GL_ENV_H
#define GL_ENV_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow *window);
void resizeView(GLFWwindow *window, int width, int height);
GLFWwindow *createOpenGLEnv(const int width, const int height, const char *windowTitle);
void GLGeneralSetup(GLFWwindow *window, float *clearColor);

#endif