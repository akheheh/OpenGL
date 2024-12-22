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
    //Create a GLFW window which loads GLAD and has resizing
    GLFWwindow *window = createOpenGLEnv(1920, 1080, "GL");

    // vertex data
    /*
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f};
*/
   /*float vertices[] = {
    // first triangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f,  0.5f, 0.0f,  // top left 
    // second triangle
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};*/

float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
     -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f, // top left

    //gradients
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

/*float colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};*/

//The top left and bottom right vertices are duplicates
//The square has 2 triangles therefore 6 vertices, when we only need 4
//We want to draw only the vertices as needed
//Create an element buffer object or EBO to store indices of unique vertices and tell OpenGL when to draw them
GLuint EBO;
glGenBuffers(1, &EBO);

GLuint indices[] = {
    //first triangle
    0, 1, 3,

    //second triangle
    1, 2, 3
};



    // Create and bind the Vertex Array
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // Create and bind the Vertex Buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Load vertext data into the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //This time we bind our EBO to the target GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Load the data into the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell the Vertex Array how to read data for the given attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(12 * sizeof(float)));

    // Turn on the attribute
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

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
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements() indicates we want to draw from an index buffer, aka the GL_ELEMENT_ARRAY_BUFFER
        //We want to draw from 6 indices which are of GLUint, with 0 offset from the element buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //Tell OpenGL to draw the triangles as lines (GL_LINE) and to do so for both the front and back of each triangle (GL_FRONT AND BACK)
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}