#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLEnv.h"

// Array destructuring for View
struct VIEW
{
    int WIDTH;
    int HEIGHT;
};

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vCol;
uniform float time;
out vec4 fragmentColor;

void main() {
    gl_Position = vec4(aPos.x, aPos.y + 0.5f * sin(time), aPos.z, 1.0f);
    fragmentColor = vec4(vCol.r + sin(time), vCol.g, vCol.b, 1.0f);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
in vec4 fragmentColor;
out vec4 pixelColor;
void main() {
    pixelColor = fragmentColor;
}
)";

auto GLView = VIEW{1920, 1080};

auto [WIDTH, HEIGHT] = GLView;

int main()
{
    GLFWwindow *window = createOpenGLEnv(WIDTH, HEIGHT, "OpenGLEnv");
    // vertices to send as data to the GPU
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f};

    // Create a Vertex Array and bind it
    // GLuint is unsigned int
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create a vertex buffer, bind it and load data to the buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Tell OpenGL how the 0th attribute loads data from the vertex buffer, and turn it on
    // NOTE: glVertexAttribPointer() can only be called after glBindBuffer()
    // glVertexAttribPointer() sets whatever GL_ARRAY_BUFFER is binded to as the source buffer
    // Meaning, if there is nothing binded to GL_ARRAY_BUFFER beforehand then it will not draw
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(9 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // The vertices array has 9 floats, 1 float = 4 bytes, allocate 36 bytes for the buffer
    // load the vertices array to these allocated memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile a vertex and fragment shader
    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glViewport(0, 0, WIDTH, HEIGHT);

    int timeLoc = glGetUniformLocation(shaderProgram, "time");
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float time = glfwGetTime();
        glUniform1f(timeLoc, time);

        glClearColor(0.95f, 0.895f, 0.742f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();

    return 0;
}

/*

Modern OpenGL recommended workflow

//initialization
glGenVertexArrays
glBindVertexArray

glGenBuffers
glBindBuffer
glBufferData

glVertexAttribPointer
glEnableVertexAttribArray

glBindVertexArray(0)

glDeleteBuffers //you can already delete it after the VAO is unbound, since the
                //VAO still references it, keeping it alive (see comments below).

...

//rendering
glBindVertexArray
glDrawWhatever

*/