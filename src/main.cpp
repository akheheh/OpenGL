#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLEnv/GLEnv.h>
/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/

struct VIEW
{
    int WIDTH;
    int HEIGHT;
    float *clearColor;
};

// Vertex shader
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 vertex;
    layout (location = 1) in vec3 vColor;
    out vec4 fragmentColor;
    void main() {
        gl_Position = vec4(vertex, 1.0f);
        fragmentColor = vec4(vColor, 1.0f);
    }
)";

// Fragment shader
const char *fragmentShaderSource = R"(
    #version 330 core
    in vec4 fragmentColor;
    out vec4 pixelColor;
    void main() {
        pixelColor = fragmentColor;
    }
)";

float clearColorArray[] = {0.0f, 0.0f, 0.0f, 1.0f};
auto GLView = VIEW{1920, 1080, clearColorArray};

auto [WIDTH, HEIGHT, CLEAR_COLOR] = GLView;

int main()
{
    GLFWwindow *window = createOpenGLEnv(1920, 1080, "GL");

    // vertex data
    float vertices[] = {
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        0.0f,
        0.75f,
        0.0f,

        0.75f,
        0.25f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        0.0f,
        0.75f,
        0.0f,
    };

    float colors[] = {
        0.282f, 0.239f, 0.545f,
        0.282f, 0.239f, 0.545f,
        0.282f, 0.239f, 0.545f,

        0.482f, 0.408f, 0.933f,
        0.482f, 0.408f, 0.933f,
        0.482f, 0.408f, 0.933f};

    // create shaders and a shader program
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

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create and bind a VAO, create, bind and load a buffer, set up attribute configurations
    GLuint VAO, VBO;
    GLuint colorBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colorBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(1);

    // Call it once, resize callback will call it again with resize dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        GLGeneralSetup(window, CLEAR_COLOR);

        // Draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}