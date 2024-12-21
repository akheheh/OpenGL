// GLFW: Windowing and I/O library
// GLAD: Library which loads OpenGL functions for C++
// Always include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// Make a vertex shader to process the vertex data now in GPU memory
// Shaders are made using GLSL
// Compile shader for use

// A simple vertex shader
// Use OpenGL 3.3
// Make a vec3 input
// Assign the vec3 to gl_Position, a vec4 variable
const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

// A simple fragment shader
// Use OpenGL 3.3
// Make a vec4 output
// Set color to some orange RGBA
const char *fragmentShaderSource = R"( 
#version 330 core
out vec4 FragColor;
uniform float alpha_t;
vec4 sandColor();
void main() {
    float animateChannel = abs(sin(alpha_t));
    FragColor = vec4(0.25f, animateChannel, 0.9f, 1.0f);
    //FragColor = sandColor();
}
vec4 sandColor() {
    return vec4(1.0f, 0.895f, 0.742f, 1.0f);
}
)";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void clearColor(GLFWwindow *window, int key, float clear[]);

int main()
{
    // First initialize GLFW
    glfwInit();
    // The major and minor hints mean, use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW to use the core-profile only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a Window object
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "LearnOpenGL", NULL, NULL);

    // If window is NULL, output error and close program
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window\n";
        glfwTerminate();
        return -1;
    }
    // Make the context of the window the main context for the current thread
    glfwMakeContextCurrent(window);

    // Initialize GLAD before using any OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Create a viewport at the lower left corner of the window object
    glViewport(0, 0, 1920, 1080);

    // Callback for resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Vertex shader
    // Vertices for a triangle
    // OpenGL only works with 3D coordinates with x, y, z values between [-1.0, 1.0], known as normalized device coordinates
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Vertex Buffer Object, VBO
    // Can store a large amount of vertices in the GPU memory
    // Advantage is send large amounts of data at once to the GPU, rather than a bit at a time
    // Sending data to GPU is slow, having it in memory can be used instantly
    unsigned int VBO;
    // Generate a VBO with ID 1
    glGenBuffers(1, &VBO);
    // Create a vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // Bind the new vertex buffer to GL_ARRAY_BUFFER buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);

    // Copy the vertex data array to the buffer's memory
    // glBufferData(buffer type to copy data to, size of data, the data to send to the buffer type, how we want the GPU to manage the data)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create a shader object, reference with an ID
    unsigned int vertexShader;
    // Call glCreateShader() to create a vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Link the shader source to the vertex shader object
    // glShaderSource(shader object to compile, how many strings the shader source has, shader source, NULL)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compile shader
    glCompileShader(vertexShader);

    // check for shader compilation result
    int success;
    char infoLog[512];
    // Get shader compilation info
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // Check if shader compilation successful
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment shader
    // Similar strategy with vertex shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders to a shader program
    unsigned int shaderProgram;
    // Create a shader program
    // glCreateProgram() creates a shader program and returns an ID reference for the shader program object
    shaderProgram = glCreateProgram();

    // Attach the shaders to the shader program and link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // How OpenGL should interpret vertex data before rendering
    // glVertexAttribPointer(which vertex attribute to configer, size of vertex attribute e.g. vec3, type of vector data, if we want to normalize data or not, stride aka space between vertex attributes, offset of type void)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Get the uniform shader variable
    int timeLoc = glGetUniformLocation(shaderProgram, "alpha_t");
    // Use the shader program
    // Ensure the application does not close after drawing, keep it running until the user closes it
    // All rendering happens in the while loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // render here
        // set a color to clear the screen with, known as clearColor
        glClearColor(0.25f, 0.3f, 0.82f, 1.0f);
        // glClear() fills the entire color buffer with whatever argument glClearColor() has
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // get the current time value
        float timeValue = glfwGetTime();
        // link the time value with the uniform variable
        glUniform1f(timeLoc, timeValue);
        glBindVertexArray(VAO);
        // Draw the triangle
        // glDrawArrays(OpenGL primative, starting index of vertex array, how many vertices to draw)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Remove the shader objects once they're linked to the program for efficiency
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    // Close the program and clean/free allocation
    glfwTerminate();
    return 0;
}

// Add an event listener and handler for resizing the window
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Handle user input
void processInput(GLFWwindow *window)
{
    // If user pressed the escape key, then call glfwWindowShouldClose(window to close, true)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float aKeyColor[] = {1.0f, 0.2f, 0.5f, 1.0f};
    float bKeyColor[] = {0.0f, 1.0f, 0.5f, 1.0f};
    clearColor(window, GLFW_KEY_A, aKeyColor);
    clearColor(window, GLFW_KEY_B, bKeyColor);
}

// function to re-render the clear color when key is pressed
void clearColor(GLFWwindow *window, int key, float clear[])
{
    if (glfwGetKey(window, key) == GLFW_PRESS)
    {
        glClearColor(clear[0], clear[1], clear[2], clear[3]);
        // glClear() fills the entire color buffer with whatever argument glClearColor() has
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw the triangle
        // glDrawArrays(OpenGL primative, starting index of vertex array, how many vertices to draw)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}