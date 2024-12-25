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
    layout (location = 1) in vec3 vertexColor;
    out vec4 fragmentColor;
    void main() {
        gl_Position = vec4(vertex, 1.0f);
        fragmentColor = vec4(vertexColor, 1.0f);
        //fragmentColor = vec4(vertexColor, 1.0f);
    }
)";

// Fragment shader
const char *fragmentShaderSource = R"(
    #version 330 core
    in vec4 fragmentColor;
    out vec4 pixelColor;
    void main() {
        //pixelColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
        pixelColor = fragmentColor;
    }
)";

float clearColorArray[] = {0.0f, 0.0f, 0.0f, 1.0f};
auto GLView = VIEW{1920, 1080, clearColorArray};

auto [WIDTH, HEIGHT, CLEAR_COLOR] = GLView;

int main()
{
    // Create a GLFW window which loads GLAD and has resizing
    GLFWwindow *window = createOpenGLEnv(1920, 1080, "GL");

    // Vertex data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Vertex color data
    float vertexColors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f};

    // Create 2 VBO's
    GLuint VBOs[2];
    // Create 2 buffers, and array to store the ID references for each buffer
    glGenBuffers(2, VBOs);

    // Bind the the first new buffer to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    // Load data to the GL_ARRAY_BUFFER binding
    // glBufferData(target, how much data, the data itself, a hint on how to use the data for optimization uses)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Repeat for the second buffer, bind to GL_ARRAY_BUFFER and load data
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

    // Create a VAO
    GLuint VAO;
    // Create 1 vertex array, with an array to store the ID references of each vertex array
    glGenVertexArrays(1, &VAO);
    // Bind the VAO for drawing
    glBindVertexArray(VAO);

    // Tell OpenGL how to interpret the buffer data for each attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(1);

    std::cout << VAO << std::endl;
    std::cout << &VAO << std::endl;

    // Create a vertex shader, fragment shader, and a shader program
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

    /*
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;
    */

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        GLGeneralSetup(window, CLEAR_COLOR);

        // Draw the triangle
        glUseProgram(shaderProgram);
        // While it's technically not necessary to call glBindVertexArray() for this example,
        // We should anyway, because if we have multiple VAOs then we will introduce unintended behavior
        // AKA we might be drawing from a different VAO than we intended, or using a library which has its own VAOs
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements() indicates we want to draw from an index buffer, aka the GL_ELEMENT_ARRAY_BUFFER
        // We want to draw from 6 indices which are of GLUint, with 0 offset from the element buffer

        // Tell OpenGL to draw the triangles as lines (GL_LINE) and to do so for both the front and back of each triangle (GL_FRONT AND BACK)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}