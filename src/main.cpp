#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLEnv/GLEnv.h>
#include <Shader/shader.h>
#include <STB/stb_image.h>
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

float clearColorArray[] = {0.0f, 0.0f, 0.0f, 1.0f};
auto GLView = VIEW{1920, 1080, clearColorArray};

auto [WIDTH, HEIGHT, CLEAR_COLOR] = GLView;

int main()
{
    GLFWwindow *window = createOpenGLEnv(1920, 1080, "GL");

    // Make a shader program
    Shader a_Shader("vertex_shader.glsl", "fragment_shader.glsl");
    // Call it once, resize callback will call it again with resize dimensions

    // Make a VAO, VBO, and index buffer object aka Element Buffer Object aka EBO
    GLuint VAO, VBO, indexBuffer;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex data: vertex coordinates, vertex colors, texture coordinates
    float vertexData[] = {
        // Positions         //Colors            //Texture coordinates
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}; // top left

    GLuint indexArray[] = {
        0, 1, 3,
        1, 2, 3};

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

    // Load our texture
    int width, height, colorChannels;

    unsigned char *textureData = stbi_load("./images/beach.jpg", &width, &height, &colorChannels, 0);

    // Make a GL texture object
    GLuint textureObj;
    glGenTextures(1, &textureObj);

    // Bind to GL_TEXURE_2D
    glBindTexture(GL_TEXTURE_2D, textureObj);

    // load texture data to the texture object binding

    // If texture is RGBA aka 4 color channels, use GL_RGBA, otherwise use GL_RGB
    if (colorChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    }

    // Make a mipmap for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // texture cleanup
    stbi_image_free(textureData);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        GLGeneralSetup(window, CLEAR_COLOR);

        // Draw the triangle
        a_Shader.use();

        glBindTexture(GL_TEXTURE_2D, textureObj);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}